#include "simulator.hpp"

#include <iostream>

Simulator::Simulator(void) {
  this->defaultValues();
  this->controller = new Controller();
  this->events = std::list<Event>();
  this->bitRatesDefault = std::vector<BitRate>();
  BitRate auxB = BitRate(10.0);
  auxB.addModulation(std::string("BPSK"), 1, 5520);
  this->bitRatesDefault.push_back(auxB);
  auxB = BitRate(40.0);
  auxB.addModulation(std::string("BPSK"), 4, 5520);
  this->bitRatesDefault.push_back(auxB);
  auxB = BitRate(100.0);
  auxB.addModulation(std::string("BPSK"), 8, 5520);
  this->bitRatesDefault.push_back(auxB);
  auxB = BitRate(400.0);
  auxB.addModulation(std::string("BPSK"), 32, 5520);
  this->bitRatesDefault.push_back(auxB);
  auxB = BitRate(1000.0);
  auxB.addModulation(std::string("BPSK"), 80, 5520);
  this->bitRatesDefault.push_back(auxB);
  this->allocatedConnections = 0;
}

Simulator::Simulator(std::string networkFilename, std::string pathFilename) {
  this->defaultValues();
  this->controller = new Controller();
  this->controller->setNetwork(new Network(networkFilename));
  this->controller->setPaths(pathFilename);
  this->events = std::list<Event>();
  this->bitRatesDefault = std::vector<BitRate>();
  BitRate auxB = BitRate(10.0);
  auxB.addModulation(std::string("BPSK"), 1, 5520);
  this->bitRatesDefault.push_back(auxB);
  auxB = BitRate(40.0);
  auxB.addModulation(std::string("BPSK"), 4, 5520);
  this->bitRatesDefault.push_back(auxB);
  auxB = BitRate(100.0);
  auxB.addModulation(std::string("BPSK"), 8, 5520);
  this->bitRatesDefault.push_back(auxB);
  auxB = BitRate(400.0);
  auxB.addModulation(std::string("BPSK"), 32, 5520);
  this->bitRatesDefault.push_back(auxB);
  auxB = BitRate(1000.0);
  auxB.addModulation(std::string("BPSK"), 80, 5520);
  this->bitRatesDefault.push_back(auxB);
  this->allocatedConnections = 0;
}

Simulator::~Simulator() {}

void Simulator::setLambda(double lambda) { this->lambda = lambda; }

void Simulator::setMu(double mu) { this->mu = mu; }

void Simulator::setSeedArrive(unsigned int seed) { this->seedArrive = seed; }

void Simulator::setSeedDeparture(unsigned int seed) {
  this->seedDeparture = seed;
}

void Simulator::setSeedBitRate(unsigned int seed) { this->seedBitRate = seed; }

void Simulator::setGoalConnections(long long goal) {
  this->goalConnections = goal;
}

void Simulator::setBitRates(std::vector<BitRate> bitRates) {
  this->bitRatesDefault = std::vector<BitRate>(bitRates);
}

void Simulator::setAllocator(Allocator *newAllocator) {
  newAllocator->setNetwork(this->controller->getNetwork());
  newAllocator->setPaths(this->controller->getPaths());
  this->controller->setAllocator(newAllocator);
}

void Simulator::defaultValues() {
  this->lambda = 3;
  this->mu = 10;
  this->seedArrive = 12345;
  this->seedDeparture = 12345;
  this->seedSrc = 12345;
  this->seedDst = 12345;
  this->seedBitRate = 12345;
  this->numberOfConnections = 0;
  this->numberOfEvents = 0;
  this->goalConnections = 10000;
  this->columnWidth = 10;
}

void Simulator::printInitialInfo() {
  std::cout << std::setfill(' ') << std::setw(20) << std::left << "Nodes:";
  std::cout << std::setw(30)
            << this->controller->getNetwork()->getNumberOfNodes() << "\n";
  std::cout << std::setw(20) << "Links:";
  std::cout << std::setw(30)
            << this->controller->getNetwork()->getNumberOfLinks() << "\n";
  std::cout << std::setw(20) << "Goal Connections:";
  std::cout << std::setw(30) << this->goalConnections << "\n";
  std::cout << "\n";

  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setw(11) << "+";
  std::cout << std::setw(11) << "+";
  std::cout << std::setw(11) << "+";
  std::cout << std::setw(1) << "+\n";

  std::cout << std::setfill(' ') << std::setw(11) << "| progress";
  std::cout << std::setw(11) << "| arrives";
  std::cout << std::setw(11) << "| blocking";
  std::cout << std::setw(11) << "| time(s)";
  std::cout << std::setw(1) << "|\n";

  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(1) << std::left << "+\n";

  this->startingTime = std::chrono::high_resolution_clock::now();
}

