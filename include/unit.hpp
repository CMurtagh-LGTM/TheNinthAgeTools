#pragma once

#include "reactive.hpp"

enum class Rarity {
    COMMON
};

struct Offense {
    Offense();
    reactive::Reactive att;
    reactive::Reactive off;
    reactive::Reactive str;
    reactive::Reactive ap;
    reactive::Reactive agi;
    reactive::Reactive offensive_rules;
};

// Could this just be a std::unordered_map?
struct Unit {
    Unit();
    // const toml::table& rules;
    // std::string name;
    // int model_count;
    // Rarity rarity;
    // std::string base;
    // reactive::Reactive height;

    reactive::Reactive cha;
    reactive::Reactive mob;
    reactive::Reactive dis;
    reactive::Reactive model_rules;

    reactive::Reactive hp;
    reactive::Reactive def;
    reactive::Reactive res;
    reactive::Reactive arm;

    std::vector<Offense> offenses{};
};
