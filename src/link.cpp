#include "link.hpp"

Link::Link(void) {
  this->id = -1;
  this->length = DEFAULT_LENGTH;
  this->slots = std::vector<bool>(DEFAULT_SLOTS);
  this->src = -1;
  this->dst = -1;
}

Link::Link(int id) {
  this->id = id;

  this->length = DEFAULT_LENGTH;
  this->slots = std::vector<bool>(DEFAULT_SLOTS);
  this->src = -1;
  this->dst = -1;
}

Link::Link(int id, float length) {
  this->id = id;

  if (length <= 0)
    throw std::runtime_error("Cannot create a link with non-positive length.");
  this->length = length;

  this->slots = std::vector<bool>(DEFAULT_SLOTS);
  this->src = -1;
  this->dst = -1;
}

Link::Link(int id, float length, int slots) {
  this->id = id;

  if (length <= 0)
    throw std::runtime_error("Cannot create a link with non-positive length.");
  this->length = length;

  if (slots < 1)
    throw std::runtime_error("Cannot create a link with " +
                             std::to_string(slots) + " slots.");
  this->slots = std::vector<bool>(slots);
  this->src = -1;
  this->dst = -1;
}

Link::~Link() {}

void Link::setId(int id) {
  if (this->id != -1)
    throw std::runtime_error(
        "Cannot set Id to a Link with Id different than -1.");

  this->id = id;
}

void Link::setLength(float length) {
  if (length <= 0)
    throw std::runtime_error("Cannot set a link with non-positive length.");
  this->length = length;
}

void Link::setSlots(int slots) {
  if (slots < 1)
    throw std::runtime_error("Cannot set a link with " + std::to_string(slots) +
                             " slots.");
  for (int i = 0; i < this->getSlots(); i++) {
    if (this->slots[i] == true)
      throw std::runtime_error(
          "Cannot change slots number if at least one slot is active.");
  }
  this->slots.resize(slots);
}

void Link::setSlot(int pos, bool value) {
  if (pos < 0 || pos >= this->getSlots())
    throw std::runtime_error("Cannot set slot in position out of bounds.");

  if (this->getSlot(pos) == value)
    throw std::runtime_error("Slot already setted in desired state.");

  this->slots[pos] = value;
}

int Link::getId(void) const { return this->id; }

float Link::getLength(void) const { return this->length; }

int Link::getSlots(void) const { return this->slots.size(); }

bool Link::getSlot(int pos) const {
  if (pos < 0 || pos >= this->getSlots())
    throw std::runtime_error("Cannot get slot in position out of bounds.");

  return this->slots[pos];
}

int Link::getSrc(void) const { return this->src; }

int Link::getDst(void) const { return this->dst; }