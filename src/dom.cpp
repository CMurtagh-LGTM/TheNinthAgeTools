#include "dom.hpp"

namespace dom {

node_t get_by_id(const std::string_view string) {
  return internal::get_by_id(string.data(), string.length());
}

void set_text(node_t node, const std::string_view string) {
  internal::set_text(node, string.data(), string.length());
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


}
