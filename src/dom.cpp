#include "dom.hpp"

namespace dom {

node_t get_by_id(const std::string_view string) {
  return internal::get_by_id(string.data(), string.length());
}

void set_text(node_t node, const std::string_view string) {
  internal::set_text(node, string.data(), string.length());
}

void set_id(node_t node, const std::string_view id) {
  internal::set_id(node, id.data(), id.length());
}

void add_class(node_t node, const std::string_view class_name) {
  internal::add_class(node, class_name.data(), class_name.length());
}

node_t append_img_child(node_t node_parent) {
  static constexpr std::string name = "img";
  return internal::append_child(node_parent, name.data(), name.length());
}

void set_img_src(node_t img, const std::string_view src) {
  internal::set_img_src(img, src.data(), src.length());
}

node_t append_table_child(node_t node_parent) {
  static constexpr std::string name = "table";
  return internal::append_child(node_parent, name.data(), name.length());
}

node_t append_tr_child(node_t node_parent) {
  static constexpr std::string name = "tr";
  return internal::append_child(node_parent, name.data(), name.length());
}

node_t append_td_child(node_t node_parent) {
  static constexpr std::string name = "td";
  return internal::append_child(node_parent, name.data(), name.length());
}

node_t append_div_child(node_t node_parent) {
  static constexpr std::string name = "div";
  return internal::append_child(node_parent, name.data(), name.length());
}

node_t append_span_child(node_t node_parent) {
  static constexpr std::string name = "span";
  return internal::append_child(node_parent, name.data(), name.length());
}

node_t append_button_child(node_t node_parent) {
  static constexpr std::string name = "button";
  return internal::append_child(node_parent, name.data(), name.length());
}

void set_popover(node_t node) {
  internal::set_popover(node);
}

void set_popover_target(node_t node, node_t popover) {
  internal::set_popover_target(node, popover);
}

dom::node_t append_popover_child(dom::node_t node) {
  auto popover = dom::append_div_child(node);
  dom::set_popover(popover);
  dom::add_class(popover, "popover");
  dom::set_popover_target(node, popover);
  return popover;
}

}
