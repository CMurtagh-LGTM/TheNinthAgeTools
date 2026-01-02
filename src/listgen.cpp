#include "wasm.hpp"

#include "dom.hpp"

#include <cstring>
#include <string>
#include <format>
#include <toml++/toml.hpp>

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

void add_table(const toml::table& book, const std::string_view name) {
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
  )"sv;
  auto book = toml::parse(source);
  if (!book) {
    wasm::log("failed to parse toml book");
    return 0;
  }
  add_table(book, "marshal");
}

