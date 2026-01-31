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

    ["State Infantry"]
    points = 160
    model_count = 20
    rarity = "Regular"
    base = "20x20"
    height = 1
    cha = 4
    mob = 4
    dis = 7
    model_rules = ["Parent Unit"]
    hp = 1
    def = 3
    res = 3
    arm = 2
    [["State Infantry".offensive]]
    name = "State Infantry"
    att = 1
    off = 3
    str = 3
    ap = 0
    agi = 3
    offensive_rules = ["Human", "Metal Armour"]

  )"sv;
  static constexpr auto model_rules_source = R"(
    Disciplined = "Command Tests taken by a unit containing Disciplined gain {{Minimized}}."
    Minimized = "For each instance of Minimised, add a dice to the roll, and discard the dice that rolled the highest result."
    "Parent Unit" = "A unit containing a model with this rule is referred to as a ‘Parent Unit’. Parent Unit gains {{Fight in Extra Rank}} while there is at least one {{Support Unit}} engaged in the same combat."
    "Support Unit" = "-"
    "Fight in Extra Rank" = "A model part with Fight in Extra Rank, or using a weapon with Fight in Extra Rank, can make Supporting Attacks from one additional rank."
  )"sv;
  auto book = toml::parse(source);
  auto model_rules = toml::parse(model_rules_source);
  if (!book) {
    wasm::log("failed to parse toml book");
    return 0;
  }
  // table::add_unit(model_rules, book, "Marshal");
  table::add_unit(model_rules, book, "State Infantry");
}
