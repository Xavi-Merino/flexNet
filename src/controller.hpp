#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "allocator.hpp"
#include "connection.hpp"
#include "network.hpp"

class Controller {
 public:
  Controller();

  Controller(Network network);

  ~Controller();

  allocationStatus assignConnection(int src, int dst, int bitRate,
                                    long long idConnection);
  int unassignConnection(long long idConnection);

  void setPaths(std::string filename);

  void setNetwork(Network network);

  Network getNetwork(void);

  void setAllocator(Allocator *allocator);

 private:
  Network network;
  Allocator *allocator;
  std::vector<std::vector<std::vector<std::vector<Link *>>>> path;
  std::vector<Connection> connections;
  allocationStatus rtnAllocation;
};

#endif