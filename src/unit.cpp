#include "unit.hpp"

Offense::Offense():
    att(reactive::Number()),
    off(reactive::Number()),
    str(reactive::Number()),
    ap(reactive::Number()),
    agi(reactive::Number()),
    offensive_rules(reactive::Rules())
{}

Unit::Unit():
    cha(reactive::Distance()),
    mob(reactive::Distance()),
    dis(reactive::Number()),
    model_rules(reactive::Rules()),

    hp(reactive::Number()),
    def(reactive::Number()),
    res(reactive::Number()),
    arm(reactive::Number())
{}
