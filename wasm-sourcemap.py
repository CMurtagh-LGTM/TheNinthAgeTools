#!/usr/bin/env python3
# Copyright 2018 The Emscripten Authors.  All rights reserved.
# Emscripten is available under two separate licenses, the MIT license and the
# University of Illinois/NCSA Open Source License.  Both these licenses can be
# found in the LICENSE file.

import argparse
import json
import logging
import os
import re
import sys
from math import floor, log

import utils

logger = logging.getLogger('wasm-sourcemap')

def parse_args(args):
  parser = argparse.ArgumentParser(prog='wasm-sourcemap.py', description=__doc__)
  parser.add_argument('wasm', help='wasm file')
  parser.add_argument('-o', '--output', help='output source map')
  parser.add_argument('--basepath', help='base path for source files, which will be relative to this')
  return parser.parse_args(args)

class Prefixes:
  def __init__(self, base_path=None):
    self.base_path = utils.normalize_path(base_path) if base_path is not None else None
    self.cache = {}

  def resolve(self, name):
    if name in self.cache:
      return self.cache[name]

    source = name

    # If prefixes were provided, we use that; otherwise if base_path is set, we
    # emit a relative path. For files with deterministic prefix, we never use
    # a relative path, precisely to preserve determinism, and because it would
    # still point to the wrong location, so we leave the filepath untouched to
    # let users map it to the proper location using prefix options.
    if not (self.base_path is None):
      try:
        source = os.path.relpath(source, self.base_path)
      except ValueError:
        source = os.path.abspath(source)
      source = utils.normalize_path(source)

    self.cache[name] = source
    return source

# SourceMapPrefixes contains resolver for file names that are:
#  - "sources" is for names that output to source maps JSON
class SourceMapPrefixes:
  def __init__(self, base_path):
    self.sources = Prefixes(base_path=base_path)


def encode_vlq(n):
  VLQ_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
  x = (n << 1) if n >= 0 else ((-n << 1) + 1)
  result = ""
  while x > 31:
    result = result + VLQ_CHARS[32 + (x & 31)]
    x = x >> 5
  return result + VLQ_CHARS[x]

def read_var_uint(wasm, pos):
  n = 0
  shift = 0
  b = ord(wasm[pos:pos + 1])
  pos = pos + 1
  while b >= 128:
    n = n | ((b - 128) << shift)
    b = ord(wasm[pos:pos + 1])
    pos = pos + 1
    shift += 7
  return n + (b << shift), pos

def get_code_section_offset(wasm):
  logger.debug('Read sections index')
  pos = 8

  while pos < len(wasm):
    section_id, pos_ = read_var_uint(wasm, pos)
    section_size, pos = read_var_uint(wasm, pos_)
    if section_id == 10:
      return pos
    pos = pos + section_size

def remove_dead_entries(entries):
  # Remove entries for dead functions. It is a heuristics to ignore data if the
  # function starting address near to 0 (is equal to its size field length).
  block_start = 0
  cur_entry = 0
  while cur_entry < len(entries):
    if not entries[cur_entry]['eos']:
      cur_entry += 1
      continue
    fn_start = entries[block_start]['address']
    # Calculate the LEB encoded function size (including size field)
    fn_size_length = floor(log(entries[cur_entry]['address'] - fn_start + 1, 128)) + 1
    min_live_offset = 1 + fn_size_length # 1 byte is for code section entries
    if fn_start < min_live_offset:
      # Remove dead code debug info block.
      del entries[block_start:cur_entry + 1]
      cur_entry = block_start
      continue
    cur_entry += 1
    block_start = cur_entry


def extract_comp_dir_map(text):
  compile_unit_pattern = re.compile(r"0x[0-9a-f]*: DW_TAG_compile_unit")
  stmt_list_pattern = re.compile(r"DW_AT_stmt_list\s+\((0x[0-9a-f]*)\)")
  comp_dir_pattern = re.compile(r"DW_AT_comp_dir\s+\(\"([^\"]+)\"\)")

  map_stmt_list_to_comp_dir = {}
  iterator = compile_unit_pattern.finditer(text)
  current_match = next(iterator, None)

  while current_match:
    next_match = next(iterator, None)
    start = current_match.end()
    end = next_match.start() if next_match else len(text)

    stmt_list_match = stmt_list_pattern.search(text, start, end)
    if stmt_list_match is not None:
      stmt_list = stmt_list_match.group(1)
      comp_dir_match = comp_dir_pattern.search(text, start, end)
      comp_dir = comp_dir_match.group(1) if comp_dir_match is not None else ''
      map_stmt_list_to_comp_dir[stmt_list] = comp_dir

    current_match = next_match

  return map_stmt_list_to_comp_dir

