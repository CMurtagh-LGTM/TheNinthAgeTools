#include "reactive.hpp"

#include <format>
#include "dom.hpp"

namespace reactive {

void Number::set(int number) {
  n = number;
  dom::set_text(*m_node, std::format(format, n));
}

void Number::set(wasm::TableRef&& node, int number) {
  m_node = std::move(node);
  set(number);
}

void Distance::set(int number) {
  n = number;
  dom::set_text(*m_node, std::format(format, n));
}

void Distance::set(wasm::TableRef&& node, int number) {
  m_node = std::move(node);
  set(number);
}

void add_text_with_popovers(const toml::table& model_rules, dom::node_t node,
                            const std::string_view text) {
  size_t start_pos = 0;
  bool inside = false;
  while (start_pos != std::string_view::npos) {
    size_t end_pos = text.find(inside ? "}}" : "{{", start_pos);

    std::string_view new_text =
        (end_pos == std::string_view::npos)
            ? text.substr(start_pos)
            : text.substr(start_pos, end_pos - start_pos);

    dom::node_t new_node =
        inside ? dom::append_button_child(node) : dom::append_span_child(node);

    dom::set_text(new_node, new_text);

    if (inside) {
      dom::add_class(new_node, "model_rule_button");
      auto popover = dom::append_popover_child(new_node);
      add_text_with_popovers(model_rules, popover,
                             model_rules[new_text].value_or(""));
    }

    if (end_pos == std::string_view::npos) {
      break;
    }

    start_pos = end_pos + 2;
    inside = !inside;
  }
}

void add_rules_entry(const toml::table& rules,
                     const toml::array* array_ptr,
                     dom::node_t data) {
  if (array_ptr) {
    bool first = true;
    for (auto&& element : *array_ptr) {
      if (!first) {
        auto span = dom::append_span_child(data);
        dom::set_text(span, ", ");
      } else {
        first = false;
      }
      auto rule_name = element.value_or("-");

      if (rules[rule_name]) {
        auto button = dom::append_button_child(data);
        dom::add_class(button, "model_rule_button");
        dom::set_text(button, rule_name);

        auto popover = dom::append_popover_child(button);
        add_text_with_popovers(rules, popover, rules[rule_name].value_or(""));
      } else {
        auto span = dom::append_span_child(data);
        dom::set_text(span, rule_name);
      }
    }
  }
}

void Rules::set(wasm::TableRef&& node, const toml::table& rules, const toml::array* array_ptr) {
  add_rules_entry(rules, array_ptr, *node);
  m_node = std::move(node);
}

}
