#pragma once

#include "wasm.hpp"

#include <string_view>

namespace dom {

using node_t = externref_t;

namespace internal {
WASM_IMPORT_MODULE("dom", "get_by_id")
extern node_t get_by_id(const char* string, int length);
WASM_IMPORT_MODULE("dom", "set_text")
extern void set_text(node_t node, const char* string, int length);
WASM_IMPORT_MODULE("dom", "append_child")
extern node_t append_child(node_t node_parent, const char* string, int length);

WASM_IMPORT_MODULE("dom", "set_img_src")
extern void set_img_src(node_t img, const char* string, int length);
}

node_t get_by_id(const std::string_view string);
void set_text(node_t node, const std::string_view string);

node_t append_img_child(node_t node_parent);
void set_img_src(node_t img, const std::string_view src);

node_t append_table_child(node_t node_parent);
node_t append_tr_child(node_t node_parent);
node_t append_td_child(node_t node_parent);

}
