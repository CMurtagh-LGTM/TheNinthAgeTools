#include "table.hpp"

#include <format>
#include <string_view>
#include <toml++/toml.hpp>

#include "dom.hpp"
#include "reactive.hpp"
#include "unit.hpp"

namespace table {

template <typename Target>
struct entry_t {
  std::string_view name;
  std::string_view key;
  reactive::Reactive Target::* reactive_ptr;
};

template <typename Target>
struct entries_t {
  std::string name;
  std::array<const entry_t<Target>, 6> entries;
};

constexpr entries_t<Unit> global_entry_descriptions = {
    "Global",
    {{
        {"Cha", "cha", &Unit::cha},
        {"Mob", "mob", &Unit::mob},
        {"Dis", "dis", &Unit::dis},
        {"", "", nullptr},
        {"", "", nullptr},
        {"Model Rules", "model_rules", &Unit::model_rules},
    }}};

constexpr entries_t<Unit> defensive_entry_descriptions = {
    "Defensive",
    {{
        {"HP", "hp", &Unit::hp},
        {"Def", "def", &Unit::def},
        {"Res", "res", &Unit::res},
        {"Arm", "arm", &Unit::arm},
        {"", "", nullptr},
        {"", "", nullptr},
    }}};

constexpr entries_t<Offense> offensive_entry_descriptions = {
    "Offensive",
    {{
        {"Att", "att", &Offense::att},
        {"Off", "off", &Offense::off},
        {"Str", "str", &Offense::str},
        {"AP", "ap", &Offense::ap},
        {"Agi", "agi", &Offense::agi},
        {"Attack Attributes, Weapons and Properties", "offensive_rules",
         &Offense::offensive_rules},
    }}};

template <typename Target>
void add_header(const entries_t<Target> entry_descriptions, const dom::node_t table) {
  auto header = dom::append_tr_child(table);
  dom::add_class(header, "header_tr");

  {
    auto header_td = dom::append_td_child(header);
    dom::set_text(header_td, entry_descriptions.name);
  }

  for (const auto& entry_description : entry_descriptions.entries) {
    auto header_td = dom::append_td_child(header);
    dom::set_text(header_td, entry_description.name);
  }
}

template <typename Target>
void add_row(Target& unit, const toml::table& rules,
             const toml::node_view<const toml::node>& book_entry,
             const entries_t<Target> entry_descriptions, dom::node_t table,
             const std::string_view row_name) {
  auto data = dom::append_tr_child(table);

  {
    auto data_td = dom::append_td_child(data);
    dom::set_text(data_td, row_name);
  }

  for (const auto& entry_description : entry_descriptions.entries) {
    auto data_td = dom::append_td_child(data);

    if (entry_description.reactive_ptr == nullptr) {
      continue;
    }
    auto node_ref = wasm::TableRef(data_td);
    std::visit(
        [&](auto&& arg) {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, reactive::Number> ||
                        std::is_same_v<T, reactive::Distance>) {
            dom::add_class(*node_ref, "td_number");
            arg.set(std::move(node_ref),
                    book_entry[entry_description.key].value_or(0));
          } else if constexpr (std::is_same_v<T, reactive::Rules>) {
            arg.set(std::move(node_ref), rules,
                    book_entry[entry_description.key].as_array());
          } else {
            static_assert(false);
          }
        },
        unit.*(entry_description.reactive_ptr));
  }
}

void add_table(Unit& unit, const toml::table& rules,
               const toml::node_view<const toml::node>& book_entry,
               dom::node_t node) {
  // TODO use css grid instead
  auto table = dom::append_table_child(node);

  add_header(global_entry_descriptions, table);
  add_row(unit, rules, book_entry, global_entry_descriptions, table, "");
  add_header(defensive_entry_descriptions, table);
  add_row(unit, rules, book_entry, defensive_entry_descriptions, table, "");

  add_header(offensive_entry_descriptions, table);
  if (const toml::array* offensive_array = book_entry["offensive"].as_array()) {
    for (auto&& offensive_entry : *offensive_array) {
      const toml::node_view<const toml::node>& view =
          static_cast<const toml::node_view<const toml::node>>(offensive_entry);
      unit.offenses.emplace_back();
      add_row(unit.offenses.back(), rules, view, offensive_entry_descriptions, table,
              view["name"].value_or(""));
    }
  }
}

void add_unit_header(const toml::node_view<const toml::node>& book_entry,
                     dom::node_t node, const std::string_view name) {
  auto header = dom::append_div_child(node);
  dom::add_class(header, "header_grid");

  auto img = dom::append_img_child(header);
  dom::add_class(img, "unit_icon");
  dom::set_img_src(img, "./pics/logo_character.png");

  auto name_div = dom::append_div_child(header);
  dom::add_class(name_div, "unit_name");
  dom::set_text(name_div, name);
  auto base_div = dom::append_div_child(header);
  dom::set_text(base_div,
                std::format("Base: {} mm", book_entry["base"].value_or("0")));
  auto points_div = dom::append_div_child(header);
  dom::set_text(points_div,
                std::format("{} pts", book_entry["points"].value_or(0)));
  auto models_div = dom::append_div_child(header);
  dom::set_text(models_div, std::format("{} models",
                                        book_entry["model_count"].value_or(0)));
  auto rarity_div = dom::append_div_child(header);
  dom::set_text(rarity_div,
                std::format("Rarity {}", book_entry["rarity"].value_or("0")));
  auto height_div = dom::append_div_child(header);
  dom::set_text(height_div,
                std::format("Height {}", book_entry["height"].value_or(0)));
}

Unit add_unit(const toml::table& rules, const toml::table& book,
              const std::string_view name) {
  Unit unit;
  auto content = dom::get_by_id("content");
  auto book_entry = book[name];

  auto div = dom::append_div_child(content);

  add_unit_header(book_entry, div, name);
  add_table(unit, rules, book_entry, div);

  return unit;
}

}  // namespace table
