#pragma once

#include <string>
#include <vector>

#define WASM_IMPORT(name) __attribute__((import_module("env"), import_name(name)))
#define WASM_IMPORT_MODULE(module, name) __attribute__((import_module(module), import_name(name)))
#define WASM_EXPORT(name) __attribute__((export_name(name)))

using externref_t = __externref_t;

namespace wasm {

namespace internal {
WASM_IMPORT("log")
extern void log(const char* string, size_t length);
}

void log(const char* string, size_t length);
void log(const std::string_view string);
void log(int number);

}
