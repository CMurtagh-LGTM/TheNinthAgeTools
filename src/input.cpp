#include "input.hpp"

namespace input {

static std::vector<slot_t> slots = std::vector<slot_t>(16, slot_t());
static size_t next_free = 0;
static std::vector<int> test = {7, 7};
static std::array<int, 2> test2 = {7, 7};
static auto used_elements = std::vector<uint8_t>(4, false);

int internal::wwwwwwww(int id) {
  wasm::fmt_log("e {} {}", test, test2);
  test[0] = 9;
  test2[0] = 8;
  return (int) test.data();
}

void internal::call_slot(size_t id) {
  wasm::fmt_log("e {} {} {} {} {} {} {}", used_elements, (void*)used_elements.data(), (void*)&used_elements, (void*)slots.data(), slots.capacity(), (void*)&next_free, test);
  wasm::wasm_assert(id < used_elements.size(), "Slot id out of bounds {}", id);
  wasm::wasm_assert(used_elements[id], "Tried to call slot that was empty {}", id);
  wasm::wasm_assert(static_cast<bool>(slots[id]),
                    "Tried to call slot was not contained {}", id);
  slots[id]();
}

size_t alloc_slot(slot_ref_t slot) {
  wasm::fmt_log("ea {} {} {}", used_elements, (void*)used_elements.data(), slots.capacity());
  size_t index = next_free;
  wasm::wasm_assert(!used_elements[index],
                    "slots tried to allocate an existing slot");
  used_elements[index] = true;
  wasm::fmt_log("Allocated {} {}", index, used_elements[index]);
  do {
    next_free++;
    if (used_elements.size() <= next_free) {
      used_elements.push_back(false);
      break;
    }
  } while (used_elements[next_free]);

  if (index == slots.size()) {
    // Make sure push_back will place index correctly
    wasm::wasm_assert(index == slots.size(),
                      "new slot was not at the end");
    // slots.push_back(slot);
  } else if (index < slots.size()) {
    // slots[index] = slot;
  } else {
    wasm::wasm_assert(false, "Tried to allocate slot far beyond the end");
  }
  wasm::fmt_log("e {} {}", used_elements, (void*)used_elements.data());
  return index;
}

void dealoc_slot(size_t index) {
  if (index == SlotRef::NULL_REF) {
    return;
  }
  wasm::fmt_log("Deallocated {}", index);
  used_elements[index] = false;
  next_free = std::min(next_free, index);
  wasm::fmt_log("e {} {}", used_elements, (void*)used_elements.data());
}

void add_click_listener(dom::node_t node, slot_ref_t slot) {
  auto index = alloc_slot(slot);
  internal::add_click_listener(node, index);
}

SlotRef::SlotRef() : index(NULL_REF) {}

SlotRef::SlotRef(slot_t slot) :index(alloc_slot(slot)) {}

SlotRef::SlotRef(SlotRef&& other) : index(other.index) {
  other.index = NULL_REF;
}

SlotRef& SlotRef::operator=(SlotRef&& other) {
  index = other.index;
  other.index = NULL_REF;
  return *this;
}

SlotRef::~SlotRef() {
  dealoc_slot(index);
  index = NULL_REF;
}

slot_t& SlotRef::operator*() {
  wasm::wasm_assert(index != NULL_REF, "slots tried to dereference null_ref");
  wasm::wasm_assert(used_elements[index],
                    "slots tried to dereference a free slot");

  return slots[index];
}

SlotRef::operator bool() const { return index != NULL_REF; }

}  // namespace input
