#include "dom.hpp"

namespace dom {

dom_t get_by_id(const std::string& string) {
  return internal::get_by_id(string.data(), string.length());
}

}
