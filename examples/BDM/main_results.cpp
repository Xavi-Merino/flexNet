#include "./src/variants.cpp"

/*********************************************************************************
 * This is an allocation algorithm that replicates the results from the article:
 * "Heuristic Approaches for Dynamic Provisioning in Multi-Band Elastic Optical Networks"
 * Three are the algorithm proposed: V1, V2, and V3. Compared to RBMSA and RMBSA
 * Reference:
 * https://ieeexplore.ieee.org/document/9632602
 **********************************************************************************/

// ############################## Set up simulation #################################

  // Set true one at a time!
  // Variant 1
  bool v1 = false;
  double median_v1;

  // Variant 2
  bool v2 = false;
  double longest_shortest;

  // Variant 3
  bool v3 = false;
  double median_v3;

  // RBMSA/RMBSA/ResRBMSA = True if all other are false
  // If RBMSA set "order" = 0
  // If RMBSA set "order" = 1
  // If ResRBMSA set "order" = 2

// ############################## Global Variables #################################

// Number of connections
int number_connections = 1e6;

// Vector that stores routes in order defined by the user in offlineApproachOrder()
std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline;

// BBP global variables
double bitrate_count_total[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
double bitrate_count_blocked[5] = {0.0, 0.0, 0.0, 0.0, 0.0};

// Weight C+L+S+E:
double mean_weight_bitrate[5] = {1.0, 1.83, 3.5, 13.33, 32.83};
// Weight Only C:
// double mean_weight_bitrate[5] = {1.0, 1.25, 3.0, 9.5, 23};

// Variants variables
  std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline_set1;
  std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline_set2;
  std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline_set3;
  std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline_set4;

// #################################################################################

// Allocation function (To compare: RBMSA/RMBSA/ResRBMSA)
BEGIN_ALLOC_FUNCTION(FirstFit) {

  int currentNumberSlots;
  int currentSlotIndex;
  int *band_slot_indexes = NULL;
  int bitRateInt = bitRates_map[REQ_BITRATE];
  int numberOfSlots;

  std::vector<AuxRoute *> *pathsRBMSA_reqBitRate = &pathsOffline[bitRateInt][SRC][DST];
  std::vector<bool> totalSlots;
  bitrate_count_total[bitRateInt] += 1;

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
                        (*pathsRBMSA_reqBitRate)[r]->getBand(),
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
        // Alloc the slots on every link
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
  return NOT_ALLOCATED;
}
END_ALLOC_FUNCTION


// Allocation function (Proposed)
BEGIN_ALLOC_FUNCTION(Variant_1) {

  int currentNumberSlots;
  int currentSlotIndex;
  int *band_slot_indexes = NULL;
  int bitRateInt = bitRates_map[REQ_BITRATE];
  int numberOfSlots;

  std::vector<AuxRoute *> *pathsRBMSA_reqBitRate_set;
  std::vector<bool> totalSlots;
  bitrate_count_total[bitRateInt] += 1;

  // We choose the order to use depending on first route length (asuming first one is the shortest one)
  if (pathsOffline[bitRateInt][SRC][DST][0]->getTotalLength() < median_v1){
    pathsRBMSA_reqBitRate_set = &pathsOffline_set1[bitRateInt][SRC][DST];
  } else pathsRBMSA_reqBitRate_set = &pathsOffline_set2[bitRateInt][SRC][DST];

  for (int r = 0; r < (*pathsRBMSA_reqBitRate_set).size();
      r++){ // <- For route r between SRC and DST

    // Number of slots that this route requires:
    numberOfSlots = (*pathsRBMSA_reqBitRate_set)[r]->getSlots();

    // Boolean vector of size of the first link of the route:
    totalSlots = std::vector<bool>((*pathsRBMSA_reqBitRate_set)[r]->getLinks()[0]->getSlots(), false);

    // This function sets the 'band_slot_indexes' array with the values of the indices that correspond to the current
    // band in use by the route r:
    // i.e. band_slot_indexes[0] =  index value of the first slot of the band
    //      band_slot_indexes[1] =  index value of the last slot of the band
    //      band_slot_indexes[3] =  total band slot capacity
    if (band_slot_indexes != NULL) delete(band_slot_indexes);
    bandSliceParameters((*pathsRBMSA_reqBitRate_set)[r]->getLinks()[0]->getSlots(),
                        (*pathsRBMSA_reqBitRate_set)[r]->getBand(),
                        &band_slot_indexes);

    for (int l = 0; l < (*pathsRBMSA_reqBitRate_set)[r]->getLinks().size();
        l++){ // <- For link l in route r
      for (int s = band_slot_indexes[0]; s <= band_slot_indexes[1];
          s++){ // <- For each slot s in the current band, of the link l
        
        // We verify the status of the slot s in every link l of the route r
        totalSlots[s] = totalSlots[s] | (*pathsRBMSA_reqBitRate_set)[r]->getLinks()[l]->getSlot(s);
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
        // Alloc the slots on every link
        for (int l = 0; l < (*pathsRBMSA_reqBitRate_set)[r]->getLinks().size();
            l++){
          ALLOC_SLOTS((*pathsRBMSA_reqBitRate_set)[r]->getLinks()[l]->getId(), currentSlotIndex, numberOfSlots)
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
  return NOT_ALLOCATED;
}
END_ALLOC_FUNCTION

// Allocation function (Proposed)
BEGIN_ALLOC_FUNCTION(Variant_2) {

  int currentNumberSlots;
  int currentSlotIndex;
  int *band_slot_indexes = NULL;
  int bitRateInt = bitRates_map[REQ_BITRATE];
  int numberOfSlots;

  std::vector<AuxRoute *> *pathsRBMSA_reqBitRate_set;
  std::vector<bool> totalSlots;
  bitrate_count_total[bitRateInt] += 1;

  // We choose the order to use depending on first route length (asuming first one is the shortest one)
  if (pathsOffline[bitRateInt][SRC][DST][0]->getTotalLength() <= longest_shortest/4){
    pathsRBMSA_reqBitRate_set = &pathsOffline_set1[bitRateInt][SRC][DST];
  } else if (pathsOffline[bitRateInt][SRC][DST][0]->getTotalLength() <= longest_shortest/2){
    pathsRBMSA_reqBitRate_set = &pathsOffline_set2[bitRateInt][SRC][DST];
  } else if (pathsOffline[bitRateInt][SRC][DST][0]->getTotalLength() <= (3*longest_shortest)/4){
    pathsRBMSA_reqBitRate_set = &pathsOffline_set3[bitRateInt][SRC][DST];
  } else pathsRBMSA_reqBitRate_set = &pathsOffline_set4[bitRateInt][SRC][DST];

  for (int r = 0; r < (*pathsRBMSA_reqBitRate_set).size();
      r++){ // <- For route r between SRC and DST

    // Number of slots that this route requires:
    numberOfSlots = (*pathsRBMSA_reqBitRate_set)[r]->getSlots();

    // Boolean vector of size of the first link of the route:
    totalSlots = std::vector<bool>((*pathsRBMSA_reqBitRate_set)[r]->getLinks()[0]->getSlots(), false);

    // This function sets the 'band_slot_indexes' array with the values of the indices that correspond to the current
    // band in use by the route r:
    // i.e. band_slot_indexes[0] =  index value of the first slot of the band
    //      band_slot_indexes[1] =  index value of the last slot of the band
    //      band_slot_indexes[3] =  total band slot capacity
    if (band_slot_indexes != NULL) delete(band_slot_indexes);
    bandSliceParameters((*pathsRBMSA_reqBitRate_set)[r]->getLinks()[0]->getSlots(),
                        (*pathsRBMSA_reqBitRate_set)[r]->getBand(),
                        &band_slot_indexes);

    for (int l = 0; l < (*pathsRBMSA_reqBitRate_set)[r]->getLinks().size();
        l++){ // <- For link l in route r
      for (int s = band_slot_indexes[0]; s <= band_slot_indexes[1];
          s++){ // <- For each slot s in the current band, of the link l
        
        // We verify the status of the slot s in every link l of the route r
        totalSlots[s] = totalSlots[s] | (*pathsRBMSA_reqBitRate_set)[r]->getLinks()[l]->getSlot(s);
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
        // Alloc the slots on every link
        for (int l = 0; l < (*pathsRBMSA_reqBitRate_set)[r]->getLinks().size();
            l++){
          ALLOC_SLOTS((*pathsRBMSA_reqBitRate_set)[r]->getLinks()[l]->getId(), currentSlotIndex, numberOfSlots)
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
  return NOT_ALLOCATED;
}
END_ALLOC_FUNCTION

// Allocation function (Proposed)
BEGIN_ALLOC_FUNCTION(Variant_3) {

  int currentNumberSlots;
  int currentSlotIndex;
  int *band_slot_indexes = NULL;
  int bitRateInt = bitRates_map[REQ_BITRATE];
  int numberOfSlots;

  std::vector<AuxRoute *> *pathsRBMSA_reqBitRate_set;
  std::vector<AuxRoute *> *pathsRBMSA_reqBitRate = &pathsOffline[bitRateInt][SRC][DST];
  std::vector<bool> totalSlots;
  bitrate_count_total[bitRateInt] += 1;


  // We choose the order to use depending on first required bitrate
  if (REQ_BITRATE < median_v3){
    pathsRBMSA_reqBitRate_set = &pathsOffline_set1[bitRateInt][SRC][DST];
  } else pathsRBMSA_reqBitRate_set = &pathsOffline_set2[bitRateInt][SRC][DST];

  for (int r = 0; r < (*pathsRBMSA_reqBitRate_set).size();
      r++){ // <- For route r between SRC and DST
    // Number of slots that this route requires:
    numberOfSlots = (*pathsRBMSA_reqBitRate_set)[r]->getSlots();

    // Boolean vector of size of the first link of the route:
    totalSlots = std::vector<bool>((*pathsRBMSA_reqBitRate_set)[r]->getLinks()[0]->getSlots(), false);

    // This function sets the 'band_slot_indexes' array with the values of the indices that correspond to the current
    // band in use by the route r:
    // i.e. band_slot_indexes[0] =  index value of the first slot of the band
    //      band_slot_indexes[1] =  index value of the last slot of the band
    //      band_slot_indexes[3] =  total band slot capacity
    if (band_slot_indexes != NULL) delete(band_slot_indexes);
    bandSliceParameters((*pathsRBMSA_reqBitRate_set)[r]->getLinks()[0]->getSlots(),
                        (*pathsRBMSA_reqBitRate_set)[r]->getBand(),
                        &band_slot_indexes);

    for (int l = 0; l < (*pathsRBMSA_reqBitRate_set)[r]->getLinks().size();
        l++){ // <- For link l in route r
      for (int s = band_slot_indexes[0]; s <= band_slot_indexes[1];
          s++){ // <- For each slot s in the current band, of the link l
        
        // We verify the status of the slot s in every link l of the route r
        totalSlots[s] = totalSlots[s] | (*pathsRBMSA_reqBitRate_set)[r]->getLinks()[l]->getSlot(s);
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
        // Alloc the slots on every link
        for (int l = 0; l < (*pathsRBMSA_reqBitRate_set)[r]->getLinks().size();
            l++){
          ALLOC_SLOTS((*pathsRBMSA_reqBitRate_set)[r]->getLinks()[l]->getId(), currentSlotIndex, numberOfSlots)
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
  return NOT_ALLOCATED;
}
END_ALLOC_FUNCTION


int main(int argc, char* argv[]) {

  // Earlangs to run:
  double  earlangs[15] = {1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500, 8000};
  // Output files:
  const char* filenames[5] = {"./out/RMBSA.txt", "./out/RBMSA.txt", "./out/Variant1.txt", "./out/Variant2.txt", "./out/Variant3.txt"};
  std::string filename;

  // Run by order type (R: route, M: modulation, B: band)
  // Order 0 = RBMSA, Order 1 = RMBSA, Order 2 = ResRBMSA
  for (int order = 0; order < 1; order++){
    // Inform to console order type running
    switch (order)
    {
    case 0:
        std::cout << "RMBSA:\n";
        break;
    case 1:
        std::cout << "RBMSA:\n";
        break;
    case 2:
        std::cout << "Resource RBMSA:\n";
        break;
    }

    // Run by traffic load
    for (int earlang = 0; earlang < (sizeof(earlangs)/sizeof(double)); earlang++){

      // Simulator object
      Simulator sim;
      sim = Simulator(std::string("./networks/COST239.json"),                    // Network nodes and links
                      std::string("./networks/routes_cost239.json"),                    // Network Routes
                      std::string("./networks/bitrate_iroBand_CLSE.json"));     // BitRates and bands (eg. BPSK/C)

      // Assing alloc function 
      if (v1){
        USE_ALLOC_FUNCTION(Variant_1, sim);
      } else if (v2) {  USE_ALLOC_FUNCTION(Variant_2, sim)
      } else if (v3) {  USE_ALLOC_FUNCTION(Variant_3, sim)
      } else USE_ALLOC_FUNCTION(FirstFit, sim);

      // Sim parameters
      sim.setGoalConnections(number_connections);
      sim.setLambda(earlangs[earlang]);
      sim.setMu(1);
      sim.init();

      // offline order by:
        // 'order' parameter defines the order to use
      pathsOffline = offlineApproachOrder(sim.getBitRates(), sim.getPaths(), order);

      // FOR VARIANT 1
      if (v1){
        // Calculate median of shortests routes
        std::vector<double> lengths = shortestLengths(pathsOffline);
        median_v1 = calculateMedian(lengths);

        // Create sets X=2
        pathsOffline_set1 = offlineApproachOrder_V1_Set1(sim.getBitRates(), sim.getPaths());
        pathsOffline_set2 = offlineApproachOrder_V1_Set2(sim.getBitRates(), sim.getPaths());

        // output results filename
        filename = filenames[2];

        // Export routes as CSV
        //offlineApproachCSV(pathsOffline_set1, 2);
        //offlineApproachCSV(pathsOffline_set2, 3);
      }

      // FOR VARIANT 2
      else if (v2){
        // Calculate longest of shortests routes
        std::vector<double> lengths = shortestLengths(pathsOffline);
        sort(lengths.begin(), lengths.end());
        longest_shortest = lengths.back();

        // Create sets X=4
        pathsOffline_set1 = offlineApproachOrder_V2_Set1(sim.getBitRates(), sim.getPaths());
        pathsOffline_set2 = offlineApproachOrder_V2_Set2(sim.getBitRates(), sim.getPaths());
        pathsOffline_set3 = offlineApproachOrder_V2_Set3(sim.getBitRates(), sim.getPaths());
        pathsOffline_set4 = offlineApproachOrder_V2_Set4(sim.getBitRates(), sim.getPaths());

        // output results filename
        filename = filenames[3];

        // Export routes as CSV
        //offlineApproachCSV(pathsOffline_set1, 4);
        //offlineApproachCSV(pathsOffline_set2, 5);
        //offlineApproachCSV(pathsOffline_set3, 6);
        //offlineApproachCSV(pathsOffline_set4, 7);
      }

      // FOR VARIANT 3  
      else if (v3){
        // Calculate longest of shortests routes
        std::vector<double> bitrates;
        for (int b = 0; b < sim.getBitRates().size(); b++){
          bitrates.push_back(sim.getBitRates()[b].getBitRate());
        }
        median_v3 = calculateMedian(bitrates);

        pathsOffline_set1 = offlineApproachOrder_V3_Set1(sim.getBitRates(), sim.getPaths());
        pathsOffline_set2 = offlineApproachOrder_V3_Set2(sim.getBitRates(), sim.getPaths());
        // output filename
        filename = filenames[4];
        // Export routes as CSV
        //offlineApproachCSV(pathsOffline_set1, 8);
        //offlineApproachCSV(pathsOffline_set2, 9);
      }

      else {
        // Export routes as CSV
        //offlineApproachCSV(pathsOffline, 0);
        // output filename
        filename = filenames[order];
      }

      // Begin sim
      //break;
      sim.run();

      // BBP calculation and output to txt
      std::fstream output;
      output.open(filename, std::ios::out | std::ios::app);
      double BBP_results;
      BBP_results = bandwidthBlockingProbability(bitrate_count_total, bitrate_count_blocked, mean_weight_bitrate);
        // Restults to file
      resultsToFile(output, BBP_results, sim.getBlockingProbability(), number_connections,
                    order, earlang, earlangs[earlang], bitrate_count_blocked);


      // ############################## Reset Variables #################################

      // Free memory offline vector paths
      offlineApproachFree(pathsOffline);
    
      // Reset global variables for BBP calculation
      for (int b = 0; b < 5; b++){
        bitrate_count_total[b] = 0.0;
        bitrate_count_blocked[b] = 0.0;
      }

      // FOR VARIANT 1
      if (v1){
        offlineApproachFree(pathsOffline_set1);
        offlineApproachFree(pathsOffline_set2);
      }
      // FOR VARIANT 2
      else if (v2){
        offlineApproachFree(pathsOffline_set1);
        offlineApproachFree(pathsOffline_set2);
        offlineApproachFree(pathsOffline_set3);
        offlineApproachFree(pathsOffline_set4);
      }
      // FOR VARIANT 3  
      else if (v3){
        offlineApproachFree(pathsOffline_set1);
        offlineApproachFree(pathsOffline_set2);
      }
      // ####################################################################################
    }
  }
  return 0;
}