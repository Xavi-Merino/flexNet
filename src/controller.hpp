#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "network.hpp"

class Controller {
 public:
  Controller();

  Controller(Network network);

  ~Controller();

  int assignConnection(int src, int dst, int numberOfSlots);

  void setPaths(std::string filename);

  void setNetwork(Network network);

 private:
  Network network;
  std::vector<std::vector<std::vector<std::vector<Link*>>>> path;
};

#endif