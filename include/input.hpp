#pragma once

#include "dom.hpp"
#include "wasm.hpp"
#include "function_ref.hpp"

// #include "memory_log.hpp"

#include <functional>

namespace input {

namespace internal {
WASM_EXPORT("wwwwwwww")
int wwwwwwww(int id);

WASM_EXPORT("call_slot")
void call_slot(size_t id);

WASM_IMPORT_MODULE("input", "add_click_listener")
extern void add_click_listener(dom::node_t node, size_t index);
}

using slot_t = std::function<void()>;
// TODO use function_ref when clang-wasi-sdk supports it
using slot_ref_t = tl::function_ref<void()>;

size_t alloc_slot(slot_ref_t slot);
void dealoc_slot(size_t index);

void add_click_listener(dom::node_t node, slot_ref_t slot);

// TODO will I use this?
class SlotRef {
public:
    static constexpr size_t NULL_REF = 0xFFFFFFFF;

    SlotRef();
    SlotRef(slot_t slot);
    ~SlotRef();
    SlotRef(const SlotRef&) = delete;
    SlotRef(SlotRef&& other);
    SlotRef& operator=(const SlotRef&) = delete;
    SlotRef& operator=(SlotRef&& other);

    slot_t& operator*();
    operator bool() const;

private:
    size_t index;
};

}

