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

class TableRef {
public:
    TableRef(externref_t ref);
    ~TableRef();
    TableRef(const TableRef&) = delete;
    TableRef(TableRef&&) = delete;
    TableRef& operator=(const TableRef&) = delete;
    TableRef& operator=(TableRef&&) = delete;

    externref_t operator*();
private:
    size_t index;
};

int alloc_ref();
void set_ref(int index, externref_t r);
externref_t get_ref();

}
