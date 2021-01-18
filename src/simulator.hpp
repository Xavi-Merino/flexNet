#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <list>

#include "controller.hpp"
#include "event.hpp"
#include "exp_variable.hpp"
#include "uniform_variable.hpp"

class Simulator {
 public:
  Simulator(void);
  Simulator(std::string networkFilename, std::string pathFilename);
  void init(void);
  void run(void);

  void setLambda(double lambda);
  void setMu(double mu);
  void setSeedArrive(unsigned int seed);
  void setSeedDeparture(double seed);
  void setGoalConnections(long long goal);
  void setBitRates(std::vector<double> bitRates);
  void defaultValues();

 private:
  double clock;
  std::list<Event> events;
  ExpVariable arriveVariable;
  ExpVariable departVariable;
  UniformVariable srcVariable;
  UniformVariable dstVariable;
  Controller controller;
  Event currentEvent;
  std::vector<double> bitRates;

  double lambda;
  double mu;
  unsigned int seedArrive;
  unsigned int seedDeparture;
  unsigned int seedSrc;
  unsigned int seedDst;
  long long numberOfConnections;
  long long goalConnections;
  double nextEventTime;
  int rtnAllocation;
  int src, dst;
  std::vector<double> bitRatesDefault;

  int eventRoutine(void);
};

#endif