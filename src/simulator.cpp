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
  std::cout << std::setw(30) << "+";  // Nueva
  std::cout << std::setw(1) << "+\n";

  std::cout << std::setfill(' ') << std::setw(11) << "| progress";
  std::cout << std::setw(11) << "| arrives";
  std::cout << std::setw(11) << "| blocking";
  std::cout << std::setw(11) << "| time(s)";
  std::cout << std::setw(30) << "| Wald CI | Agresti-Coull |";  // Nueva
  std::cout << std::setw(1) << "|\n";

  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(11) << std::left << "+";
  std::cout << std::setfill('-') << std::setw(30) << std::left << "+";  // Nueva
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

  // Nuevo
  /*
  std::cout << std::scientific << std::setprecision(8) << std::setfill(' ')
            << std::right << std::setw(7) << std::fixed << "["
            << this->confidenceInterval(0.95, true, 0) << ";"
            << this->confidenceInterval(0.95, false, 0) << "]"
            << " |";

  std::cout << std::scientific << std::setprecision(8) << std::setfill(' ')
            << std::right << std::setw(7) << std::fixed << "["
            << this->confidenceInterval(0.95, true, 1) << ";"
            << this->confidenceInterval(0.95, false, 1) << "]"
            << " |";
  */

  std::cout << std::scientific << std::setprecision(8) << std::setfill(' ')
            << std::right << std::setw(7) << std::fixed << std::endl
            << "\t[" << this->waldCI(1.96, true) << ";"
            << this->waldCI(1.96, false) << "]"
            << " = " << this->waldCI(1.96, false) - this->waldCI(1.96, true)
            << " |";

  std::cout << std::scientific << std::setprecision(8) << std::setfill(' ')
            << std::right << std::setw(7) << std::fixed << "\t["
            << this->agrestiCI(1.96, true) << ";"
            << this->agrestiCI(1.96, false) << "]"
            << " = "
            << this->agrestiCI(1.96, false) - this->agrestiCI(1.96, true)
            << " |";

  std::cout << std::scientific << std::setprecision(8) << std::setfill(' ')
            << std::right << std::setw(7) << std::fixed << "\t["
            << this->wilsonCI(1.96, true) << ";" << this->wilsonCI(1.96, false)
            << "]"
            << " = " << this->wilsonCI(1.96, false) - this->wilsonCI(1.96, true)
            << " |";
  /*
  std::cout << "n: " << this->numberOfConnections
            << " | a: " << this->allocatedConnections
            << " | p: " << this->getBlockingProbability(false)
            << " | np: " << this->getAllocatedProbability(false)
            << " | np2: " << 1 - this->getBlockingProbability(false);

  float z = 1.96;
  float n = this->numberOfConnections;
  float p = this->getAllocatedProbability(false);
  float np = 1 - p;
  float np2 = this->getBlockingProbability(false);
  float sd = sqrt((np * p) / this->numberOfConnections);
  std::cout << "n: " << n << " | p: " << p << " | np: " << np
            << "  | np2:" << np2 << " |" << std::endl;
  */
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

double Simulator::getBlockingProbability(bool type) {
  if (type == true)
    return 1 -
           ((this->allocatedConnections + 2) / (this->numberOfConnections + 4));

  return 1 - this->allocatedConnections / this->numberOfConnections;
}

double Simulator::getAllocatedProbability(bool type) {
  if (type == true)
    return (this->allocatedConnections + 2) / (this->numberOfConnections + 4);

  return this->allocatedConnections / this->numberOfConnections;
}

/*
double Simulator::infCI(double confidence) {
  if (confidence <= 0 || confidence >= 1) {
    throw std::runtime_error(
        "You can't set a confidence interval with confidence equal/higher than "
        "1 or equal/lower than 0.");
  }

  // double significance = 1 - confidence; //Para después
  float z = 1.96;
  float error =
      z *
      sqrt((this->getAllocatedProbability() * this->getBlockingProbability()) /
           this->numberOfConnections);

  return this->getBlockingProbability() - error;
}
*/

double Simulator::confidenceInterval(double confidence, bool inf, int type) {
  if (confidence <= 0 || confidence >= 1) {
    throw std::runtime_error(
        "You can't set a confidence interval with confidence equal/higher than "
        "1 or equal/lower than 0.");
  }

  // double significance = 1 - confidence; //Para después
  float z = 1.96;
  float error, result;

  switch (type) {
    default:
      error = z * sqrt((this->getAllocatedProbability(false) *
                        this->getBlockingProbability(false)) /
                       this->numberOfConnections);

      if (inf == true) {
        result = this->getBlockingProbability(false) - error;
      }

      else {
        result = this->getBlockingProbability(false) + error;
      }

      break;

    case 1:
      error = z * sqrt((this->getAllocatedProbability(true) *
                        this->getBlockingProbability(true)) /
                           this->numberOfConnections +
                       4);

      if (inf == true) {
        result = this->getBlockingProbability(true) - error;
      }

      else {
        result = this->getBlockingProbability(true) + error;
      }
      break;
  }

  return result;
}

double Simulator::confidenceInterval2(double confidence, bool inf, int type) {
  /* Type:
   * 0 = Wald
   * 1 = Agresti-Coull
   * 2 = Wilson
   * 3 = Clopper-Pearson
   * 4 = Bayesian
   */

  if (confidence <= 0 || confidence >= 1) {
    throw std::runtime_error(
        "You can't set a confidence interval with confidence equal/higher than "
        "1 or equal/lower than 0.");
  }

  // float p = this->getBlockingProbability(type);
  // float np = 1 - p;

  float np = this->getAllocatedProbability(type);
  float p = 1 - np;

  float z = 1.96;
  float sd = sqrt((np * p) / (this->numberOfConnections + type * 4));

  if (inf) return p - (z * sd);
  return p + (z * sd);
}

