#pragma once

#include "dom.hpp"
#include "wasm.hpp"

namespace reactive {

class Number {
public:
    Number(dom::node_t node, int number);
    void set(int number);
    int get();
private:
    int n;
    wasm::TableRef m_node;
};

}
