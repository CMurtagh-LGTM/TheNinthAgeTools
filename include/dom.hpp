#pragma once

#include "wasm.hpp"

#include <string>

typedef externref_t dom_t;

namespace internal {
WASM_IMPORT_MODULE("dom", "get_by_id")
extern dom_t get_by_id(const char* string, int length);
}

namespace dom {

dom_t get_by_id(const std::string& string);


}
