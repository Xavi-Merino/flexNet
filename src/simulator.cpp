#include "simulator.hpp"

Simulator::Simulator(void) {
  this->clock = 0;
  this->arriveVariable = ExpVariable(123456, 3);
  this->departVariable = ExpVariable(123456, 10);
  this->controller = Controller();
}

void Simulator::run(void) {}

void Simulator::init(void) {}