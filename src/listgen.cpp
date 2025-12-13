#include "wasm.hpp"

#include "dom.hpp"

#include <cstring>
#include <string>
#include <toml++/toml.hpp>

constexpr const int SIZE = 1024;
int buffer[SIZE];

int used = 0;

WASM_EXPORT("get_offset")
int* get_offset() {
  return buffer;
}

WASM_EXPORT("get_used")
int get_used() {
  return used;
}

int main() {
  const char* string = "<img src=\"./pics/logo_character.png\"/>";
  const size_t string_length = strlen(string);
  memcpy(buffer, string, string_length);
  used = string_length;

  wasm::log(reinterpret_cast<const char *>(buffer), string_length);

  auto c = dom::get_by_id("content");
}

