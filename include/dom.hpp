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
WASM_IMPORT_MODULE("dom", "set_id")
extern void set_id(node_t node, const char* string, int length);
WASM_IMPORT_MODULE("dom", "add_class")
extern void add_class(node_t node, const char* string, int length);

WASM_IMPORT_MODULE("dom", "append_child")
extern node_t append_child(node_t node_parent, const char* string, int length);

WASM_IMPORT_MODULE("dom", "set_img_src")
extern void set_img_src(node_t img, const char* string, int length);

WASM_IMPORT_MODULE("dom", "set_popover")
extern void set_popover(node_t node);
WASM_IMPORT_MODULE("dom", "set_popover_target")
extern void set_popover_target(node_t node, node_t popover);
}

node_t get_by_id(const std::string_view string);
void set_text(node_t node, const std::string_view string);
void set_id(node_t node, const std::string_view id);
void add_class(node_t node, const std::string_view class_name);

node_t append_img_child(node_t node_parent);
void set_img_src(node_t img, const std::string_view src);

node_t append_table_child(node_t node_parent);
node_t append_tr_child(node_t node_parent);
node_t append_td_child(node_t node_parent);

node_t append_div_child(node_t node_parent);
node_t append_span_child(node_t node_parent);

node_t append_button_child(node_t node_parent);

dom::node_t append_popover_child(dom::node_t node);

}
