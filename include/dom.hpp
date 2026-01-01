#pragma once

#include "wasm.hpp"

#include <string>

namespace dom {

typedef externref_t node_t;

typedef node_t img_t;

namespace internal {
WASM_IMPORT_MODULE("dom", "get_by_id")
extern node_t get_by_id(const char* string, int length);
WASM_IMPORT_MODULE("dom", "append_child")
extern node_t append_child(node_t node_parent, const char* string, int length);

WASM_IMPORT_MODULE("dom", "set_img_src")
extern void set_img_src(img_t img, const char* string, int length);
}

node_t get_by_id(const std::string& string);

img_t append_img_child(node_t node_parent);
void set_img_src(img_t img, const std::string& src);

}
