#include "link.hpp"

Link::Link(void) { this->id = -1; }

Link::Link(int id) { this->id = id; }

Link::~Link() {}

void Link::setId(int id) { this->id = id; }

int Link::getId(void) { return this->id; }