#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include "controller.hpp"
#include "event.hpp"
#include "exp_variable.hpp"

class Simulator {
 public:
  Simulator(void);
  void init(void);
  void run(void);

 private:
  double clock;
  std::list<Event> events;
  ExpVariable lambda;
  ExpVariable mu;
  Controller controller;
}

#endif