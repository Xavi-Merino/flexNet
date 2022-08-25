#include "link.hpp"

Link::Link(void) {
  this->id = -1;
  this->length = DEFAULT_LENGTH;
  this->slots = std::vector<std::vector<std::vector<bool>>>
                (1, std::vector<std::vector<bool>>(1, std::vector<bool>(DEFAULT_SLOTS,false)));
  this->src = -1;
  this->dst = -1;
  this->number_of_cores = DEFAULT_CORES;
  this->number_of_modes = DEFAULT_MODES;
}

Link::Link(int id) {
  this->id = id;

  this->length = DEFAULT_LENGTH;
  this->slots = std::vector<std::vector<std::vector<bool>>>
                (1, std::vector<std::vector<bool>>(1, std::vector<bool>(DEFAULT_SLOTS,false)));
  this->src = -1;
  this->dst = -1;
  this->number_of_cores = DEFAULT_CORES;
  this->number_of_modes = DEFAULT_MODES;
}

Link::Link(int id, float length) {
  this->id = id;

  if (length <= 0)
    throw std::runtime_error("Cannot create a link with non-positive length.");
  this->length = length;

  this->slots = std::vector<std::vector<std::vector<bool>>>
                (1, std::vector<std::vector<bool>>(1, std::vector<bool>(DEFAULT_SLOTS,false)));
  this->src = -1;
  this->dst = -1;
  this->number_of_cores = DEFAULT_CORES;
  this->number_of_modes = DEFAULT_MODES;
}


Link::Link(int id, float length, int slots) {
  this->id = id;

  if (length <= 0)
    throw std::runtime_error("Cannot create a link with non-positive length.");
  this->length = length;

  if (slots < 1)
    throw std::runtime_error("Cannot create a link with " +
                             std::to_string(slots) + " slots.");
  this->slots = std::vector<std::vector<std::vector<bool>>>
                (1, std::vector<std::vector<bool>>(1, std::vector<bool>(slots,false)));
  this->src = -1;
  this->dst = -1;
  this->number_of_cores = DEFAULT_CORES;
  this->number_of_modes = DEFAULT_MODES;
}

Link::Link(int id, float length, int slots, int number_of_cores) {
  this->id = id;

  if (length <= 0)
    throw std::runtime_error("Cannot create a link with non-positive length.");
  this->length = length;

  if (slots < 1)
    throw std::runtime_error("Cannot create a link with " +
                             std::to_string(slots) + " slots.");

  if (number_of_cores < 1)
    throw std::runtime_error("Cannot create a link with " +
                             std::to_string(number_of_cores) + " cores.");

  this->slots = std::vector<std::vector<std::vector<bool>>>
                (number_of_cores, std::vector<std::vector<bool>>
                (DEFAULT_MODES, std::vector<bool>(slots,false)));
  this->src = -1;
  this->dst = -1;
  this->number_of_cores = number_of_cores;
  this->number_of_modes = DEFAULT_MODES;
}

Link::Link(int id, float length, int slots, int number_of_cores, int number_of_modes) {
  this->id = id;

  if (length <= 0)
    throw std::runtime_error("Cannot create a link with non-positive length.");
  this->length = length;

  if (slots < 1)
    throw std::runtime_error("Cannot create a link with " +
                             std::to_string(slots) + " slots.");

  if (number_of_cores < 1)
    throw std::runtime_error("Cannot create a link with " +
                             std::to_string(number_of_cores) + " cores.");

  if (number_of_modes < 1)
    throw std::runtime_error("Cannot create a link with " +
                             std::to_string(number_of_modes) + " modes.");

  this->slots = std::vector<std::vector<std::vector<bool>>>
                (number_of_cores, std::vector<std::vector<bool>>
                (number_of_modes, std::vector<bool>(slots,false)));
  this->src = -1;
  this->dst = -1;
  this->number_of_cores = number_of_cores;
  this->number_of_modes = number_of_modes;
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
  for (int i = 0; i < this->getCores(); i++)
    for (int j = 0; j < this->getModes(); j++)
      for (int k = 0; k < this->getSlots(i,j); k++)
        if (this->slots[i][j][k] == true)
          throw std::runtime_error(
              "Cannot change slots number if at least one slot is active within this link.");

  for (int i = 0; i < this->getCores(); i++)
    for (int j = 0; j < this->getModes(); j++)
      this->slots[i][j].resize(slots);
}