def read_dwarf_info(wasm):
  logger.debug('Reading DWARF information from %s' % wasm)

  # We need only three tags in the debug info: DW_TAG_compile_unit for
  # source location, and DW_TAG_subprogram and DW_TAG_inlined_subroutine
  # for the function ranges.
  dwarfdump_cmd = ['./wasi-sdk/bin/llvm-dwarfdump', '-debug-info', '-debug-line', wasm, '--recurse-depth=0']

  proc = utils.run_process(dwarfdump_cmd, stdout=utils.PIPE)
  output = proc.stdout

  debug_line_pattern = re.compile(r"debug_line\[(0x[0-9a-f]*)\]")
  include_dir_pattern = re.compile(r"include_directories\[\s*(\d+)\] = \"([^\"]*)")
  file_pattern = re.compile(r"file_names\[\s*(\d+)\]:\s+name: \"([^\"]*)\"\s+dir_index: (\d+)")
  line_pattern = re.compile(r"\n0x([0-9a-f]+)\s+(\d+)\s+(\d+)\s+(\d+)(.*?end_sequence)?")

  entries = []
  iterator = debug_line_pattern.finditer(output)
  current_match = None
  try:
    current_match = next(iterator)
    debug_info_end = current_match.start() # end of .debug_info contents
  except StopIteration:
    debug_info_end = len(output)

  debug_info = output[:debug_info_end] # .debug_info contents
  map_stmt_list_to_comp_dir = extract_comp_dir_map(debug_info)

  while current_match:
    next_match = next(iterator, None)

    stmt_list = current_match.group(1)
    start = current_match.end()
    end = next_match.start() if next_match else len(output)

    comp_dir = map_stmt_list_to_comp_dir.get(stmt_list, '')

    # include_directories[  1] = "/Users/yury/Work/junk/sqlite-playground/src"
    # file_names[  1]:
    #            name: "playground.c"
    #       dir_index: 1
    #        mod_time: 0x00000000
    #          length: 0x00000000
    #
    # Address            Line   Column File   ISA Discriminator Flags
    # ------------------ ------ ------ ------ --- ------------- -------------
    # 0x0000000000000006     22      0      1   0             0  is_stmt
    # 0x0000000000000007     23     10      1   0             0  is_stmt prologue_end
    # 0x000000000000000f     23      3      1   0             0
    # 0x0000000000000010     23      3      1   0             0  end_sequence
    # 0x0000000000000011     28      0      1   0             0  is_stmt

    include_directories = {'0': comp_dir}
    for dir in include_dir_pattern.finditer(output, start, end):
      include_directories[dir.group(1)] = os.path.join(comp_dir, dir.group(2))

    files = {}
    for file in file_pattern.finditer(output, start, end):
      dir = include_directories[file.group(3)]
      file_path = os.path.join(dir, file.group(2))
      files[file.group(1)] = file_path

    for line in line_pattern.finditer(output, start, end):
      entry = {'address': int(line.group(1), 16), 'line': int(line.group(2)), 'column': int(line.group(3)), 'file': files[line.group(4)], 'eos': line.group(5) is not None}
      if not entry['eos']:
        entries.append(entry)
      else:
        # move end of function to the last END operator
        entry['address'] -= 1
        if entries[-1]['address'] == entry['address']:
          # last entry has the same address, reusing
          entries[-1]['eos'] = True
        else:
          entries.append(entry)

    current_match = next_match

  remove_dead_entries(entries)

  # return entries sorted by the address field
  entries = sorted(entries, key=lambda entry: entry['address'])

  return entries

def build_sourcemap(entries, code_section_offset, base_path):
  prefixes = SourceMapPrefixes(base_path)

  sources = []
  # There can be duplicate names in case an original source function has
  # multiple disjoint PC ranges or is inlined to multiple callsites. Make the
  # 'names' list a unique list of names, and map the function ranges to the
  # indices in that list.
  mappings = []
  sources_map = {}
  last_address = 0
  last_source_id = 0
  last_line = 1
  last_column = 1
  last_func_id = 0

  active_funcs = []
  next_func_range_id = 0

  for entry in entries:
    line = entry['line']
    column = entry['column']
    # ignore entries with line 0
    if line == 0:
      continue
    # start at least at column 1
    if column == 0:
      column = 1

    address = entry['address'] + code_section_offset
    file_name = utils.normalize_path(entry['file'])
    source_name = '../' + prefixes.sources.resolve(file_name)

    if source_name not in sources_map:
      source_id = len(sources)
      sources_map[source_name] = source_id
      sources.append(source_name)
    else:
      source_id = sources_map[source_name]

    address_delta = address - last_address
    source_id_delta = source_id - last_source_id
    line_delta = line - last_line
    column_delta = column - last_column
    last_address = address
    last_source_id = source_id
    last_line = line
    last_column = column
    mapping = encode_vlq(address_delta) + encode_vlq(source_id_delta) + encode_vlq(line_delta) + encode_vlq(column_delta)
    mappings.append(mapping)

  return {'version': 3,
          'sources': sources,
          'mappings': ','.join(mappings)}

def encode_uint_var(n):
  result = bytearray()
  while n > 127:
    result.append(128 | (n & 127))
    n = n >> 7
  result.append(n)
  return bytes(result)

def append_source_mapping(wasm, url):
  logger.debug('Append sourceMappingURL section')
  section_name = "sourceMappingURL"
  section_content = encode_uint_var(len(section_name)) + section_name.encode() + encode_uint_var(len(url)) + url.encode()
  return wasm + encode_uint_var(0) + encode_uint_var(len(section_content)) + section_content

def main(args):
  options = parse_args(args)

  wasm_input = options.wasm
  with open(wasm_input, 'rb') as infile:
    wasm = infile.read()
  entries = read_dwarf_info(wasm_input)

  code_section_offset = get_code_section_offset(wasm)

  map = build_sourcemap(entries, code_section_offset, options.basepath)
  with open(options.output, 'w', encoding='utf-8') as outfile:
    json.dump(map, outfile, separators=(',', ':'), ensure_ascii=False)

  map_url = os.path.basename(wasm_input) + '.map'
  wasm = append_source_mapping(wasm, map_url)
  with open(wasm_input, 'wb') as outfile:
      outfile.write(wasm)

if __name__ == '__main__':
  main(sys.argv[1:])
