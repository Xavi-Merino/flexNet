#ifndef __DUMMY_ALLOCATOR_H__
#define __DUMMY_ALLOCATOR_H__

#include "allocator.hpp"

class DummyAllocator : public Allocator {
 public:
  DummyAllocator(Network *network);
  ~DummyAllocator();

  int exec(int src, int dst, Connection &con);
};

#endif