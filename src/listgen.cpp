#include "wasm.hpp"

#include "table.hpp"

#include <string_view>
#include <toml++/toml.hpp>

int main() {
  using namespace std::string_view_literals;
  static constexpr auto source = R"(
    [Marshal]
    points = 60
    model_count = 1
    rarity = "Common"
    base = "20x20"
    height = 1
    cha = 4
    mob = 4
    dis = 8
    model_rules = ["Disciplined"]
    hp = 3
    def = 5
    res = 4
    arm = 3
    [[Marshal.offensive]]
    name = "Marshal"
    att = 3
    off = 5
    str = 4
    ap = 1
    agi = 5
    offensive_rules = ["Human", "Metal Armour"]
    [[Marshal.offensive]]
    name = "test"
    att = 1
    off = 1
    str = 1
    ap = 1
    agi = 1
    offensive_rules = ["Human", "Metal Armour"]
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
  Unit unit = table::add_unit(model_rules, book, "Marshal");
  std::get<reactive::Number>(unit.offenses[1].att).set(7);
}
