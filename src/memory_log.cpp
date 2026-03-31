#include "memory_log.hpp"

// #include "wasm.hpp"

// // no inline, required by [replacement.functions]/3
// void* operator new(std::size_t sz) {
//   if (sz == 0)
//     ++sz;  // avoid std::malloc(0) which may return nullptr on success

//   void* ptr = std::malloc(sz);
//   wasm::fmt_log("{:x}", (int)ptr);
//   if (ptr == nullptr) std::__throw_bad_alloc();
//   return ptr;
//   // throw std::bad_alloc{}; // required by [new.delete.single]/3
// }

// // no inline, required by [replacement.functions]/3
// void* operator new[](std::size_t sz) {
//   if (sz == 0)
//     ++sz;  // avoid std::malloc(0) which may return nullptr on success

//   void* ptr = std::malloc(sz);
//   wasm::fmt_log("{:x}", (int)ptr);
//   if (ptr == nullptr) std::__throw_bad_alloc();
//   return ptr;

//   // throw std::bad_alloc{}; // required by [new.delete.single]/3
// }
