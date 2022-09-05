#include "./src/offline_approach.cpp"

// ############################## Global Variables #################################
// Queue for buffer
std::deque<buffer_element> buffer;

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

// #################################################################################

// Allocation function
BEGIN_ALLOC_FUNCTION(FirstFits) {

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
    // If not allocated add to back of buffer
    buffer.push_back(buffer_element(SRC, DST, con.getId(), con.getBitrate()));

    // If the present connection IS coming from buffer, we poped it because was added to back
    if (allocating_from_buffer){
      delete(buffer.front().bitRate);
      buffer.pop_front();
      int temp = tried_times.front() + 1;
      tried_times.push_back(temp);
      tried_times.pop_front();
    }
    // If the present connection isn't coming from buffer, add another try
    else {
      current_attempts++;
      tried_times.push_back(current_attempts);
      pushed++;
    }
  }

  return NOT_ALLOCATED;
}
END_ALLOC_FUNCTION

// Unalloc callback function
BEGIN_UNALLOC_CALLBACK_FUNCTION {
  if (buffer.size() > 0){
    buffer_element *front_queue = &buffer.front();

    // Let the alloc function know we are allocating from buffer
    allocating_from_buffer = true;

    // try to alloc
    if (buffer_controller->assignConnection(front_queue->src, front_queue->dst, *(front_queue->bitRate), front_queue->id, t) == ALLOCATED){
      
      // Add departure to event routine
      sim.addDepartureEvent(front_queue->id);

      // We keep track of how many times was tried to be allocated and reset counter
      total_attempts += tried_times.front();
      tried_times.pop_front();
      current_attempts = 0;

      // Element allocated so we poped it and delete() members
      delete(buffer.front().bitRate);
      buffer.pop_front();

      // Keep count of how many connections where allocated from the buffer
      poped++;
      // std::cout << "Allocated!\n";   
    }

    // Not allocating from buffer anymore
    allocating_from_buffer = false;
  }
}
END_UNALLOC_CALLBACK_FUNCTION



int main(int argc, char* argv[]) {

  // Traffic load to use:
  double  run[10] = {44.72, 89.44, 134.16, 178.88, 223.6, 268.32, 313.04, 357.76, 402.48, 447.2};
  int times_goal = 50;
  int times_current = 10;

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

      // Buffer state to console 
      if (buffer_state) std::cout << "Buffer:\t\t    ON\n";
      else std::cout << "Buffer:\t\t    OFF\n";

      sim = Simulator(std::string("./networks/NSFNet.json"),                    // Network nodes and links
                      std::string("./networks/routes.json"),                    // Network Routes
                      std::string("./networks/bitrate_iroBand_CLSE.json"));     // BitRates and bands (eg. BPSK/C)
            
      USE_ALLOC_FUNCTION(FirstFits, sim);

      // Assing unalloc function if buffer is activated
      if (buffer_state)
        USE_UNALLOC_FUNCTION(sim);

      sim.setGoalConnections(1e6);
      sim.setLambda(run[0]*times_current);
      sim.setMu(1);
      sim.init();

      // offline order by:
      pathsOffline = offlineApproachOrder(sim.getBitRates(), sim.getPaths(), o);

      // Export as CSV
      // offlineApproachCSV(pathsOffline, o);

      // set controller
      buffer_controller = sim.getController();

      sim.run();

      // BBP calculation and output to txt
      std::fstream output;
      output.open("./out/outputV2.0-RBM.txt", std::ios::out | std::ios::app);
      double BBP_results;

        // attempts calculation
      for (int a = 0; a < tried_times.size(); a++){
        total_attempts += tried_times[a];
      }
      float mean = total_attempts/(poped+tried_times.size());

        // different BBP formula depending if buffer is activated
      if (buffer_state) BBP_results = bandwidthBlockingProbabilityWBuffer(bitrate_count_total, buffer, mean_weight_bitrate);
      else BBP_results = bandwidthBlockingProbability(bitrate_count_total, bitrate_count_blocked, mean_weight_bitrate);
      resultsToFile(buffer_state, output, BBP_results, sim.getBlockingProbability(),
                    o, times_current, poped, buffer, mean, bitrate_count_blocked);

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
      for (int be = 0; be < buffer.size(); be++) delete(buffer[be].bitRate);
      buffer.clear();
      poped = 0;
      tried_times.clear();
      current_attempts = 0;
      total_attempts = 0;

      // Reset global variables for BBP calculation
      for (int b = 0; b < 5; b++){
        bitrate_count_total[b] = 0.0;
        bitrate_count_blocked[b] = 0.0;
      }

      // ####################################################################################
      times_current++;
    }
  }
  return 0;
}