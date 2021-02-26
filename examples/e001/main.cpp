/*
This is an allocation algorithm that follows the First Fit allocation policy,
which always chooses the available slots with the lowest posible index to serve
the connection request. If the slot or group of slots required to make the
connection is available, taking into account the spectrum contiguity and
continuity constraints, it creates the connections and returns 'ALLOCATED' to
indicate success; otherwise, it returns 'NOT_ALLOCATED' to indicate that the
process failed
*/
#include <fnsim/simulator.hpp>

BEGIN_ALLOC_FUNCTION(FirstFit) {
  int numberOfSlots = REQ_SLOTS(0);
  int currentNumberSlots;
  int currentSlotIndex;
  std::vector<bool> totalSlots =
      std::vector<bool>(LINK_IN_ROUTE(0, 0)->getSlots(), false);
  for (int i = 0; i < NUMBER_OF_ROUTES;
       i++) {  // this loops through the routes between the source and
               // destination node
    for (int j = 0; j < NUMBER_OF_LINKS(i);
         j++) {  // this loops through the links that the current route contains
      for (int k = 0; k < LINK_IN_ROUTE(i, j)->getSlots();
           k++) {  // this loops through the slots on the current link to fill
                   // the total slots vector with the slot status information
        totalSlots[k] = totalSlots[k] | LINK_IN_ROUTE(i, j)->getSlot(k);
      }
    }
    currentNumberSlots = 0;
    currentSlotIndex = 0;
    for (int j = 0; j < totalSlots.size();
         j++) {  // this loops through the totalSlots vector to find if the
                 // required amount of continous slots is available on all
                 // the links of the current route. If the value of a position
                 // on the vector is false, the corresponding slot is available
                 // on every link on the route; otherwise, it's unavailable
      if (totalSlots[j] == false) {
        currentNumberSlots++;
      } else {
        currentNumberSlots = 0;
        currentSlotIndex = j + 1;
      }
      if (currentNumberSlots == numberOfSlots) {
        for (int j = 0; j < NUMBER_OF_LINKS(i);
             j++) {  // if the necessary amount of consecutive slots was
                     // available, this loops through the links on that route to
                     // create the connections
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
  Simulator sim =
      Simulator(std::string("NSFNet.json"),
                std::string("routes.json"));  // this creates the simulator
                                              // object using the JSON files
                                              // that contain the network
                                              // information
  USE_ALLOC_FUNCTION(FirstFit, sim);
  sim.init();
  sim.run();

  return 0;
}
