#include "dom.hpp"

namespace dom {

node_t get_by_id(const std::string& string) {
  return internal::get_by_id(string.data(), string.length());
}

img_t append_img_child(node_t node_parent) {
  static constexpr std::string name = "img";
  return internal::append_child(node_parent, name.data(), name.length());
}

void set_img_src(img_t img, const std::string& src) {
  internal::set_img_src(img, src.data(), src.length());
}

}
