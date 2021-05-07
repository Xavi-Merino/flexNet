#define CATCH_CONFIG_MAIN

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