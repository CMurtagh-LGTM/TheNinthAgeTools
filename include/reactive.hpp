#pragma once

#include <variant>
#include <toml++/toml.hpp>

#include "dom.hpp"
#include "wasm.hpp"

namespace reactive {

class Number {
public:
    Number() = default;
    void set(int number);
    void set(wasm::TableRef&& node, int number);
    int get();
private:
    static constexpr std::string format = "{}";
    int n;
    wasm::TableRef m_node;
};


class Distance {
public:
    Distance() = default;
    void set(int number);
    void set(wasm::TableRef&& node, int number);
    int get();
private:
    static constexpr std::string format = "{}\"";
    int n;
    wasm::TableRef m_node;
};

class Rule {
};

class Rules {
public:
    void set(wasm::TableRef&& node, const toml::table& rules, const toml::array* array_ptr);
private:
    wasm::TableRef m_node;
};

using Reactive = std::variant<Number, Distance, Rules>;

}
