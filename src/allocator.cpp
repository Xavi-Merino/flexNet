#include "allocator.hpp"

Allocator::Allocator(Network *network) {
  this->network = network;
  this->name = std::string("No name");
}

Allocator::~Allocator() {}

allocationStatus Allocator::exec(int src, int dst, BitRate bitRate,
                                 Connection &con) {
  throw std::runtime_error(
      "You must implement a method to allocate resources. You can do this "
      "making an inherited class from Allocator, or ...");
}

std::string Allocator::getName(void) { return this->name; }

Allocator::Allocator(void) {
  this->network = nullptr;
  this->path = nullptr;
}

void Allocator::setNetwork(Network *net) { this->network = net; }

void Allocator::setPaths(
    std::vector<std::vector<std::vector<std::vector<Link *>>>> *path) {
  this->path = path;
}
