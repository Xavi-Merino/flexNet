#define CATCH_CONFIG_MAIN

#include "../src/bitrate.hpp"
#include "../src/simulator.hpp"
#include "catch.hpp"

BEGIN_ALLOC_FUNCTION(macros) {
  REQ_SLOTS(0);
  REQ_REACH(0);
  REQ_MODULATION(0);
  REQ_BITRATE_STR;
  REQ_BITRATE;
  return ALLOCATED;
}
END_ALLOC_FUNCTION

BEGIN_ALLOC_FUNCTION(ExactFit) {
  int numberOfSlots = REQ_SLOTS(0);
  int currentNumberSlots;
  int currentSlotIndex;
  int firstIndex;
  std::vector<bool> totalSlots =
      std::vector<bool>(LINK_IN_ROUTE(0, 0)->getSlots(), false);
  for (int i = 0; i < NUMBER_OF_ROUTES; i++) {
    firstIndex = -1;
    for (int j = 0; j < NUMBER_OF_LINKS(i); j++) {
      for (int k = 0; k < LINK_IN_ROUTE(i, j)->getSlots(); k++) {
        totalSlots[k] = totalSlots[k] | LINK_IN_ROUTE(i, j)->getSlot(k);
      }
    }
    currentNumberSlots = 0;
    currentSlotIndex = 0;
    for (int j = 0; j < totalSlots.size(); j++) {
      if (totalSlots[j] == false) {
        currentNumberSlots++;
      } else {
        if (currentNumberSlots == numberOfSlots) {
          for (int j = 0; j < NUMBER_OF_LINKS(i); j++) {
            ALLOC_SLOTS(LINK_IN_ROUTE_ID(i, j), currentSlotIndex, numberOfSlots)
          }
          return ALLOCATED;
        }
        currentNumberSlots = 0;
        currentSlotIndex = j + 1;
      }
      if (firstIndex == -1 && currentNumberSlots > numberOfSlots) {
        firstIndex = currentSlotIndex;
      }
    }
    if (firstIndex != -1) {
      for (int j = 0; j < NUMBER_OF_LINKS(i); j++) {
        ALLOC_SLOTS(LINK_IN_ROUTE_ID(i, j), firstIndex, numberOfSlots)
      }
      return ALLOCATED;
    }
  }
  return NOT_ALLOCATED;
}
END_ALLOC_FUNCTION

TEST_CASE("MACROS with FF andd 100 simulations (Simulator)") {
  Simulator s = Simulator(std::string("../networks/NSFNet.json"),
                          std::string("../networks/NSFNet_routes.json"),
                          std::string("../bitrate/bitrate.json"));
  USE_ALLOC_FUNCTION(macros, s);
  s.init();
  CHECK_NOTHROW(s.run());
}

TEST_CASE("Algorithm - metrics (Simulator)") {
  Simulator s = Simulator(std::string("../networks/NSFNet.json"),
                          std::string("../networks/NSFNet_routes.json"),
                          std::string("../bitrate/bitrate.json"));
  USE_ALLOC_FUNCTION(ExactFit, s);
  s.init();
  s.run();

  CHECK_NOTHROW(s.getBlockingProbability());
  CHECK_NOTHROW(s.getTimeDuration());
}

TEST_CASE("Constructor - (Simulator)") {
  CHECK_NOTHROW(Simulator());
  CHECK_NOTHROW(
      Simulator("../networks/NSFNet.json", "../networks/NSFNet_routes.json"));
  CHECK_NOTHROW(Simulator("../networks/NSFNet.json",
                          "../networks/NSFNet_routes.json",
                          "../bitrate/bitrate.json"));
}

TEST_CASE("Set lambda") {
  Simulator s = Simulator();
  double lambda = 8.16;
  CHECK_NOTHROW(s.setLambda(lambda));
}

TEST_CASE("Set mu") {
  Simulator s = Simulator();
  double mu = 4.3;
  CHECK_NOTHROW(s.setMu(mu));
}

TEST_CASE("Set seed arrive") {
  Simulator s = Simulator();
  unsigned int seed = 32;
  CHECK_NOTHROW(s.setSeedArrive(seed));
}

TEST_CASE("Set seed departure") {
  Simulator s = Simulator();
  unsigned int seed = 32;
  CHECK_NOTHROW(s.setSeedDeparture(seed));
}

TEST_CASE("Set seed bit rate") {
  Simulator s = Simulator();
  unsigned int seed = 32;
  CHECK_NOTHROW(s.setSeedDeparture(seed));
}

TEST_CASE("Set goal connections") {
  Simulator s = Simulator();
  long long goalConnections = 1e7;
  CHECK_NOTHROW(s.setGoalConnections(goalConnections));
}

TEST_CASE("Set bit rates") {
  Simulator s = Simulator();
  std::vector<BitRate> netBitRates = std::vector<BitRate>();
  netBitRates = BitRate::readBitRateFile("../bitrate/bitrate.json");
  CHECK_NOTHROW(s.setBitRates(netBitRates));
}

TEST_CASE("Set allocator") {
  Network net = Network();
  Allocator aloc = Allocator(&net);
  Simulator s = Simulator();
  CHECK_NOTHROW(s.setAllocator(&aloc));
}

TEST_CASE("Uniform Variable Tests") {
  CHECK_THROWS(UniformVariable(12345, -5.0));

  UniformVariable uniformExample(12345, 5.0);
  bool flag = true;
  int value;
  for (int i = 0; i < 1000; i++) {
    value = uniformExample.getNextValue();
    if (value < 0 || value > 5) {
      flag = false;
      break;
    }
  }
  CHECK(flag);
}
