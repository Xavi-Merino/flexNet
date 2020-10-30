#include "link.hpp"

Link::Link(void) {
  this->id = -1;
  this->length = DEFAULT_LENGTH;
  this->slots.resize(DEFAULT_SLOTS);
}

Link::Link(int id, float length) {
  this->id = id;
  this->length = length;
  this->slots.resize(DEFAULT_SLOTS);
}

Link::Link(int id, float length, int slots) {
  this->id = id;
  this->length = length;
  this->slots.resize(slots);
}

Link::~Link() {}

void Link::setId(int id) { this->id = id; }
void Link::setLength(float length) { this->length = length; }
void Link::setSlots(int slots) { this->slots.resize(slots); }

int Link::getId(void) { return this->id; }
float Link::getLength(void) { return this->length; }
int Link::getSlots(void) { return this->slots.size(); }