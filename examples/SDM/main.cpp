#include "./src/utils.cpp"


// ############################## Global Variables #################################

Controller *SDM_controller;
Simulator sim;

// #################################################################################

// Allocation function
BEGIN_ALLOC_FUNCTION(FirstFit) {

  int currentNumberSlots;
  int currentSlotIndex;
  int bitRateInt = bitRates_map[REQ_BITRATE];
  int numberOfSlots;
  std::vector<bool> totalSlots;

  // Bitrate count
  bitrate_count_total[bitRateInt] += 1;

  for (int r = 0; r < NUMBER_OF_ROUTES;
        r++){ // <- For route r between current SRC and DST
    totalSlots = std::vector<bool>(LINK_IN_ROUTE(r, 0)->getSlots(), false); // <- Assuming all links have the same capacity
    for (int m = 0; m < NUMBER_OF_MODULATIONS;
        m++){ // <- For modulation m
        numberOfSlots = REQ_SLOTS(m); //<- Number of slots that this modulation format requires
        double route_length = 0; // For checking route length
        for (int l = 0; l < NUMBER_OF_LINKS(r);
            l++){ // <- this loops through the links that the current route contains

          route_length += LINK_IN_ROUTE(r,l)->getLength();

          for (int s = 0; s < LINK_IN_ROUTE(r, l)->getSlots();
              s++){   // <- this loops through the slots on the current link to fill
                      //    the total slots vector with the slot status information
            totalSlots[s] = totalSlots[s] | LINK_IN_ROUTE(r, l)->getSlot(s);
            }
          }
          // We verify that the current modulation format has sufficient reach
          if (route_length > REQ_REACH(m))
          {
            // std::cout << "Not reaching!\n";
            continue;
          }
          // Number of consecutive free slots:
          currentNumberSlots = 0;
          currentSlotIndex = 0;
          for (int s = 0; s < totalSlots.size();
              s++) {  // this loops through the totalSlots vector to find if the
                      // required amount of continous slots is available on all
                      // the links of the current route. If the value of a position
                      // on the vector is false, the corresponding slot is available
                      // on every link on the route; otherwise, it's unavailable
              if (totalSlots[s] == false) {
                currentNumberSlots++;
              } else {
                currentNumberSlots = 0;
                currentSlotIndex = s + 1;
              }
              if (currentNumberSlots == numberOfSlots) {
                for (int l = 0; l < NUMBER_OF_LINKS(r);
                    l++) {  // if the necessary amount of consecutive slots was
                            // available, this loops through the links on that route to
                            // create the connections
                  ALLOC_SLOTS(LINK_IN_ROUTE_ID(r, l), currentSlotIndex, numberOfSlots)
                }
                return ALLOCATED;
              }
          }
      }
    }
    bitrate_count_blocked[bitRateInt] += 1;
    return NOT_ALLOCATED;
}
END_ALLOC_FUNCTION

// Unalloc callback function
BEGIN_UNALLOC_CALLBACK_FUNCTION {

}
END_UNALLOC_CALLBACK_FUNCTION

int main(int argc, char* argv[]) {

  // Sim parameters
  double  lambdas[31] = {36, 72, 108, 144, 180, 216, 252, 288, 324, 360, 396, 432, 468, 504, 540, 576, 612, 648, 684, 720, 756, 792, 828, 864, 900, 936, 972, 1008, 1044, 1080, 1016};
  double mu = 1;
  int number_connections = 1e7;

  // ############################## USNet #################################
  for (int lambda = 0; lambda < 1; lambda++) {

    // Simulator object
    sim = Simulator(std::string("./networks/USNet.json"),                      // Network nodes, links and cores
                    std::string("./networks/USNet_routes.json"),               // Network Routes
                    std::string("./networks/bitrates.json"));                  // BitRates (eg. BPSK)

    // Assign alloc function   
    USE_ALLOC_FUNCTION(FirstFit, sim);
    // Assign unalloc function
    USE_UNALLOC_FUNCTION(sim);

    // Assign parameters
    sim.setGoalConnections(number_connections);
    sim.setLambda(lambdas[lambda]);
    sim.setMu(mu);
    sim.init();

    // Set controller accessible for unalloc function
    SDM_controller = sim.getController();

    // Begin simulation
    sim.run();

    // BBP calculation and output results
    double BBP_results;
    std::fstream output;
    BBP_results = bandwidthBlockingProbability(bitrate_count_total, bitrate_count_blocked, mean_weight_bitrate, false);
    output.open("./out/SDM-USNet-1e6.txt", std::ios::out | std::ios::app);

    resultsToFile(output, BBP_results, sim.getBlockingProbability(), number_connections,
                  lambda, lambdas[lambda], bitrate_count_blocked);

    // Reset global variables
    for (int b = 0; b < 5; b++){
      bitrate_count_total[b] = 0.0;
      bitrate_count_blocked[b] = 0.0;
    }
  }

  return 0;
}