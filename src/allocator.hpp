#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "connection.hpp"
#include "network.hpp"

typedef enum allocStatus { ALLOCATED, NOT_ALLOCATED } allocationStatus;

class Allocator {
 public:
  Allocator(Network *network);

  ~Allocator();

  virtual allocationStatus exec(int src, int dst, int bitRate, Connection &con);

 protected:
  Network *network;
};
#endif