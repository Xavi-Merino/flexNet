#ifndef __DUMMY_ALLOCATOR_H__
#define __DUMMY_ALLOCATOR_H__

#include "allocator.hpp"

class DummyAllocator : public Allocator {
 public:
  DummyAllocator(Network *network);
  ~DummyAllocator();

  allocationStatus exec(int src, int dst, int bitRate, Connection &con);
};

#endif