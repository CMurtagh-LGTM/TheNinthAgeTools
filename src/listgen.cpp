#include "wasm.hpp"

#include "dom.hpp"

#include <cstring>
#include <format>
#include <string>
#include <toml++/toml.hpp>

#include <ranges>

struct entry_t {
  std::string_view name;
  std::string_view key;
  bool inches;
};

constexpr std::array<const entry_t, 3> global_entry_descriptions = {{
    {"Cha", "cha", true},
    {"Mob", "mob", true},
    {"Dis", "dis", false},
}};

constexpr std::array<const entry_t, 4> defensive_entry_descriptions = {{
    {"HP", "hp", false},
    {"Def", "def", false},
    {"Res", "res", false},
    {"Arm", "arm", false},
}};


dom::node_t add_popover(dom::node_t node) {
  auto popover = dom::append_div_child(node);
  dom::set_popover(popover);
  dom::add_class(popover, "popover");
  dom::set_popover_target(node, popover);
  return popover;
}

void add_text_with_popovers(const toml::table &model_rules, dom::node_t node,
                            const std::string_view text) {
  size_t start_pos = 0;
  bool inside = false;
  while (start_pos != std::string_view::npos) {
    size_t end_pos = text.find(inside ? "}}" : "{{", start_pos);

    std::string_view new_text =
        (end_pos == std::string_view::npos)
            ? text.substr(start_pos)
            : text.substr(start_pos, end_pos - start_pos);

    dom::node_t new_node = inside ? dom::append_button_child(node) : dom::append_span_child(node);

    dom::set_text(new_node, new_text);

    if (inside) {
      auto popover = add_popover(new_node);
      add_text_with_popovers(model_rules, popover,
                            model_rules[new_text].value_or(""));

    }

    if(end_pos == std::string_view::npos) {
      break;
    }

    start_pos = end_pos + 2;
    inside = !inside;
  }
}

void add_table(const toml::table &model_rules, const toml::table &book,
               const std::string_view name) {
  auto content = dom::get_by_id("content");
  auto book_entry = book[name];

  auto table = dom::append_table_child(content);
  auto global_header = dom::append_tr_child(table);
  auto global_data = dom::append_tr_child(table);

  for (const auto &entry_description : global_entry_descriptions) {
    auto header = dom::append_td_child(global_header);
    dom::set_text(header, entry_description.name);
    auto data = dom::append_td_child(global_data);
    int value = book_entry[entry_description.key].value_or(0);
    if (entry_description.inches) {
      dom::set_text(data, std::format("{}\"", value));
    } else {
      dom::set_text(data, std::format("{}", value));
    }
  }

  dom::append_td_child(global_header);
  dom::append_td_child(global_data);

  {
    auto header = dom::append_td_child(global_header);
    dom::set_text(header, "Model Rules");
    auto data = dom::append_td_child(global_data);

    if (const toml::array *arr = book_entry["model_rules"].as_array()) {
      bool first = true;
      for (auto &&element : *arr) {
        if (!first) {
          auto span = dom::append_span_child(data);
          dom::set_text(span, ", ");
        } else {
          first = false;
        }
        auto rule = element.value_or("-");

        if (model_rules[rule]) {
          auto button = dom::append_button_child(data);
          dom::set_text(button, rule);

          auto popover = add_popover(button);
          add_text_with_popovers(model_rules, popover,
                                 model_rules[rule].value_or(""));
        } else {
          auto span = dom::append_span_child(data);
          dom::set_text(span, rule);
        }
      }
    }
  }

  auto defensive_header = dom::append_tr_child(table);
  auto defensive_data = dom::append_tr_child(table);

  for (const auto &entry_description : defensive_entry_descriptions) {
    auto header = dom::append_td_child(defensive_header);
    dom::set_text(header, entry_description.name);
    auto data = dom::append_td_child(defensive_data);
    int value = book_entry[entry_description.key].value_or(0);
    if (entry_description.inches) {
      dom::set_text(data, std::format("{}\"", value));
    } else {
      dom::set_text(data, std::format("{}", value));
    }
  }
}

int main() {
  using namespace std::string_view_literals;
  static constexpr auto source = R"(
    [marshal]
    cha = 4
    mob = 4
    dis = 8
    model_rules = ["Disciplined"]
    hp = 3
    def = 5
    res = 4
    arm = 3
  )"sv;
  static constexpr auto model_rules_source = R"(
    Disciplined = "Command Tests taken by a unit containing Disciplined gain {{Minimized}}."
    Minimized = "For each instance of Minimised, add a dice to the roll, and discard the dice that rolled the highest result."
  )"sv;
  auto book = toml::parse(source);
  auto model_rules = toml::parse(model_rules_source);
  if (!book) {
    wasm::log("failed to parse toml book");
    return 0;
  }
  add_table(model_rules, book, "marshal");
}
