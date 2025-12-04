#pragma once

#include "traits.h"

// Borrowed from llvm libc

using size_t = unsigned long;

template <typename U, convertible_to<U> T>
void memcpy(T* dst, const U* src, unsigned int count) {
  for (size_t offset = 0; offset < count; ++offset) {
    dst[offset] = src[offset];
  }
}

size_t strlen(const char* string);