///////////////////////////////////////////////////////////////////////////////////

double Simulator::confidenceInterval3(double confidence, bool inf, int type) {
  /* Type:
   * 0 = Wald
   * 1 = Agresti-Coull
   * 2 = Wilson
   * 3 = Clopper-Pearson
   * 4 = Bayesian
   */

  if (confidence <= 0 || confidence >= 1) {
    throw std::runtime_error(
        "You can't set a confidence interval with confidence equal/higher than "
        "1 or equal/lower than 0.");
  }
  float p, np, sd;
  float z = 1.96;
  // Si funciona bien, se pueden simplificar las funciones
  // getBlocking/getAllocated Prob
  switch (type) {
    case 0:  // Wald
    case 1:  // Agresti-Coul
    case 2:  // Wilson
      np = this->getAllocatedProbability(false);

      // Agresti-Coul = agrega dos observaciones a cada estado
      if (type == 1) {
        np = np *
             ((this->numberOfConnections * (this->allocatedConnections + 2)) /
              (this->allocatedConnections * (this->numberOfConnections + 4)));
      }

      p = 1 - np;

      sd = sqrt((np * p) / (this->numberOfConnections + type * 4));

      if (type == 2) {
        float denom = (1 + ((z * z) / this->numberOfConnections));
        // sd = sqrt()
      }
  }

  if (inf) return p - (z * sd);
  return p + (z * sd);
}

double Simulator::confidenceValue(double alpha) {
  if (alpha <= 0 || alpha >= 1) {
    throw std::runtime_error(
        "You can't set a confidence interval with confidence equal/higher than "
        "1 or equal/lower than 0.");
  }

  return 1.96;
}

double Simulator::waldCI(double confidence, bool lower) {
  double np = this->getAllocatedProbability(false);
  double p = 1 - np;
  int n = this->numberOfConnections;
  double sd = sqrt((np * p) / n);

  if (lower) return p - (confidence * sd);
  return p + (confidence * sd);
}

double Simulator::agrestiCI(double confidence, bool lower) {
  double np = this->getAllocatedProbability(false);
  int n = this->numberOfConnections;

  np = np * ((n * (this->allocatedConnections + 2)) /
             (this->allocatedConnections * (n + 4)));

  double p = 1 - np;
  double sd = sqrt((np * p) / (n + 4));

  if (lower) return p - (confidence * sd);
  return p + (confidence * sd);
}

double Simulator::wilsonCI(double confidence, bool lower) {
  double np = this->getAllocatedProbability(false);
  double p = 1 - np;
  int n = this->numberOfConnections;

  double denom = (1 + (pow(confidence, 2) / n));

  double k = p + pow(confidence, 2) / (2 * n);
  double sd = sqrt(((np * p) / n) + ((pow(confidence, 2)) / (4 * pow(n, 2))));

  if (lower) return (k - (confidence * sd)) / denom;
  return (k + (confidence * sd)) / denom;
}

double Simulator::clopperCI(double confidence, bool lower) {
  int n = this->numberOfConnections;
  int r = n - this->allocatedConnections;  // Numero conexiones rechazadas
  double m1 = 2 * (n - r + 1);
  double m2 = 2 * r;
  std::fisher_f_distribution<> distribution(m1, m2);

  double number;
  if (lower) {
    // number = distribution(1.0 - confidence / 2);
    // return r / (r + (n - r + 1) * distribution(1 - confidence / 2));
  }
  return 1.0;
}
/*
// Fail
double Simulator::confidenceInterval4(double confidence, bool inf, int type) {
  /* Type:
   * 0 = Wald
   * 1 = Agresti-Coull
   * 2 = Wilson
   * 3 = Clopper-Pearson
   * 4 = Bayesian


if (confidence <= 0 || confidence >= 1) {
  throw std::runtime_error(
      "You can't set a confidence interval with confidence equal/higher than "
      "1 or equal/lower than 0.");
}

// Si funciona bien, se pueden simplificar las funciones
// getBlocking/getAllocated Prob
float p = this->getBlockingProbability(false);

// Agresti-Coul = agrega dos observaciones a cada estado
if (type == 1) {
  p = p * ((this->numberOfConnections *
            (this->numberOfConnections - this->allocatedConnections + 6)) /
           ((this->numberOfConnections - this->allocatedConnections) *
            (this->numberOfConnections + 4)));
}

float np = 1 - p;
float z = 1.96;
float sd = sqrt((np * p) / (this->numberOfConnections + type * 4));

if (inf) return p - (z * sd);
return p + (z * sd);
}

double Simulator::infCI(double confidence, int type) {
  if (confidence <= 0 || confidence >= 1) {
    throw std::runtime_error(
        "You can't set a confidence interval with confidence equal/higher than "
        "1 or equal/lower than 0.");
  }

  // double significance = 1 - confidence; //Para después
  float z = 1.96;
  float error =
      z *
      sqrt((this->getAllocatedProbability() * this->getBlockingProbability()) /
           this->numberOfConnections);

  return this->getAllocatedProbability() - error;
}

double Simulator::supCI(double confidence) {
  if (confidence <= 0 || confidence >= 1) {
    throw std::runtime_error(
        "You can't set a confidence interval with confidence equal/higher than "
        "1 or equal/lower than 0.");
  }

  // double significance = 1 - confidence; //Para después
  float z = 1.96;
  float error =
      z *
      sqrt((this->getAllocatedProbability() * this->getBlockingProbability()) /
           this->numberOfConnections);

  return this->getAllocatedProbability() + error;
}
*/