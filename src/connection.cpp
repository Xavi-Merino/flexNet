#include "connection.hpp"

Connection::Connection(long long id) {
  this->id = id;
  this->links = std::vector<int>();
  this->slots = std::vector<std::vector<int> >();
}

Connection::~Connection() {}

void Connection::addLink(int idLink, std::vector<int> slots) {
  this->links.push_back(idLink);
  this->slots.push_back(slots);
}

void Connection::addLink(int idLink, int fromSlot, int toSlot) {
  this->links.push_back(idLink);
  this->slots.push_back(std::vector<int>(toSlot - fromSlot));
  int j = 0;
  for (int i = fromSlot; i < toSlot; i++) {
    this->slots.back()[j] = i;
  }
}