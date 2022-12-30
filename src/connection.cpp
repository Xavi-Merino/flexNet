#include "connection.hpp"

Connection::Connection(long long id, double time, BitRate *bitRate) {
  this->id = id;
  this->links = std::vector<int>();
  this->slots = std::vector<std::vector<int> >();
  this->cores = std::vector<int>();
  this->modes = std::vector<int>();
  this->timeConnection = time;
  this->bitRate = bitRate;
}

Connection::~Connection() {}

void Connection::addLink(int idLink, std::vector<int> slots) {
  this->links.push_back(idLink);
  //this->modes.push_back(0);
  //this->cores.push_back(0);
  this->slots.push_back(slots);
}

void Connection::addLink(int idLink, int fromSlot, int toSlot) {
  this->links.push_back(idLink);
  //this->modes.push_back(0);
  //this->cores.push_back(0);
  this->slots.push_back(std::vector<int>(toSlot - fromSlot));
  int j = 0;
  for (int i = fromSlot; i < toSlot; i++) {
    this->slots.back()[j] = i;
    j++;
  }
}

void Connection::addLink(int idLink, int core, int mode, int fromSlot, int toSlot) {
  this->links.push_back(idLink);
  this->modes.push_back(mode);
  this->cores.push_back(core);
  this->slots.push_back(std::vector<int>(toSlot - fromSlot));
  int j = 0;
  for (int i = fromSlot; i < toSlot; i++) {
    this->slots.back()[j] = i;
    j++;
  }
}

std::vector<int> Connection::getLinks(void) { return this->links; }
std::vector<int> Connection::getCores(void) { return this->cores; }
std::vector<int> Connection::getModes(void) { return this->modes; }
std::vector<std::vector<int> > Connection::getSlots(void) {
  return this->slots;
}

double Connection::getTimeConnection(void) { return this->timeConnection; }
BitRate *Connection::getBitrate(void) { return this->bitRate; }
long long Connection::getId(void) { return this->id; }