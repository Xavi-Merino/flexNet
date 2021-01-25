#include "dummy_allocator.hpp"

DummyAllocator::DummyAllocator(Network *network) : Allocator(network) {}

DummyAllocator::~DummyAllocator() {}

allocationStatus DummyAllocator::exec(int src, int dst, int bitRate,
                                      Connection &con) {
  if (this->network->isConnected(src, dst)) {
    con.addLink(0, 0, 2);
    return ALLOCATED;
  } else {
    return NOT_ALLOCATED;
  }
}
DummyAllocator::DummyAllocator() : Allocator() {}
