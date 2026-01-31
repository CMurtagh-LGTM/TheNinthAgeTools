#include "wasm.hpp"

#include <cassert>
#include <string>

static externref_t ref_table[0];
static size_t next_free = 0;
static std::vector<bool> used_elements = {false};

namespace wasm {

namespace {
// Avoid the std::to_string as that has stdio calls
std::string int_to_string(int number) {
  std::string string;

  if (number < 0) {
    number = -number;
    string = "-";
  }

  while (number != 0) {
    string += (number % 10) + 0x30;
    number /= 10;
  }

  return string;
}
}

void log(const char* string, size_t length) {
  internal::log(string, length);
}
void log(const std::string_view string) {
  wasm::log(string.data(), string.length());
}
void log(int number){
  wasm::log(int_to_string(number));
}


TableRef::TableRef(): index(NULL_REF) {
}

TableRef::TableRef(externref_t externref) {
  index = next_free;
  assert(!used_elements[index]);
  used_elements[index] = true;
  do {
    next_free++;
    if (used_elements.size() < next_free) {
      used_elements.push_back(false);
      break;
    }
  } while(used_elements[next_free]);

  if ( __builtin_wasm_table_size(ref_table) <= index ) {
    __builtin_wasm_table_grow(ref_table, externref, index + 1);
  }
    
  __builtin_wasm_table_set(ref_table, index, externref);
}


TableRef::TableRef(TableRef&& other) : index(other.index) {
  other.index = NULL_REF;
}

TableRef& TableRef::operator=(TableRef&& other) {
  index = other.index;
  other.index = NULL_REF;
  return *this;
}

TableRef::~TableRef() {
  if (index == NULL_REF) {
    return;
  }
  used_elements[index] = false;
  next_free = std::min(next_free, index);
}

externref_t TableRef::operator*() {
  assert(index != NULL_REF);
  externref_t retval = __builtin_wasm_table_get(ref_table, index);

  return retval;
}

TableRef::operator bool() const {
  return index != NULL_REF;
}

}
