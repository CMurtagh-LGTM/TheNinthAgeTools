#pragma once

#include <string_view>
#include <toml++/toml.hpp>

namespace table {

void add_table(const toml::table &model_rules, const toml::table &book,
               const std::string_view name);

}
