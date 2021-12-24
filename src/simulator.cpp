#include "simulator.hpp"

#include <cmath>  //Nuevo
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

Simulator::Simulator(std::string networkFilename, std::string pathFilename,
                     std::string bitrateFilename) {
  this->defaultValues();
  this->controller = new Controller();
  this->controller->setNetwork(new Network(networkFilename));
  this->controller->setPaths(pathFilename);
  this->events = std::list<Event>();
  this->bitRatesDefault = BitRate::readBitRateFile(bitrateFilename);
  this->allocatedConnections = 0;
}

Simulator::~Simulator() {}

void Simulator::setLambda(double lambda) {
  if (this->initReady) {
    throw std::runtime_error(
        "You can not set lambda parameter AFTER calling init simulator "
        "method.");
  }
  this->lambda = lambda;
}

void Simulator::setMu(double mu) {
  if (this->initReady) {
    throw std::runtime_error(
        "You can not set mu parameter AFTER calling init simulator "
        "method.");
  }
  this->mu = mu;
}

void Simulator::setSeedArrive(unsigned int seed) {
  if (this->initReady) {
    throw std::runtime_error(
        "You can not set seed arrive parameter AFTER calling init simulator "
        "method.");
  }
  this->seedArrive = seed;
}

void Simulator::setSeedDeparture(unsigned int seed) {
  if (this->initReady) {
    throw std::runtime_error(
        "You can not set seed departure parameter AFTER calling init simulator "
        "method.");
  }
  this->seedDeparture = seed;
}

void Simulator::setSeedBitRate(unsigned int seed) {
  if (this->initReady) {
    throw std::runtime_error(
        "You can not set seed bitrate parameter AFTER calling init simulator "
        "method.");
  }
  this->seedBitRate = seed;
}

void Simulator::setSeedSrc(unsigned int seed) {
  if (this->initReady) {
    throw std::runtime_error(
        "You can not set seed source parameter AFTER calling init simulator "
        "method.");
  }
  this->seedSrc = seed;
}

void Simulator::setSeedDst(unsigned int seed) {
  if (this->initReady) {
    throw std::runtime_error(
        "You can not set seed destination parameter AFTER calling init "
        "simulator "
        "method.");
  }
  this->seedDst = seed;
}

void Simulator::setGoalConnections(long long goal) {
  if (this->initReady) {
    throw std::runtime_error(
        "You can not set goal connections parameter AFTER calling init "
        "simulator method.");
  }
  this->goalConnections = goal;
}

void Simulator::setBitRates(std::vector<BitRate> bitRates) {
  if (this->initReady) {
    throw std::runtime_error(
        "You can not set bitrates parameter AFTER calling init simulator "
        "method.");
  }
  this->bitRatesDefault = std::vector<BitRate>(bitRates);
}

void Simulator::setAllocator(Allocator *newAllocator) {
  if (this->initReady) {
    throw std::runtime_error(
        "You can not set allocator parameter AFTER calling init simulator "
        "method.");
  }
  newAllocator->setNetwork(this->controller->getNetwork());
  newAllocator->setPaths(this->controller->getPaths());
  this->controller->setAllocator(newAllocator);
}

void Simulator::setConfidence(double c) {
  if (c <= 0 || c >= 1) {
    throw std::runtime_error(
        "You can't set a confidence interval with confidence equal/higher than "
        "1 or equal/lower than 0.");
  }
  this->confidence = c;
}

