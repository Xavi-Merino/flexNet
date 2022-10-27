#include "./src/offline_approach.cpp"
/*********************************************************************************
 * This is an allocation algorithm that follows the First Fit allocation
 * policy and include the implementation of a infinite buffer that stores blocked
 * connections and try to allocate them every time a departure occurs, also using
 * First Fit. This policy always chooses the available slots with the lowest posible
 * index to serve the connection request. If the required slot or group of slots
 * is available, taking into account the spectrum contiguity and continuity
 * constraints, it creates the connections and returns 'ALLOCATED' to indicate
 * success; otherwise, it returns 'NOT_ALLOCATED' to indicate that the process
 * failed and add it to the buffer (or keep in it if allocating from buffer)
 **********************************************************************************/


// ############################## Global Variables #################################

// Number of connections
int number_connections = 1e7;

// Queue for buffer
Buffer buffer;

// Vector that stores routes in order defined by the user in offlineApproachOrder()
std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline;

// BBP global variables
double bitrate_count_total[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
double bitrate_count_blocked[5] = {0.0, 0.0, 0.0, 0.0, 0.0};

// Buffer state
bool buffer_state = true;
bool allocating_from_buffer = false;

// Weight C+L+S+E:
// double mean_weight_bitrate[5] = {1.0, 1.83, 3.5, 13.33, 32.83};
// Weight Only C:
double mean_weight_bitrate[5] = {1.0, 1.25, 3.0, 9.5, 23};

// Controller and Simulator for acces from UNALLOC
Controller *buffer_controller;
Simulator sim;

// debug
// std::fstream outputTEST;

// Variables for output of the time every connection was allocated from buffer
std::fstream realloc_time;
const char* fileName[50] = {"none",
                    "./Rout/EUROCORE_RSArealloc1.txt","./Rout/EUROCORE_RSArealloc2.txt","./Rout/EUROCORE_RSArealloc3.txt","./Rout/EUROCORE_RSArealloc4.txt","./Rout/EUROCORE_RSArealloc5.txt","./Rout/EUROCORE_RSArealloc6.txt","./Rout/EUROCORE_RSArealloc7.txt","./Rout/EUROCORE_RSArealloc8.txt",
                    "./Rout/EUROCORE_RSArealloc9.txt","./Rout/EUROCORE_RSArealloc10.txt","./Rout/EUROCORE_RSArealloc11.txt","./Rout/EUROCORE_RSArealloc12.txt","./Rout/EUROCORE_RSArealloc13.txt","./Rout/EUROCORE_RSArealloc14.txt","./Rout/EUROCORE_RSArealloc15.txt","./Rout/EUROCORE_RSArealloc16.txt",
                    "./Rout/EUROCORE_RSArealloc17.txt","./Rout/EUROCORE_RSArealloc18.txt","./Rout/EUROCORE_RSArealloc19.txt","./Rout/EUROCORE_RSArealloc20.txt","./Rout/EUROCORE_RSArealloc21.txt","./Rout/EUROCORE_RSArealloc22.txt","./Rout/EUROCORE_RSArealloc23.txt","./Rout/EUROCORE_RSArealloc24.txt",
                    "./Rout/EUROCORE_RSArealloc25.txt","./Rout/EUROCORE_RSArealloc26.txt","./Rout/EUROCORE_RSArealloc27.txt","./Rout/EUROCORE_RSArealloc28.txt","./Rout/EUROCORE_RSArealloc29.txt","./Rout/EUROCORE_RSArealloc30.txt","./Rout/EUROCORE_RSArealloc31.txt","./Rout/EUROCORE_RSArealloc32.txt",
                    "./Rout/EUROCORE_RSArealloc33.txt","./Rout/EUROCORE_RSArealloc34.txt","./Rout/EUROCORE_RSArealloc35.txt","./Rout/EUROCORE_RSArealloc36.txt","./Rout/EUROCORE_RSArealloc37.txt","./Rout/EUROCORE_RSArealloc38.txt","./Rout/EUROCORE_RSArealloc39.txt","./Rout/EUROCORE_RSArealloc40.txt",
                    "./Rout/EUROCORE_RSArealloc41.txt","./Rout/EUROCORE_RSArealloc42.txt","./Rout/EUROCORE_RSArealloc43.txt","./Rout/EUROCORE_RSArealloc44.txt","./Rout/EUROCORE_RSArealloc45.txt","./Rout/EUROCORE_RSArealloc46.txt","./Rout/EUROCORE_RSArealloc47.txt","./Rout/EUROCORE_RSArealloc48.txt",
                    "./Rout/EUROCORE_RSArealloc49.txt"};

// #################################################################################

// Allocation function
BEGIN_ALLOC_FUNCTION(FirstFits) {

  // Calculate avg buffer size
  if (buffer_state){
    buffer.mean_size_time += buffer.size()*(con.getTimeConnection() -  buffer.last_time);
    buffer.last_time = con.getTimeConnection();
  }

  int currentNumberSlots;
  int currentSlotIndex;
  int *band_slot_indexes = NULL;
  int bitRateInt = bitRates_map[REQ_BITRATE];
  int numberOfSlots;


  if (!allocating_from_buffer) bitrate_count_total[bitRateInt] += 1;

  std::vector<AuxRoute *> *pathsRBMSA_reqBitRate = &pathsOffline[bitRateInt][SRC][DST];
  std::vector<bool> totalSlots;

  for (int r = 0; r < (*pathsRBMSA_reqBitRate).size();
      r++){ // <- For route r between SRC and DST
    // Number of slots that this route requires:
    numberOfSlots = (*pathsRBMSA_reqBitRate)[r]->getSlots();
    // Boolean vector of size of the first link of the route:
    totalSlots = std::vector<bool>((*pathsRBMSA_reqBitRate)[r]->getLinks()[0]->getSlots(), false);

    // This function sets the 'band_slot_indexes' array with the values of the indices that correspond to the current
    // band in use by the route r:
    // i.e. band_slot_indexes[0] =  index value of the first slot of the band
    //      band_slot_indexes[1] =  index value of the last slot of the band
    //      band_slot_indexes[3] =  total band slot capacity
    if (band_slot_indexes != NULL) delete(band_slot_indexes);
    bandSliceParameters((*pathsRBMSA_reqBitRate)[r]->getLinks()[0]->getSlots(),
                        (*pathsRBMSA_reqBitRate)[r]->getBandInt(),
                        &band_slot_indexes);

    for (int l = 0; l < (*pathsRBMSA_reqBitRate)[r]->getLinks().size();
        l++){ // <- For link l in route r
      for (int s = band_slot_indexes[0]; s <= band_slot_indexes[1];
          s++){ // <- For each slot s in the current band, of the link l
        
        // We verify the status of the slot s in every link l of the route r
        totalSlots[s] = totalSlots[s] | (*pathsRBMSA_reqBitRate)[r]->getLinks()[l]->getSlot(s);
      }
    }

    // Number of consecutive free slots:
    currentNumberSlots = 0;
    // Current index in 'totalSlots' vector (we set band_slot_indexes[0] as begining)
    currentSlotIndex = band_slot_indexes[0];
    for (int s = band_slot_indexes[0]; s <= band_slot_indexes[1];
        s++){
      if (totalSlots[s] == false) currentNumberSlots++;
      else {
        currentNumberSlots = 0;
        currentSlotIndex = s + 1;
    }

      if (currentNumberSlots == numberOfSlots) {
        for (int l = 0; l < (*pathsRBMSA_reqBitRate)[r]->getLinks().size();
            l++){
          ALLOC_SLOTS((*pathsRBMSA_reqBitRate)[r]->getLinks()[l]->getId(), currentSlotIndex, numberOfSlots)
        }

        // Clear band_slot_indexes
        if (band_slot_indexes != NULL) delete(band_slot_indexes);
        //if (allocating_from_buffer)
        //  std::cout << "BITRATE: "<< REQ_BITRATE << ", Source: " << SRC << ", Destination: " << DST << ", Route: " << r << ", Desde slots: " << currentSlotIndex << " al " << currentSlotIndex+numberOfSlots-1 << '\n';
        return ALLOCATED;
      }      
    }
  }
  // N/Buffer BBP calculation
  bitrate_count_blocked[bitRateInt] += 1;

  // Clear band_slot_indexes
  if (band_slot_indexes != NULL) delete(band_slot_indexes);

  if (buffer_state){
    // If the present connection ISN'T coming from buffer, we push to queue
    if (!allocating_from_buffer){
      buffer.addElement(buffer_element(SRC, DST, con.getId(), con.getBitrate(), con.getTimeConnection()));
    }
    // If the present connection IS coming from buffer, add another attempt
    else {
      buffer.front()->current_attempts++;
    }
  }

  return NOT_ALLOCATED;
}
END_ALLOC_FUNCTION

// Unalloc callback function
BEGIN_UNALLOC_CALLBACK_FUNCTION {
  if (buffer.size() > 0){

    // For simplicity
    buffer_element *front_queue = buffer.front();

    // Let the alloc function know we are allocating from buffer
    allocating_from_buffer = true;

    // try to alloc
    if (buffer_controller->assignConnection(front_queue->src, front_queue->dst, *(front_queue->bitRate), front_queue->id, t) == ALLOCATED){

      // Add departure to event routine
      sim.addDepartureEvent(front_queue->id);

      // Total time the connection was in queue
      buffer.mean_service_time += t - front_queue->time_arrival;

      // We keep track of how many times attempted to be allocated from buffer
      buffer.mean_attempts += buffer.front()->current_attempts;

      // We output the currrent time being allocated
      realloc_time << t << "\n";
      //std::cout << t << "\n";
      
      // Element allocated so we poped it and delete() members
      delete(buffer.front()->bitRate);
      buffer.pop_front();

      // Keep count of how many connections where allocated from the buffer
      buffer.poped++;
      // std::cout << "Allocated!\n";   
    }

    // Not allocating from buffer anymore
    allocating_from_buffer = false;
  }
}
END_UNALLOC_CALLBACK_FUNCTION



int main(int argc, char* argv[]) {

  // debug
  //outputTEST.open("./out/outputTEST.txt", std::ios::out | std::ios::app);

  // Traffic load to use:
    // Earlang parameters:
      // First runs (1e6)
    //double  run[10] = {44.72, 89.44, 134.16, 178.88, 223.6, 268.32, 313.04, 357.76, 402.48, 447.2};
      // Second runs (1e7)
    double  run[10] = {36};

    int times_goal = 21;
    int times_current = 1;

  // Run by order type (R: route, M: modulation, B: band)
  for (int o = 3; o < 4; o++){

    // Inform to console order type running
    switch (o)
    {
    case 0:
        std::cout << "RMB:\n";
        break;
    case 1:
        std::cout << "BMR:\n";
        break;
    case 2:
        std::cout << "BRM:\n";
        break;
    case 3:
        std::cout << "RBM:\n";
        break;
    case 4:
        std::cout << "MRB:\n";
        break;
    case 5:
        std::cout << "By resource RBM:\n";
        break;
    case 6:
        std::cout << "MBR:\n";
        break;
    case 7:
        std::cout << "By resource RMB:\n";
        break;
    }

    // Run by traffic load
    while (times_current < times_goal){

      // Buffer state to console (ON/OFF)
      if (buffer_state) std::cout << "Buffer:\t\t    ON\n";
      else std::cout << "Buffer:\t\t    OFF\n";

      // Simulator object
      sim = Simulator(std::string("./networks/Eurocore.json"),                      // Network nodes and links
                      std::string("./networks/Eurocore_routes.json"),                      // Network Routes
                      std::string("./networks/bitrate_iroBand_C_BPSK.json"));     // BitRates and bands (eg. BPSK/C)

      // Assing alloc function   
      USE_ALLOC_FUNCTION(FirstFits, sim);

      // Assing unalloc function ONLY if buffer is activated
      if (buffer_state){
        USE_UNALLOC_FUNCTION(sim);

        // Output of realloc time:
        realloc_time.open(fileName[times_current], std::ios::out | std::ios::app);
      }

      // Parameters
      sim.setGoalConnections(number_connections);
      sim.setLambda(144 + run[0]*times_current);
      sim.setMu(1);
      sim.init();

      // offline order by:
        // o parameter defines the order to use
      pathsOffline = offlineApproachOrder(sim.getBitRates(), sim.getPaths(), o);

      // Export routes as CSV
      // offlineApproachCSV(pathsOffline, o);

      // set controller (required for buffer)
      buffer_controller = sim.getController();

      // Begin sim
      sim.run();

      // BBP calculation and output to txt
      std::fstream output;
      output.open("./out/Eurocore-WBuffer-1e7-RSA.txt", std::ios::out | std::ios::app);
      double BBP_results;
        // different BBP formula depending if buffer is activated
      if (buffer_state) BBP_results = bandwidthBlockingProbabilityWBuffer(bitrate_count_total, buffer.elements, mean_weight_bitrate);
      else BBP_results = bandwidthBlockingProbability(bitrate_count_total, bitrate_count_blocked, mean_weight_bitrate);

      resultsToFile(buffer_state, output, BBP_results, sim.getBlockingProbability(), number_connections,
                    o, times_current, run[0]*times_current, bitrate_count_blocked, buffer);

      if (buffer_state) realloc_time.close();

      // ############################## DEBUG #################################
/*
      std::cout << "Tried times in :\n";
      for (int t = 0; t < tried_times.size(); t++){
      std::cout << t << ": " << tried_times[t] << ", \n";  
      }
      std::cout << "\n";

      std::cout << " Blockeos por cada bitRate: \n";
      for (int b = 0; b < 5; b++){
      std::cout << bitrate_count_blocked[b] << "\n\n";
      }
*/
      // ############################## Clean UP variables #################################

      // Free memory offline vector paths
      offlineApproachFree(pathsOffline);

      // Clear buffer and related variables
      for (int be = 0; be < buffer.size(); be++) delete(buffer.elements[be].bitRate);
      buffer.clear();
      buffer.poped = 0;
      buffer.last_time = 0;
      buffer.mean_service_time = 0;
      buffer.mean_size_time = 0;
      buffer.mean_attempts = 0;

      // Reset global variables for BBP calculation
      for (int b = 0; b < 5; b++){
        bitrate_count_total[b] = 0.0;
        bitrate_count_blocked[b] = 0.0;
      }

      // ####################################################################################

      // Traffic load increment
      times_current++;
    }
  }
  return 0;
}