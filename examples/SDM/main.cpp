#include <fstream>
#include "./simulator.hpp"
#include <bits/stdc++.h>
// #include <fnsim/simulator.hpp>

#define DA_RCSA 0
#define FF_RCSA 1
#define EPSILON 1e-6

// ############################## Setup Global Variables #################################

int numberConnections = 1e5; // (requests)

// Traffic loads
double  lambdas[10] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

// Select algorithm to use: (DA_RCSA, FF_RCSA)
int active_algorithm = DA_RCSA;

// Number of cores per link
int const number_of_cores = 7;

// Total capacity network: (number of links * number of cores * number of slots)
double totalCapacity = 44 * number_of_cores * 80;

// (XT)
// coupling coefficient (k)
double k = 4e-4;
// bend radius (r)
double r = 5e-2;
// propagation constant (b)
double b = 4e6;
// core pitch (w)
double w = 4e-5;
// XT Threshold                             (16QAM | QPSK | BPSK )
std::vector<double> XT_Threshold_by_bitrate = {-25, -18.5, -14};

// ############################## Other global variables #################################

// Global access
Simulator sim;

// Cores adjacency matrix
int adjacencyMatrix[number_of_cores][number_of_cores];

// blocked by XT counter
int blocked_by_XT = 0;

// Resource utilization variables
double currentUtilization = 0;
double maxUtilization = 0;


// ################################## Aux functions ######################################

// Power coupling coeficient
double pwrCouplingCoef(double k, double r, double b, double w)
{
    double numerator = 2 * (pow(k, 2.0)) * r;
    double denominator = b * w;
    double h = numerator / denominator;
    return h;
}

// Mean crosstalk
double meanCrosstalk(int n, double L, double k, double r, double b, double w)
{
    double h = pwrCouplingCoef(k, r, b, w);
    double numerator = n - n * (exp(-(n + 1) * 2 * h * L * 1000));
    double denominator = 1 + n * (exp(-(n + 1) * 2 * h * L * 1000));
    double XT = numerator / denominator;
    //std::cout << numerator << ", " << denominator << ", " << n << ", " << XT << "\n";
    return std::log10(XT)*10;
}

// Add edge into the adjacency matrix
void addEdge(int u, int v)
{
    adjacencyMatrix[u][v] = 1;
    adjacencyMatrix[v][u] = 1;
}

