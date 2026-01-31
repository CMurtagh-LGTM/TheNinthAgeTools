#include "reactive.hpp"

#include <format>
#include "dom.hpp"

namespace reactive {

Number::Number(dom::node_t node, int number): n(number), m_node(node) {
  dom::set_text(*m_node, std::format("{}", n));
}

void Number::set(int number) {
  n = number;
  dom::set_text(*m_node, std::format("{}", n));
}

}
