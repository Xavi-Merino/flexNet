#include "../src/simulator.hpp"

BEGIN_ALLOC_FUNCTION(FirstFit) {
  int numberOfSlots = REQ_SLOTS(0);
  int currentNumberSlots;
  int currentSlotIndex;
  std::vector<bool> totalSlots =
      std::vector<bool>(LINK_IN_ROUTE(0, 0)->getSlots(), false);
  for (int i = 0; i < NUMBER_OF_ROUTES; i++) {
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
        currentNumberSlots = 0;
        currentSlotIndex = j + 1;
      }
      if (currentNumberSlots == numberOfSlots) {
        for (int j = 0; j < NUMBER_OF_LINKS(i); j++) {
          ALLOC_SLOTS(LINK_IN_ROUTE_ID(i, j), currentSlotIndex, numberOfSlots)
        }
        return ALLOCATED;
      }
    }
  }
  return NOT_ALLOCATED;
}
END_ALLOC_FUNCTION

int main(int argc, char* argv[]) {
  Simulator sim = Simulator(std::string("../networks/NSFNet.json"),
                            std::string("../networks/NSFNet_routes.json"));
  USE_ALLOC_FUNCTION(FirstFit, sim);
  sim.init();
  sim.run();

  return 0;
}