void Simulator::printRow(double percentage) {
  this->checkTime = std::chrono::high_resolution_clock::now();
  this->timeDuration =
      std::chrono::duration_cast<std::chrono::duration<double>>(
          this->checkTime - this->startingTime);
  std::cout << std::setprecision(1);
  std::cout << "|";
  std::cout << std::setfill(' ') << std::right << std::setw(7) << std::fixed
            << percentage << "%  |";
  std::cout << std::setfill(' ') << std::setw(7) << std::scientific
            << this->numberOfConnections - 1 << "   |";
  std::cout << std::setfill(' ') << std::setw(9) << std::right
            << std::scientific
            << 1 - this->allocatedConnections / this->numberOfConnections
            << " |";
  std::cout << std::setprecision(0) << std::setfill(' ') << std::setw(8)
            << std::right << std::fixed << this->timeDuration.count() << "  |";

  std::cout << std::setw(1) << "\n";
}

int Simulator::eventRoutine(void) {
  this->currentEvent = this->events.front();
  this->rtnAllocation = N_A;
  this->clock = this->currentEvent.getTime();
  if (this->currentEvent.getType() == ARRIVE) {
    nextEventTime = this->clock + this->arriveVariable.getNextValue();
    for (std::list<Event>::reverse_iterator pos = this->events.rbegin();
         pos != this->events.rend(); pos++) {
      if ((pos)->getTime() < nextEventTime) {
        this->events.insert((pos).base(), Event(ARRIVE, nextEventTime,
                                                this->numberOfConnections++));
        break;
      }
    }
    this->src = this->srcVariable.getNextIntValue();
    this->dst = this->dstVariable.getNextIntValue();
    while (this->src == this->dst) {
      this->dst = this->dstVariable.getNextIntValue();
    }
    this->bitRate = bitRateVariable.getNextIntValue();
    this->rtnAllocation = this->controller->assignConnection(
        this->src, this->dst, this->bitRates[this->bitRate],
        this->currentEvent.getIdConnection());
    if (this->rtnAllocation == ALLOCATED) {
      nextEventTime = this->clock + this->departVariable.getNextValue();
      for (std::list<Event>::reverse_iterator pos = this->events.rbegin();
           pos != this->events.rend(); pos++) {
        if ((pos)->getTime() < nextEventTime) {
          this->events.insert((pos).base(),
                              Event(DEPARTURE, nextEventTime,
                                    this->currentEvent.getIdConnection()));
          break;
        }
      }
      this->allocatedConnections++;
    }
  } else if (this->currentEvent.getType() == DEPARTURE) {
    this->controller->unassignConnection(this->currentEvent.getIdConnection());
  }
  this->events.pop_front();
  return this->rtnAllocation;
}

void Simulator::init(void) {
  this->clock = 0;
  this->arriveVariable = ExpVariable(this->seedArrive, this->lambda);
  this->departVariable = ExpVariable(this->seedDeparture, this->mu);
  this->srcVariable = UniformVariable(
      this->seedSrc, this->controller->getNetwork()->getNumberOfNodes() - 1);
  this->dstVariable = UniformVariable(
      this->seedDst, this->controller->getNetwork()->getNumberOfNodes() - 1);
  this->bitRateVariable =
      UniformVariable(this->seedBitRate, this->bitRatesDefault.size() - 1);
  this->events.push_back(Event(ARRIVE, this->arriveVariable.getNextValue(),
                               this->numberOfConnections++));
  this->bitRates = this->bitRatesDefault;
}

void Simulator::run(void) {
  int timesToShow = 20;
  int arrivesByCycle = this->goalConnections / timesToShow;
  printInitialInfo();
  for (int i = 1; i <= timesToShow; i++) {
    while (this->numberOfConnections <= i * arrivesByCycle) {
      eventRoutine();
    }
    printRow((100 / timesToShow) * i);
    // std::cout << (100 / timesToShow) * i << "% \t Blocking probability: "
    //          << 1 - this->allocatedConnections / this->numberOfConnections
    //          << "\n";
  }
}