void Link::setSlot(int pos, bool value) {
  if (pos < 0 || pos >= this->getSlots())
    throw std::runtime_error("Cannot set slot in position out of bounds.");

  if (this->getSlot(pos) == value)
    throw std::runtime_error("Slot already setted in desired state.");

  this->slots[0][0][pos] = value;
}

void Link::setCores(int number_of_cores) {
  if (number_of_cores < 1)
    throw std::runtime_error("Cannot set a link with " + std::to_string(number_of_cores) +
                             " cores.");

  if (this->getCores() == number_of_cores)
    throw std::runtime_error("Number of cores is already the desired.");

  for (int i = 0; i < this->getCores(); i++)
    for (int j = 0; j < this->getModes(); j++)
      for (int k = 0; k < this->getSlots(i,j); k++)
        if (this->slots[i][j][k] == true)
          throw std::runtime_error(
              "The number of cores cannot be changed if at least one slot is active within this core.");
  this->number_of_cores = number_of_cores;
  this->slots.resize(number_of_cores);
}

void Link::setModes(int number_of_modes) {
  if (number_of_modes < 1)
    throw std::runtime_error("Cannot set a link with " + std::to_string(number_of_modes) +
                             " modes.");

  if (this->getModes() == number_of_modes)
    throw std::runtime_error("Number of slots is already the desired.");

  for (int i = 0; i < this->getCores(); i++)
    for (int j = 0; j < this->getModes(); j++)
      for (int k = 0; k < this->getSlots(i,j); k++)
        if (this->slots[i][j][k] == true)
          throw std::runtime_error(
              "The number of modes cannot be changed if at least one slot is active within this mode.");

  this->number_of_modes = number_of_modes;
  for (int i = 0; i < this->getCores(); i++)
    this->slots[i].resize(number_of_modes);
}

void Link::setSlots(int slots, int core, int mode) {
  if (core < 0 || core >= this->getCores())
    throw std::runtime_error("Cannot set number of slots in core out of bounds.");

  if (mode < 0 || mode >= this->getModes())
    throw std::runtime_error("Cannot set number of slots in mode out of bounds.");

  if (slots < 1)
    throw std::runtime_error("Cannot set a link with " + std::to_string(slots) +
                             " slots.");

  if (this->getSlots(core, mode) == slots)
    throw std::runtime_error("Number of modes is already the desired.");

  for (int i = 0; i < this->getSlots(core, mode); i++) 
    if (this->slots[core][mode][i] == true)
      throw std::runtime_error(
          "Cannot change slots number if at least one slot is active.");

  this->slots[core][mode].resize(slots);
}

void Link::setSlot(int core, int mode, int pos, bool value) {
  if (core < 0 || core >= this->getCores())
    throw std::runtime_error("Cannot set slot in core out of bounds.");

  if (mode < 0 || mode >= this->getModes())
    throw std::runtime_error("Cannot set slot in mode out of bounds.");

  if (pos < 0 || pos >= this->getSlots(core, mode))
    throw std::runtime_error("Cannot set slot in position out of bounds.");

  if (this->getSlot(core, mode, pos) == value)
    throw std::runtime_error("Slot already setted in desired state.");

  this->slots[core][mode][pos] = value;
}

int Link::getId(void) const { return this->id; }

float Link::getLength(void) const { return this->length; }

int Link::getSlots(void) const { return this->slots[0][0].size(); }

int Link::getSlots(int core, int mode) const { return this->slots[core][mode].size(); }

bool Link::getSlot(int pos) const {
  if (pos < 0 || pos >= this->getSlots())
    throw std::runtime_error("Cannot get slot in position out of bounds.");

  return this->slots[0][0][pos];
}

bool Link::getSlot(int core, int mode, int pos) const {
  if (core < 0 || core >= this->getCores())
    throw std::runtime_error("Cannot set number of slots in core out of bounds.");

  if (mode < 0 || mode >= this->getModes())
    throw std::runtime_error("Cannot set number of slots in mode out of bounds.");

  if (pos < 0 || pos >= this->getSlots())
    throw std::runtime_error("Cannot get slot in position out of bounds.");

  return this->slots[core][mode][pos];
}

int Link::getCores(void) const { return this->number_of_cores; }

int Link::getModes(void) const { return this->number_of_modes; }

int Link::getSrc(void) const { return this->src; }

int Link::getDst(void) const { return this->dst; }