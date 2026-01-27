#include "wasm.hpp"

#include "dom.hpp"

#include <cstring>
#include <string>
#include <format>
#include <toml++/toml.hpp>


#include <ranges>

struct entry_t {
  std::string_view name;
  std::string_view key;
  bool inches;
};

constexpr std::array<const entry_t, 3> entry_descriptions = {{
  {"Cha", "cha", true},
  {"Mob", "mob", true},
  {"Dis", "dis", false},
}};

void add_table(const toml::table& model_rules, const toml::table& book, const std::string_view name) {
  auto content = dom::get_by_id("content");
  auto book_entry = book[name];

  auto table = dom::append_table_child(content);
  auto global_header = dom::append_tr_child(table);
  auto global_data = dom::append_tr_child(table);

  for (const auto& entry_description: entry_descriptions) {
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

  {
    auto header = dom::append_td_child(global_header);
    dom::set_text(header, "Model Rules");
    auto data = dom::append_td_child(global_data);

    auto rules = book_entry["rules"];
    if (const toml::array* arr = rules.as_array()) {
      bool first = true;
      for (auto&& element: *arr) {
        if(!first) {
          auto span = dom::append_span_child(data);
          dom::set_text(span, ", ");
        } else {
          first = false;
        }
        auto rule = element.value_or("-");

        if(model_rules[rule]) {
          auto button = dom::append_button_child(data);
          dom::set_text(button, rule);

          auto popover = dom::append_div_child(data);
          dom::set_popover(popover);
          dom::add_class(popover, "popover");
          dom::set_text(popover, model_rules[rule].value_or(""));

          dom::set_popover_target(button, popover);
        } else {
          auto span = dom::append_span_child(data);
          dom::set_text(span, rule);
        }
      }
    }
  }
}


int main() {
  // auto content = dom::get_by_id("content");
  // auto img = dom::append_img_child(content);
  // dom::set_img_src(img, "./pics/logo_character.png");


  using namespace std::string_view_literals;
  static constexpr auto source = R"(
    [marshal]
    cha = 4
    mob = 4
    dis = 8
    rules = ["Disciplined", "test"]
  )"sv;
  static constexpr auto model_rules_source = R"(
    Disciplined = "Command Tests taken by a unit containing Disciplined gain Minimized."
  )"sv;
  auto book = toml::parse(source);
  auto model_rules = toml::parse(model_rules_source);
  if (!book) {
    wasm::log("failed to parse toml book");
    return 0;
  }
  add_table(model_rules, book, "marshal");
}

