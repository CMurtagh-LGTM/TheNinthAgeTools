#pragma once

#include <string_view>
#include <toml++/toml.hpp>
#include "unit.hpp"

namespace table {

Unit add_unit(const toml::table &rules, const toml::table &book,
               const std::string_view name);

}
