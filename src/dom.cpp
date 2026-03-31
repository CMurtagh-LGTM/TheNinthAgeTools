#include "dom.hpp"

#include "input.hpp"

namespace dom {

// node_t get_by_id(const std::string_view string) {
//   return internal::get_by_id(string.data(), string.length());
// }

// void set_text(node_t node, const std::string_view string) {
//   internal::set_text(node, string.data(), string.length());
// }

// void set_id(node_t node, const std::string_view id) {
//   internal::set_id(node, id.data(), id.length());
// }

// void add_class(node_t node, const std::string_view class_name) {
//   internal::add_class(node, class_name.data(), class_name.length());
// }

// void set_visible(node_t node, bool visible) {
//   internal::set_visible(node, visible);
// }

// node_t append_img_child(node_t node_parent) {
//   static constexpr std::string name = "img";
//   return internal::append_child(node_parent, name.data(), name.length());
// }

// void set_img_src(node_t img, const std::string_view src) {
//   internal::set_img_src(img, src.data(), src.length());
// }

// node_t append_table_child(node_t node_parent) {
//   static constexpr std::string name = "table";
//   return internal::append_child(node_parent, name.data(), name.length());
// }

// node_t append_tr_child(node_t node_parent) {
//   static constexpr std::string name = "tr";
//   return internal::append_child(node_parent, name.data(), name.length());
// }

// node_t append_td_child(node_t node_parent) {
//   static constexpr std::string name = "td";
//   return internal::append_child(node_parent, name.data(), name.length());
// }

// node_t append_div_child(node_t node_parent) {
//   static constexpr std::string name = "div";
//   return internal::append_child(node_parent, name.data(), name.length());
// }

// node_t append_span_child(node_t node_parent) {
//   static constexpr std::string name = "span";
//   return internal::append_child(node_parent, name.data(), name.length());
// }

// node_t append_button_child(node_t node_parent) {
//   static constexpr std::string name = "button";
//   return internal::append_child(node_parent, name.data(), name.length());
// }

// void set_popover(node_t node) {
//   internal::set_popover(node);
// }

// void set_popover_target(node_t node, node_t popover) {
//   internal::set_popover_target(node, popover);
// }

// node_t append_popover_child(node_t node) {
//   node_t popover = append_div_child(node);
//   set_popover(popover);
//   add_class(popover, "popover");
//   set_popover_target(node, popover);
//   return popover;
// }

// accordion_t append_accordion_child(node_t node) {
//   node_t button = append_div_child(node);
//   node_t div = append_div_child(node);
//   set_visible(div, false);

//   wasm::TableRef div_ref = wasm::TableRef(div);

//   input::add_click_listener(button, [div_ = std::move(div_ref), hidden = true]() mutable {
//     set_visible(*div_, !hidden);
//     hidden = !hidden;
//   });

//   return {wasm::TableRef(button), wasm::TableRef(div)};
// }

}
