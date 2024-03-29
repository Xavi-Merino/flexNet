#include "dummy_allocator.hpp"

DummyAllocator::DummyAllocator(Network *network) : Allocator(network) {
  this->name = std::string("Dummy Allocator");
}

DummyAllocator::~DummyAllocator() {}

allocationStatus DummyAllocator::exec(int src, int dst, BitRate bitRate,
                                      Connection &con) {
  int link = this->network->isConnected(src, dst);
  if (link != -1) {
    if (!this->network->isSlotUsed(link, 0, 2)) {
      con.addLink(link, 0, 2);
      return ALLOCATED;
    } else {
      return NOT_ALLOCATED;
    }
  } else {
    return NOT_ALLOCATED;
  }
}
DummyAllocator::DummyAllocator() : Allocator() {}
