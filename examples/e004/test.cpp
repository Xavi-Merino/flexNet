#include "offline_approach.cpp"

// ############################## Global Variables #################################
// Queue for buffer
std::deque<buffer_element> buffer;

// Vector that stores routes in order defined by the user in offlineApproachOrder()
std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline;

// BBP global variables
double bitrate_count_total[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
double bitrate_count_blocked[5] = {0.0, 0.0, 0.0, 0.0, 0.0};

// Number of connections popped from buffer (allocated succesfully)
int poped = 0;

// Number of connections pushed to buffer (blocked) = (buffer.size() - poped)
int pushed = 0;

// Number of times a connection was tried to be allocated from buffer
std::vector<int> tried_times;
int current_tries = 0;

// Buffer state
bool buffer_state = true;
bool allocating_from_buffer = false;

// Weight C+L+S+E:
// double mean_weight_bitrate[5] = {1.0, 1.83, 3.5, 13.33, 32.83};
// Weight Only C:
double mean_weight_bitrate[5] = {1.0, 1.25, 3.0, 9.5, 23};

// Controller
Controller *buffer_controller;
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

        return ALLOCATED;
      }      
    }
  }
  // N/Buffer BBP calculation
  bitrate_count_blocked[bitRateInt] += 1;

  // Clear band_slot_indexes
  if (band_slot_indexes != NULL) delete(band_slot_indexes);

  // If the present connection ISN'T coming from buffer, we push to queue
  if (!allocating_from_buffer){
    buffer.push_back(buffer_element(SRC, DST, con.getId(), con.getBitrate()));
    pushed++;
  }
  return NOT_ALLOCATED;
}
END_ALLOC_FUNCTION

// Unalloc callback function
BEGIN_UNALLOC_CALLBACK_FUNCTION {
  if (buffer.size() > 0){

    // Let the alloc function know we are allocating from buffer
    allocating_from_buffer = true;

    // try to alloc
    if (buffer_controller->assignConnection(buffer.front().src, buffer.front().dst, *(buffer.front().bitRate), buffer.front().id, t) == ALLOCATED){

      // We keep track of how many times was tried to be allocated and reset counter
      tried_times.push_back(current_tries);
      current_tries = 0;

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

  // Run by order type (R: route, M: modulation, B: band)
  for (int o = 3; o < 4; o++){

    // Run by traffic load
    for (int i = 3; i < 10; i++){

      // Buffer state to console 
      if (buffer_state) std::cout << "Buffer:\t\t    ON\n";
      else std::cout << "Buffer:\t\t    OFF\n";

      // simulator object:
      Simulator sim =
          Simulator(std::string("NSFNet.json"),                    // Network nodes and links
                    std::string("routes.json"),                    // Network Routes
                    std::string("bitrate_iroBand_CLSE.json"));     // BitRates and bands (eg. BPSK/C)


      // Assign allocation function
      USE_ALLOC_FUNCTION(FirstFits, sim);

      // Assing unalloc function if buffer is activated
//      if (buffer_state)
//        USE_UNALLOC_FUNCTION(sim);

      sim.setGoalConnections(1e6);
      sim.setLambda(run[i]);

      sim.setMu(1);
      sim.init();

      // offline order by:
      pathsOffline = offlineApproachOrder(sim.getBitRates(), sim.getPaths(), o);

      // Export as CSV
      // offlineApproachCSV(pathsOffline, o);

      // set controller
      buffer_controller = sim.getController();

      // Run simulation
      sim.run();

      // BBP calculation and output to txt
      std::fstream output;
      output.open("test.txt", std::ios::out | std::ios::app);
      double BBP_results;

        // different BBP formula depending if buffer is activated
      switch (buffer_state){

        // if buffer OFF
        case false:
          break;

        // if buffer ON
        case true:
          // calculate BBP:
          BBP_results = bandwidthBlockingProbabilityWBuffer(bitrate_count_total, buffer, mean_weight_bitrate);
          // output info to txt and console:
          std::cout << "\n BBP: " << BBP_results << " BP: " << (buffer.size()/(1e6)) << "\n\n";
          output << "W/Buffer orden: " << o 
                 << ", earlang: " << i << ", BBP: " << BBP_results 
                 << ", blocking segun buffer: " << (buffer.size()/(1e6)) 
                 << ", blocking original: " << sim.getBlockingProbability() 
                 << ", buffer size: " << buffer.size() 
                 << ", reallocated: " << poped 
                 << '\n';
          break;
      }

      std::cout << buffer.size() << ',' << pushed << ',' << poped << '\n';

      // ############################## Clean UP variables #################################

      // Free memory offline vector paths
      offlineApproachFree(pathsOffline);

      // Clear buffer and related variables
      buffer.clear();
      poped = 0;
      pushed = 0;
      tried_times.clear();
      current_tries = 0;

      // Reset global variables for BBP calculation
      for (int b = 0; b < 5; b++){
        bitrate_count_total[b] = 0.0;
        bitrate_count_blocked[b] = 0.0;
      }

      // ####################################################################################
    }
  }
  return 0;
}