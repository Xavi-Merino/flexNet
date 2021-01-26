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
  void setSeedDeparture(unsigned int seed);
  void setSeedBitRate(unsigned int seed);
  void setGoalConnections(long long goal);
  void setBitRates(std::vector<BitRate> bitRates);
  void setAllocator(Allocator *newAllocator);
  void defaultValues();

 private:
  double clock;
  std::list<Event> events;
  ExpVariable arriveVariable;
  ExpVariable departVariable;
  UniformVariable srcVariable;
  UniformVariable dstVariable;
  UniformVariable bitRateVariable;
  Controller *controller;
  Event currentEvent;
  std::vector<BitRate> bitRates;

  double lambda;
  double mu;
  unsigned int seedArrive;
  unsigned int seedDeparture;
  unsigned int seedSrc;
  unsigned int seedDst;
  unsigned int seedBitRate;
  long long numberOfConnections;
  long long goalConnections;
  double nextEventTime;
  allocationStatus rtnAllocation;
  int src, dst, bitRate;
  std::vector<BitRate> bitRatesDefault;
  double allocatedConnections;
  double blockingProbability;

  int eventRoutine(void);
};

#endif