void Simulator::defaultValues() {
  this->initReady = false;
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
  this->confidence = 0.95;
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
  std::cout << std::setw(20) << "Lambda:";
  std::cout << std::setw(30) << this->lambda << "\n";
  std::cout << std::setw(20) << "Mu:";
  std::cout << std::setw(30) << this->mu << "\n";
  std::cout << std::setw(20) << "Algorithm:";
  std::cout << std::setw(30) << this->controller->getAllocator()->getName()
            << "\n";
  std::cout << "\n";

  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setw(11) << "+";
  std::cout << std::setw(11) << "+";
  std::cout << std::setw(11) << "+";
  std::cout << std::setw(11) << "+";
  std::cout << std::setw(11) << "+";
  std::cout << std::setw(11) << "+";
  std::cout << std::setw(1) << "+\n";

  std::cout << std::setfill(' ') << std::setw(11) << "| progress";
  std::cout << std::setw(11) << "| arrives";
  std::cout << std::setw(11) << "| blocking";
  std::cout << std::setw(11) << "| time(s)";
  std::cout << std::setw(11) << "| Wald CI";
  std::cout << std::setw(11) << "| A-C. CI";
  std::cout << std::setw(11) << "| Wilson CI";
  std::cout << std::setw(1) << "|\n";

  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  // Nueva
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
            << " |";  // getBlockingProbability
  std::cout << std::setprecision(0) << std::setfill(' ') << std::setw(8)
            << std::right << std::fixed << this->timeDuration.count() << "  |";

  std::cout << std::setprecision(1) << std::setfill(' ') << std::setw(9)
            << std::right << std::scientific << this->waldCI() << " |";

  std::cout << std::setfill(' ') << std::setw(9) << std::right
            << std::scientific << this->agrestiCI() << " |";

  std::cout << std::setfill(' ') << std::setw(9) << std::right
            << std::scientific << this->wilsonCI() << " |";

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
        this->currentEvent.getIdConnection(), this->clock);
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
    (this->controller->*(this->controller->unassignConnection))(
        this->currentEvent.getIdConnection(), this->clock);
  }
  this->events.pop_front();
  return this->rtnAllocation;
}

void Simulator::init(void) {
  this->initReady = true;
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
  this->initZScore();
}

void Simulator::run(void) {
  float timesToShow = 20;
  float arrivesByCycle = this->goalConnections / timesToShow;
  printInitialInfo();
  for (int i = 1; i <= timesToShow; i++) {
    while (this->numberOfConnections <= i * arrivesByCycle) {
      eventRoutine();
    }
    printRow((100 / timesToShow) * i);
  }
}

unsigned int Simulator::getTimeDuration(void) {
  return static_cast<unsigned int>(this->timeDuration.count());
}

double Simulator::getBlockingProbability(void) {
  return 1 - this->allocatedConnections / this->numberOfConnections;
}

double Simulator::getAllocatedProbability(void) {
  return this->allocatedConnections / this->numberOfConnections;
}

double Simulator::waldCI() {
  double np = this->getAllocatedProbability();
  double p = 1 - np;
  int n = this->numberOfConnections;
  double sd = sqrt((np * p) / n);

  return this->zScore * sd;
}

double Simulator::agrestiCI() {
  double np = this->getAllocatedProbability();
  int n = this->numberOfConnections;

  np = np * ((n * (this->allocatedConnections + 2)) /
             (this->allocatedConnections * (n + 4)));

  double p = 1 - np;
  double sd = sqrt((np * p) / (n + 4));

  return this->zScore * sd;
}

double Simulator::wilsonCI() {
  double np = this->getAllocatedProbability();
  double p = 1 - np;
  int n = this->numberOfConnections;

  double denom = (1 + (pow(this->zScore, 2) / n));

  double k = p + pow(this->zScore, 2) / (2 * n);
  double sd = sqrt(((np * p) / n) + ((pow(this->zScore, 2)) / (4 * pow(n, 2))));

  return (this->zScore * sd) / denom;
}

void Simulator::initZScore(void) {
  double actual = 0.0;
  double step = 1.0;
  double covered = 0.0;
  double objective = this->confidence;
  double epsilon = 1e-6;

  while (fabs(objective - covered) > epsilon) {
    if (objective > covered) {
      actual += step;
      covered =
          ((1 + erf(actual / sqrt(2))) - (1 + erf(-actual / sqrt(2)))) / 2;
      if (covered > objective) {
        step /= 2;
      }
    } else {
      actual -= step;
      covered =
          ((1 + erf(actual / sqrt(2))) - (1 + erf(-actual / sqrt(2)))) / 2;
      if (covered < objective) {
        step /= 2;
      }
    }
  }
  this->zScore = actual;
}

void Simulator::setUnassignCallback(void (*callbackFunction)(Connection, double,
                                                             Network *)) {
  this->controller->setUnassignCallback(callbackFunction);
}