#include "string.h"

// Borrowed from llvm libc

size_t strlen(const char* src) {
  size_t length;
  for (length = 0; *src; ++src, ++length)
    ;
  return length;
}
