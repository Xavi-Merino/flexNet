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
#define REQ_REACH(pos) bitRate.getReach(pos)
#define REQ_MODULATION(pos) bitRate.getModulation(pos)
#define REQ_BITRATE_STR bitRate.getBitRateStr()
#define REQ_BITRATE bitRate.getBitRate()
#define LINK_IN_ROUTE(route, link) (*this->path)[src][dst][route][link]
#define LINK_IN_ROUTE_SRC(route, link) \
  (*this->path)[src][dst][route][link]->getSrc()
#define LINK_IN_ROUTE_DST(route, link) \
  (*this->path)[src][dst][route][link]->getDst()
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
/**
 * @brief Class Simulator, represents network execution.
 */
class Simulator {
 public:
  /**
   * @brief Construct a new Simulator object.
   */
  Simulator(void);
  /**
   * @brief Construct the object Simulator from two JSON files, this files
   * contains the network configuration and the routes.
   *
   * @param networkFilename Source of network file. This file is the
   * configuration of the network, nodes information (id, destiny, source,
   * lenght, slots).
   * @param pathFilename Source of path file. This file contains the routes
   * between nodes.
   */
  Simulator(std::string networkFilename, std::string pathFilename);
  /**
   * @brief Construct the object Simulator from three JSON files, this files
   * contains the network configuration the routes and the bit rates.
   *
   * @param networkFilename Source of network file. This file is the
   * configuration of the network, nodes information (id, destiny, source,
   * lenght, slots).
   * @param pathFilename Source of path file. This file contains the routes
   * between nodes.
   * @param bitrateFilename Source of bit rates file. This file contains the
   * differents bit rates configurations.
   */
  Simulator(std::string networkFilename, std::string pathFilename,
            std::string bitrateFilename);
  /**
   * @brief Deletes the object Simulator.
   */
  ~Simulator();
  /**
   * @brief Saves the simulator processes to run and leaves them ready to start,
   * does not start them.
   */
  void init(void);
  /**
   * @brief Start the simulator processes.
   */
  void run(void);
  /**
   * @brief Assign the lambda for simulation.
   *
   * @param lambda Param type double.
   */
  void setLambda(double lambda);
  /**
   * @brief Assign the mu for simulation.
   *
   * @param mu Param type doble.
   */
  void setMu(double mu);
  /**
   * @brief Set the seed for the arrives. The aleatory arrives are generated
   * from these seeds.
   *
   * @param seed Param type unsigned integer.
   */
  void setSeedArrive(unsigned int seed);
  /**
   * @brief Set the seed for the departures. The aleatory departures are
   * generated from these seeds.
   *
   * @param seed param type unsigned integer.
   */
  void setSeedDeparture(unsigned int seed);
  /**
   * @brief Set the seed for the bit rates. The aleatory bit rates are generated
   * from these seeds.
   *
   * @param seed Param type unsigned integer.
   */
  void setSeedBitRate(unsigned int seed);
  /**
   * @brief Set connections goal.
   *
   * @param goal Param type long long. Represents the total number of
   * connections to be simulated.
   */
  void setGoalConnections(long long goal);
  /**
   * @brief Set type of Bit Rates.
   *
   * @param bitRates Array of bitrate objects.
   */
  void setBitRates(std::vector<BitRate> bitRates);
  /**
   * @brief
   *
   * @param newAllocator Pointer of object type Allocator.
   */
  void setAllocator(Allocator *newAllocator);
  /**
   * @brief Get the Time Duration object, that corresponds to the simulation
   * time.
   *
   * @return Unsigned int the number of seconds that the simulation was running.
   */
  unsigned int getTimeDuration();
  /**
   * @brief Get the Blocking Probability of the simulation
   *
   * @return Double The blocking probability calculated as 1 - (allocated /
   * total connections)
   */
  double getBlockingProbability();

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

  bool initReady;
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
  /**
   * @brief Takes the event and executes it.
   *
   * @return The status of the allocation. It can be ALLOCATED,
   * NOT_ALLOCATED, N_A (not assigned )
   */
  int eventRoutine(void);
  /**
   * @brief Set the default values for the different fields:
   * lambda = 3
   * mu = 10
   * seedArrive = 12345
   * seedDeparture = 12345
   * seedSrc = 12345
   * seedDst = 12345
   * seedBitRate = 12345
   * numberOfConnections = 0
   * numberOfEvents = 0
   * goalConnections = 10000
   * columnWidth = 10
   */
  void defaultValues();
  /**
   * @brief Shows on screen the values of the current simulator configuration.
   */
  void printInitialInfo();
  /**
   * @brief Shows on screen the percentage of completion of the simulation.
   */
  void printRow(double percentage);
};

#endif