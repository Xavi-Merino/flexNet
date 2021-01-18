#include "simulator.hpp"

Simulator::Simulator(void) {
  this->defaultValues();
  this->controller = Controller();
  this->events = std::list<Event>();
  this->bitRatesDefault =
      std::vector<double>({10.0, 40.0, 100.0, 400.0, 1000.0});
}

Simulator::Simulator(std::string networkFilename, std::string pathFilename) {
  this->defaultValues();
  this->controller = Controller();
  this->controller.setNetwork(Network(networkFilename));
  this->controller.setPaths(pathFilename);
  this->events = std::list<Event>();
  this->bitRatesDefault =
      std::vector<double>({10.0, 40.0, 100.0, 400.0, 1000.0});
}

void Simulator::run(void) {}

void Simulator::setLambda(double lambda) { this->lambda = lambda; }

void Simulator::setMu(double mu) { this->mu = mu; }

void Simulator::setSeedArrive(unsigned int seed) { this->seedArrive = seed; }

void Simulator::setSeedDeparture(double seed) { this->seedDeparture = seed; }

void Simulator::setGoalConnections(long long goal) {
  this->goalConnections = goal;
}

void Simulator::setBitRates(std::vector<double> bitRates) {
  this->bitRatesDefault = std::vector<double>(bitRates);
}

void Simulator::defaultValues() {
  this->lambda = 3;
  this->mu = 10;
  this->seedArrive = 12345;
  this->seedDeparture = 12345;
  this->seedSrc = 12345;
  this->seedDst = 12345;
  this->numberOfConnections = 0;
  this->goalConnections = 10000;
}

int Simulator::eventRoutine(void) {
  this->currentEvent = this->events.front();
  this->rtnAllocation = -1;
  if (this->currentEvent.getType() == ARRIVE) {
    nextEventTime = this->clock + this->arriveVariable.getNextValue();
    for (std::list<Event>::reverse_iterator pos = this->events.rbegin();
         pos != this->events.rend(); pos++) {
      if (pos->getTime() < nextEventTime) {
        this->events.insert(++pos.base(), Event(ARRIVE, nextEventTime,
                                                this->numberOfConnections++));
      }
    }
    src = this->srcVariable.getNextIntValue();
    dst = this->dstVariable.getNextIntValue();
    while (src == dst) {
      dst = this->dstVariable.getNextIntValue();
    }
    this->rtnAllocation = this->controller.assignConnection(src, dst, 10);
  } else if (this->currentEvent.getType() == DEPARTURE) {
  }
  this->events.pop_front();
  return this->rtnAllocation;
}

void Simulator::init(void) {
  this->clock = 0;
  this->arriveVariable = ExpVariable(this->seedArrive, this->lambda);
  this->departVariable = ExpVariable(this->seedDeparture, this->mu);
  this->srcVariable = UniformVariable(
      this->seedSrc, this->controller.getNetwork().getNumberOfNodes() - 1);
  this->dstVariable = UniformVariable(
      this->seedDst, this->controller.getNetwork().getNumberOfNodes() - 1);
  this->events.push_back(Event(ARRIVE, this->arriveVariable.getNextValue(),
                               this->numberOfConnections++));
  this->bitRates = this->bitRatesDefault;
}