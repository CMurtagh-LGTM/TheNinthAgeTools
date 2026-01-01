#include "wasm.hpp"

#include <string>

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
void log(const std::string& string) {
  wasm::log(string.data(), string.length());
}
void log(const std::string_view string) {
  wasm::log(string.data(), string.length());
}
void log(int number){
  wasm::log(int_to_string(number));
}

}
