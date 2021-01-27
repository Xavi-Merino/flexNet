#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#define BEGIN_ALLOC_FUNCTION(name__)                                  \
  class f_##name__ : public Allocator {                               \
   public:                                                            \
    f_##name__() : Allocator() { this->name = std::string(#name__); } \
    allocationStatus exec(int src, int dst, BitRate bitRate, Connection &con)
#define END_ALLOC_FUNCTION \
  }                        \
  ;

#define USE_ALLOC_FUNCTION(fun, simObject) simObject.setAllocator(new f_##fun);
#define SRC src
#define DST dst
#define REQ_SLOTS(pos) bitRate.getNumberOfSlots(pos)
#define LINK_IN_ROUTE(route, link) (*this->path)[src][dst][route][link]
#define LINK_IN_ROUTE_ID(route, link) \
  (*this->path)[src][dst][route][link]->getId()
#define NUMBER_OF_ROUTES (*this->path)[src][dst].size()
#define NUMBER_OF_LINKS(route) (*this->path)[src][dst][route].size()
#define ALLOC_SLOTS(link, from, to) con.addLink(link, from, from + to);

#include <chrono>
#include <iomanip>
#include <list>

#include "controller.hpp"
#include "event.hpp"
#include "exp_variable.hpp"
#include "uniform_variable.hpp"

class Simulator {
 public:
  Simulator(void);
  Simulator(std::string networkFilename, std::string pathFilename);
  ~Simulator();
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
  void printInitialInfo();
  void printRow(double percentage);

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
  long long numberOfEvents;
  long long goalConnections;
  double nextEventTime;
  allocationStatus rtnAllocation;
  int src, dst, bitRate;
  std::vector<BitRate> bitRatesDefault;
  double allocatedConnections;
  double blockingProbability;
  int columnWidth;
  std::chrono::high_resolution_clock::time_point startingTime;
  std::chrono::high_resolution_clock::time_point checkTime;
  std::chrono::duration<double> timeDuration;

  int eventRoutine(void);
};

#endif