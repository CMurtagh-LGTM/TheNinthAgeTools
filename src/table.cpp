#include "table.hpp"

#include <format>
#include <string_view>
#include <toml++/toml.hpp>

#include "dom.hpp"

namespace table {

enum class entry_kind_e { EMPTY, NORMAL, DISTANCE, RULES };

struct entry_t {
  std::string_view name;
  std::string_view key;
  entry_kind_e entry_kind;
};

struct entries_t {
  std::string name;
  std::array<const entry_t, 6> entries;
};

constexpr entries_t global_entry_descriptions = {
    "Global",
    {{
        {"Cha", "cha", entry_kind_e::DISTANCE},
        {"Mob", "mob", entry_kind_e::DISTANCE},
        {"Dis", "dis", entry_kind_e::NORMAL},
        {"", "", entry_kind_e::EMPTY},
        {"", "", entry_kind_e::EMPTY},
        {"Model Rules", "model_rules", entry_kind_e::RULES},
    }}};

constexpr entries_t defensive_entry_descriptions = {
    "Defensive",
    {{
        {"HP", "hp", entry_kind_e::NORMAL},
        {"Def", "def", entry_kind_e::NORMAL},
        {"Res", "res", entry_kind_e::NORMAL},
        {"Arm", "arm", entry_kind_e::NORMAL},
        {"", "", entry_kind_e::EMPTY},
        {"", "", entry_kind_e::EMPTY},
    }}};

constexpr entries_t offensive_entry_descriptions = {
    "Offensive",
    {{
        {"Att", "att", entry_kind_e::NORMAL},
        {"Off", "off", entry_kind_e::NORMAL},
        {"Str", "str", entry_kind_e::NORMAL},
        {"AP", "ap", entry_kind_e::NORMAL},
        {"Agi", "agi", entry_kind_e::NORMAL},
        {"Attack Attributes, Weapons and Properties", "offensive_rules",
         entry_kind_e::RULES},
    }}};

dom::node_t add_popover(dom::node_t node) {
  auto popover = dom::append_div_child(node);
  dom::set_popover(popover);
  dom::add_class(popover, "popover");
  dom::set_popover_target(node, popover);
  return popover;
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
      auto popover = add_popover(new_node);
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

void add_normal_entry(const toml::node_view<const toml::node>& book_entry,
                      const std::string_view key, dom::node_t data) {
  int value = book_entry[key].value_or(0);
  dom::set_text(data, std::format("{}", value));
}

void add_distance_entry(const toml::node_view<const toml::node>& book_entry,
                        const std::string_view key, dom::node_t data) {
  int value = book_entry[key].value_or(0);
  dom::set_text(data, std::format("{}\"", value));
}

void add_rules_entry(const toml::table& model_rules,
                     const toml::node_view<const toml::node>& book_entry,
                     const std::string_view key, dom::node_t data) {
  if (const toml::array* arr = book_entry[key].as_array()) {
    bool first = true;
    for (auto&& element : *arr) {
      if (!first) {
        auto span = dom::append_span_child(data);
        dom::set_text(span, ", ");
      } else {
        first = false;
      }
      auto rule = element.value_or("-");

      if (model_rules[rule]) {
        auto button = dom::append_button_child(data);
        dom::add_class(button, "model_rule_button");
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

void add_row(const toml::table& model_rules,
             const toml::node_view<const toml::node>& book_entry,
             const entries_t entry_descriptions, dom::node_t table,
             const std::string_view row_name) {
  auto header = dom::append_tr_child(table);
  dom::add_class(header, "header_tr");
  auto data = dom::append_tr_child(table);

  {
    auto header_td = dom::append_td_child(header);
    dom::set_text(header_td, entry_descriptions.name);
    auto data_td = dom::append_td_child(data);
    dom::set_text(data_td, row_name);
  }

  for (const auto& entry_description : entry_descriptions.entries) {
    auto header_td = dom::append_td_child(header);
    dom::set_text(header_td, entry_description.name);
    auto data_td = dom::append_td_child(data);
    switch (entry_description.entry_kind) {
      case entry_kind_e::EMPTY:
        break;
      case entry_kind_e::NORMAL:
        add_normal_entry(book_entry, entry_description.key, data_td);
        break;
      case entry_kind_e::DISTANCE:
        add_distance_entry(book_entry, entry_description.key, data_td);
        break;
      case entry_kind_e::RULES:
        add_rules_entry(model_rules, book_entry, entry_description.key, data_td);
        break;
    }
  }
}

void add_table(const toml::table& model_rules, const toml::table& book,
               const std::string_view name) {
  auto content = dom::get_by_id("content");
  auto book_entry = book[name];

  auto table = dom::append_table_child(content);

  add_row(model_rules, book_entry, global_entry_descriptions, table, "");
  add_row(model_rules, book_entry, defensive_entry_descriptions, table, "");

  if(const toml::array* offensive_array = book_entry["offensive"].as_array()){
    for(auto&& offensive_entry: *offensive_array){
      const toml::node_view<const toml::node>& view = static_cast<const toml::node_view<const toml::node>>(offensive_entry);
      add_row(model_rules, view, offensive_entry_descriptions, table, view["name"].value_or(""));
    }
  }
}

}  // namespace table
