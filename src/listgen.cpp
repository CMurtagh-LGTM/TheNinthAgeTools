#include <cstring>

#include <toml++/toml.hpp>

extern "C" {
__attribute__((import_name("log")))
extern void js_log(int);
}

constexpr const int SIZE = 1024;
int buffer[SIZE];

int used = 0;

__attribute__((export_name("get_offset")))
int* get_offset() {
  return buffer;
}

__attribute__((export_name("get_used")))
int get_used() {
  return used;
}

int main() {
  const char* string = "<img src=\"./pics/logo_character.png\"/>";
  const int string_length = strlen(string);
  memcpy(buffer, string, string_length);
  used = string_length;

  js_log(reinterpret_cast<uintptr_t>(buffer));
  js_log(used);
}
