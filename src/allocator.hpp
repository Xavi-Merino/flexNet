#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "connection.hpp"
#include "network.hpp"

class Allocator {
 public:
  Allocator(Network *network);

  ~Allocator();

  virtual int exec(int src, int dst, Connection &con);

 protected:
  Network *network;
};
#endif