#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "bitrate.hpp"
#include "connection.hpp"
#include "network.hpp"

typedef enum allocStatus { ALLOCATED, NOT_ALLOCATED, N_A } allocationStatus;

class Allocator {
 public:
  Allocator(void);
  Allocator(Network *network);

  void setNetwork(Network *net);
  void setPaths(
      std::vector<std::vector<std::vector<std::vector<Link *>>>> *path);

  ~Allocator();

  virtual allocationStatus exec(int src, int dst, BitRate bitRate,
                                Connection &con);

 protected:
  Network *network;
  std::vector<std::vector<std::vector<std::vector<Link *>>>> *path;
};
#endif