#include <fnsim/simulator.hpp>

BEGIN_ALLOC_FUNCTION(ExactFit) {
  int numberOfSlots = REQ_SLOTS(0);
  int currentNumberSlots;
  int currentSlotIndex;
  int firstIndex;
  std::vector<bool> totalSlots;
  for (int i = 0; i < NUMBER_OF_ROUTES; i++) {
    totalSlots = std::vector<bool>(LINK_IN_ROUTE(0, 0)->getSlots(), false);
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

int main(int argc, char* argv[]) {
  Simulator sim =
      Simulator(std::string("NSFNet.json"), std::string("routes.json"),
                std::string("bitrates.json"));
  USE_ALLOC_FUNCTION(ExactFit, sim);
  sim.init();
  sim.run();

  return 0;
}
