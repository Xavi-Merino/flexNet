#include "allocator.hpp"

Allocator::Allocator(Network *network) { this->network = network; }

Allocator::~Allocator() {}

allocationStatus Allocator::exec(int src, int dst, int bitRate,
                                 Connection &con) {
  throw std::runtime_error(
      "You must implement a method to allocate resources. You can do this "
      "making an inherited class from Allocator, or ...");
}