// Display core adjacency matrix
void displayMatrix(int number_of_cores)
{
    for (int i = 0; i < number_of_cores; i++)
    {
        for (int j = 0; j < number_of_cores; j++)
        {
            std::cout << adjacencyMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// Get neighbours (returns neighbours indexes of a given core)
std::vector<int> getNeighbours(int core, int number_of_cores)
{
    std::vector<int> neighbours;
    for (int n = 0; n < number_of_cores; n++)
    {
        if (core == n)
            continue;
        if (adjacencyMatrix[core][n] == 1)
            neighbours.push_back(n);
    }
    return neighbours;
}

// Returns true or false if current connection would be over Threshold
bool isOverThreshold(int coreIndex, std::vector<Link *> links, int reqSlots, int slotBegin, int routeLength, double XT_Threshold)
{
    std::vector<int> neighbours = getNeighbours(coreIndex, number_of_cores);
    int slotEnd = reqSlots + slotBegin - 1;
    int activeNeighboursCurrent = 0;
    int activeNeighbours = 0;

    for (int l = 0; l < links.size(); l++) {
      for (int s = slotBegin; s < slotEnd; s++) {
        activeNeighboursCurrent = 0;
        for (int neighbour = 0; neighbour < neighbours.size(); neighbour++) {
          if (links[l]->getSlot(neighbours[neighbour], 0, s) == true) {
            if (coreIndex != 6) {
              if (activeNeighbours < 3)
                activeNeighboursCurrent++;
                if (activeNeighboursCurrent > activeNeighbours)
                  activeNeighbours = activeNeighboursCurrent;
              else {
                break;
              }
            }
            else {
              if (activeNeighbours < 6)
                activeNeighboursCurrent++;
                if (activeNeighboursCurrent > activeNeighbours)
                  activeNeighbours = activeNeighboursCurrent;
              else {
                  break;
              }
            }
          }
        }
      }
    }
    if (meanCrosstalk(activeNeighbours, routeLength, k, r, b, w) >  XT_Threshold + EPSILON) {
        blocked_by_XT++;
        return true;
    }
    return false;
}

// ############################## Metrics and results functions #################################

// BBP variables
double bitrateCountTotal[3] = {0.0, 0.0, 0.0};
double bitrateCountBlocked[3] = {0.0, 0.0, 0.0};
double meanWeightBitrate[3] = {2.33, 4.66, 7};

// Bitrate map
std::map<float, int> bitRates_map { { 40.0 , 0 }, { 100.0 , 1 }, { 200.0 , 2 }};

// Calculate Bandwidth blocking probability
double bandwidthBlockingProbability(double bitrateCountTotal[3], 
                                    double bitrateCountBlocked[3],
                                    double meanWeightBitrate[3])
    {
    double BBP = 0;
    double BP = 0;
    double total_weight = 0;

    for (int b = 0; b < 3; b++){
        total_weight += meanWeightBitrate[b];
        if (bitrateCountTotal[b] == 0) continue;
        BP = bitrateCountBlocked[b] / bitrateCountTotal[b];
        BBP += meanWeightBitrate[b] * BP;
    }

    return (BBP/total_weight);
}

// Result to TXT
void resultsToFile(std::fstream &output, double BBP, double BP, double confidenceInterval, int lambda_index,
                    double erlang, double maxUtilization, double blocked_by_XT, double time_elapsed_ms)
{
  output << "erlang index: " << lambda_index
          << ", erlang: " << erlang
          << ", general blocking: " << BP
          << ", BBP: " << BBP
          << ", Confidence interval: " << confidenceInterval
          << ", Total current utilization: " << maxUtilization
          << ", Blocked by XT: " << blocked_by_XT
          << ", CPU time: " << time_elapsed_ms                     
          << '\n';
}

// ################################## Alloc function ######################################

// DA-RCSA
BEGIN_ALLOC_FUNCTION(DA) {

  int currentNumberSlots;
  int currentSlotIndex;
  int bitrateInt = bitRates_map[REQ_BITRATE];
  int numberOfSlots;
  std::vector<bool> totalSlots;

  // Bitrate count
  bitrateCountTotal[bitrateInt] += 1;

  for (int r = 0; r < NUMBER_OF_ROUTES; r++){ // <- this loops through the routes between SRC and DST
    for (int c = 0; c < NUMBER_OF_CORES(r, 0); c++){ // <- this loops through the cores (assuming continuity and contiguity constrain)

      double routeLength = 0;
      totalSlots = std::vector<bool>(LINK_IN_ROUTE(r, 0)->getSlots(), false);

      for (int l = 0; l < NUMBER_OF_LINKS(r); l++){ // <- this loops through the links that the current route contains
        
        // Save current route length for XT calculation
        routeLength += LINK_IN_ROUTE(r,l)->getLength();

        for (int s = 0; s < LINK_IN_ROUTE(r, l)->getSlots(); s++){
          totalSlots[s] = totalSlots[s] | LINK_IN_ROUTE(r, l)->getSlot(c, 0, s);
          }
      }

      for (int m = 0; m < NUMBER_OF_MODULATIONS; m++){

        numberOfSlots = REQ_SLOTS(m); //<- Number of slots that this modulation format requires
        if (routeLength > REQ_REACH(m) + EPSILON) continue; // Check transparent reach

        // Number of consecutive free slots:
        currentNumberSlots = 0;
        currentSlotIndex = 0;

        for (int s = 0; s < totalSlots.size(); s++) {
          if (totalSlots[s] == false) {
            currentNumberSlots++;
          } else {
            currentNumberSlots = 0;
            currentSlotIndex = s + 1;
          }
          if (currentNumberSlots >= numberOfSlots) {
            double XT_Threshold = XT_Threshold_by_bitrate[m];

            // Check if crosstalk satisfied
            if (!isOverThreshold(c, sim.getPaths()->at(SRC)[DST][r], numberOfSlots, currentSlotIndex, routeLength, XT_Threshold)){
              for (int l = 0; l < NUMBER_OF_LINKS(r); l++) {
                ALLOC_SLOTS_SDM(LINK_IN_ROUTE_ID(r, l), c, 0, currentSlotIndex, numberOfSlots)
              }

              // Update max utilization
              currentUtilization = currentUtilization + (numberOfSlots * NUMBER_OF_LINKS(r));
              if (currentUtilization/totalCapacity > maxUtilization) maxUtilization = currentUtilization/totalCapacity;
              
              return ALLOCATED;
            }
            else {
              currentSlotIndex++;
            }
          }
        }
      }
    }
  }
  bitrateCountBlocked[bitrateInt] += 1;
  return NOT_ALLOCATED;
}
END_ALLOC_FUNCTION

// FF-RCSA
BEGIN_ALLOC_FUNCTION(FF) {

  int currentNumberSlots;
  int currentSlotIndex;
  int bitrateInt = bitRates_map[REQ_BITRATE];
  int numberOfSlots;
  std::vector<bool> totalSlots;

  int BPSK = 2;
  double XT_Threshold = XT_Threshold_by_bitrate[BPSK];

  // Bitrate count
  bitrateCountTotal[bitrateInt] += 1;

  for (int r = 0; r < NUMBER_OF_ROUTES; r++){ // <- this loops through the routes between SRC and DST
    for (int c = 0; c < NUMBER_OF_CORES(r, 0); c++){ // <- this loops through the cores (assuming continuity and contiguity constrain)

      double routeLength = 0;
      totalSlots = std::vector<bool>(LINK_IN_ROUTE(r, 0)->getSlots(), false);

      for (int l = 0; l < NUMBER_OF_LINKS(r); l++){ // <- this loops through the links that the current route contains
        
        // Save current route length for XT calculation
        routeLength += LINK_IN_ROUTE(r,l)->getLength();

        for (int s = 0; s < LINK_IN_ROUTE(r, l)->getSlots(); s++){
          totalSlots[s] = totalSlots[s] | LINK_IN_ROUTE(r, l)->getSlot(c, 0, s);
          }
      }

      numberOfSlots = REQ_SLOTS(BPSK); //<- Number of slots that BPSK modulation format requires
      if (routeLength > REQ_REACH(BPSK) + EPSILON) break; // Check transparent reach

      // Number of consecutive free slots:
      currentNumberSlots = 0;
      currentSlotIndex = 0;

      for (int s = 0; s < totalSlots.size(); s++) {
        if (totalSlots[s] == false) {
          currentNumberSlots++;
        } else {
          currentNumberSlots = 0;
          currentSlotIndex = s + 1;
        }
        if (currentNumberSlots >= numberOfSlots) {

          // Check if crosstalk satisfied
          if (!isOverThreshold(c, sim.getPaths()->at(SRC)[DST][r], numberOfSlots, currentSlotIndex, routeLength, XT_Threshold)){
            for (int l = 0; l < NUMBER_OF_LINKS(r); l++) {
              ALLOC_SLOTS_SDM(LINK_IN_ROUTE_ID(r, l), c, 0, currentSlotIndex, numberOfSlots)
            }

            // Update max utilization
            currentUtilization = currentUtilization + (numberOfSlots * NUMBER_OF_LINKS(r));
            if (currentUtilization/totalCapacity > maxUtilization) maxUtilization = currentUtilization/totalCapacity;
            
            return ALLOCATED;
          }
          else {
            currentSlotIndex++;
          }
        }
      }
    }
  }
  bitrateCountBlocked[bitrateInt] += 1;
  return NOT_ALLOCATED;
}
END_ALLOC_FUNCTION

// ############################## Unalloc callback function #################################

BEGIN_UNALLOC_CALLBACK_FUNCTION {
  
  // Store max utilization
  currentUtilization = currentUtilization - (c.getSlots()[0].size() * c.getLinks().size());
  if (currentUtilization/totalCapacity > maxUtilization) maxUtilization = currentUtilization/totalCapacity;

}
END_UNALLOC_CALLBACK_FUNCTION

// #############################################################################################

int main(int argc, char* argv[]) {

  // Set adjacent cores matrix
  addEdge(0, 1);
  addEdge(0, 5);
  addEdge(0, 6);
  addEdge(1, 2);
  addEdge(1, 6);
  addEdge(2, 3);
  addEdge(2, 6);
  addEdge(3, 4);
  addEdge(3, 6);
  addEdge(4, 5);
  addEdge(4, 6);
  addEdge(5, 6);
  //displayMatrix(number_of_cores);

  // Simulation
  for (int lambda = 0; lambda < sizeof(lambdas)/sizeof(double); lambda++) {

    // Simulator object
    sim = Simulator(std::string("./networks/NSFNet.json"),                      // Network nodes, links and cores
                    std::string("./networks/NSFNet_routes.json"),               // Network Routes
                    std::string("./networks/bitrates.json"),                    // BitRates (eg. BPSK)
                    SDM);                                                       // Network type (SDM, EON)
                                             

    // Sim parameters
    if (active_algorithm == DA_RCSA) USE_ALLOC_FUNCTION(DA, sim)
    else USE_ALLOC_FUNCTION(FF, sim)
    
    USE_UNALLOC_FUNCTION_SDM(sim);
    sim.setGoalConnections(numberConnections);
    sim.setLambda(lambdas[lambda]);
    // sim.setSeedArrive(505);
    // sim.setSeedDeparture(505);
    sim.setMu(1);
    sim.init();

    // Run simulation
    std::clock_t c_start = std::clock();
    sim.run();
    std::clock_t c_end = std::clock();

    // CPU time
    double time_elapsed_ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;

// ############################## Out results #################################

    std::fstream output;
    output.open("./out/SDM-NSFNet.txt", std::ios::out | std::ios::app);

    double BBP_results;
    double confidenceInterval =  sim.wilsonCI();
    BBP_results = bandwidthBlockingProbability(bitrateCountTotal, bitrateCountBlocked, meanWeightBitrate);
    currentUtilization = currentUtilization/totalCapacity;

    resultsToFile(output, BBP_results, sim.getBlockingProbability(), confidenceInterval,
                  lambda, lambdas[lambda], maxUtilization, blocked_by_XT, time_elapsed_ms);
    

// ############################## reset metric variables #################################

    for (int b = 0; b < 3; b++){
      bitrateCountTotal[b] = 0.0;
      bitrateCountBlocked[b] = 0.0;
    }
    blocked_by_XT = 0;
    currentUtilization = 0;
    maxUtilization = 0;
  }
  return 0;
}