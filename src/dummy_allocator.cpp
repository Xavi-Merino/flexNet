#include "dummy_allocator.hpp"

DummyAllocator::DummyAllocator(Network *network) : Allocator(network) {}

DummyAllocator::~DummyAllocator() {}

int DummyAllocator::exec(int src, int dst, Connection &con) {
  if (this->network->isConnected(src, dst)) {
    con.addLink(0, 0, 2);
    return 0;
  } else {
    return 1;
  }
}