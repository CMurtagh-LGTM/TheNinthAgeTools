#include "wasm.hpp"

#include "dom.hpp"

#include <cstring>
#include <string>
#include <toml++/toml.hpp>

constexpr const int SIZE = 1024;
int buffer[SIZE];

int used = 0;

WASM_EXPORT("get_offset")
int* get_offset() {
  return buffer;
}

WASM_EXPORT("get_used")
int get_used() {
  return used;
}

int main() {

  auto content = dom::get_by_id("content");
  auto img = dom::append_img_child(content);
  dom::set_img_src(img, "./pics/logo_character.png");

  using namespace std::string_view_literals;
  static constexpr auto source = R"(
      str = "hello world"

      numbers = [ 1, 2, 3, "four", 5.0 ]
      vegetables = [ "tomato", "onion", "mushroom", "lettuce" ]
      minerals = [ "quartz", "iron", "copper", "diamond" ]

      [animals]
      cats = [ "tiger", "lion", "puma" ]
      birds = [ "macaw", "pigeon", "canary" ]
      fish = [ "salmon", "trout", "carp" ]
  )"sv;
  auto tbl = toml::parse(source);

  std::string_view str = tbl["str"].value_or(""sv);
  wasm::log(str);
}

