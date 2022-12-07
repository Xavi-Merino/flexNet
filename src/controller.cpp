#include "controller.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>

Controller::Controller() {
  this->connections = std::vector<Connection>();
  this->network = nullptr;
  this->allocator = new Allocator;
  this->unassignConnection = &Controller::unassignConnectionNormal;
};

Controller::Controller(Network *network) {
  this->network = network;
  this->connections = std::vector<Connection>();
  this->allocator = new Allocator;
  this->unassignConnection = &Controller::unassignConnectionNormal;
};

Controller::~Controller() {
  if (this->network != nullptr) {
    for (int i = 0; i < this->network->getNumberOfLinks(); i++) {
      delete this->network->getLink(i);
    }
    for (int i = 0; i < this->network->getNumberOfNodes(); i++) {
      delete this->network->getNode(i);
    }
  }
  delete this->allocator;
};

allocationStatus Controller::assignConnection(int src, int dst, BitRate bitRate,
                                              long long idConnection,
                                              double time) {
  Connection con = Connection(idConnection, time, &bitRate);
  this->rtnAllocation = this->allocator->exec(src, dst, bitRate, con);
  if (this->rtnAllocation == ALLOCATED) {
    this->connections.push_back(con);
    for (unsigned int j = 0; j < con.links.size(); j++) {
      for (unsigned int k = 0; k < con.slots[j].size(); k++) {
        this->network->useSlot(con.links[j], con.slots[j][k]);
      }
    }
  }
  return this->rtnAllocation;
}

int Controller::unassignConnectionNormal(long long idConnection, double time) {
  for (unsigned int i = 0; i < this->connections.size(); i++) {
    if (this->connections[i].id == idConnection) {
      for (unsigned int j = 0; j < this->connections[i].links.size(); j++) {
        for (unsigned int k = 0; k < this->connections[i].slots[j].size();
             k++) {
          this->network->unuseSlot(this->connections[i].links[j],
                                   this->connections[i].slots[j][k]);
        }
      }
      this->connections.erase(this->connections.begin() + i);
      break;
    }
  }
  return 0;
}

int Controller::unassignConnectionWCallback(long long idConnection,
                                            double time) {
  for (unsigned int i = 0; i < this->connections.size(); i++) {
    if (this->connections[i].id == idConnection) {
      this->unassignCallback(this->connections[i], time, this->network);
      for (unsigned int j = 0; j < this->connections[i].links.size(); j++) {
        for (unsigned int k = 0; k < this->connections[i].slots[j].size();
             k++) {
          this->network->unuseSlot(this->connections[i].links[j],
                                   this->connections[i].slots[j][k]);
        }
      }
      this->connections.erase(this->connections.begin() + i);
      break;
    }
  }
  return 0;
}

void Controller::setPaths(std::string filename) {
  // open JSON file
  std::ifstream file(filename);

  if (file.fail()) {
    findRoutes(3, filename);
    file.open(filename);
  }

  nlohmann::json filePaths;
  file >> filePaths;

  int numberOfNodes;
  numberOfNodes = this->network->getNumberOfNodes();

  // allocate space for path[src]
  this->path.resize(numberOfNodes);

  // allocate space for path[src][dst]
  for (int t = 0; t < numberOfNodes; t++) {
    this->path[t].resize(numberOfNodes);
  }

  int routesNumber;
  routesNumber = filePaths["routes"].size();

  for (int i = 0; i < routesNumber; i++) {
    int pathsNumber;
    pathsNumber = filePaths["routes"][i]["paths"].size();
    int src, dst;
    src = filePaths["routes"][i]["src"];
    dst = filePaths["routes"][i]["dst"];

    // allocate path[src][dst][pathsNumber]
    int linksInPath;
    linksInPath = pathsNumber - 1;
    // because 3 nodes has 2 links
    this->path[src][dst].resize(pathsNumber);

    // go through available routes
    for (int b = 0; b < pathsNumber; b++) {
      int nodesPathNumber;
      nodesPathNumber = filePaths["routes"][i]["paths"][b].size();
      int lastNode = nodesPathNumber - 1;

      for (int c = 0; c < lastNode; c++) {
        int actNode, nextNode;
        actNode = filePaths["routes"][i]["paths"][b][c];
        nextNode = filePaths["routes"][i]["paths"][b][c + 1];

        int idLink;
        idLink = this->network->isConnected(actNode, nextNode);

        this->path[src][dst][b].push_back(this->network->getLink(idLink));
      }
    }
  }
}

void Controller::setNetwork(Network *network) { this->network = network; }

Network *Controller::getNetwork(void) { return this->network; }

void Controller::setAllocator(Allocator *allocator) {
  this->allocator = allocator;
}

Allocator *Controller::getAllocator(void) { return this->allocator; }

std::vector<std::vector<std::vector<std::vector<Link *>>>>
    *Controller::getPaths() {
  return &(this->path);
}

void Controller::setUnassignCallback(void (*callbackFunction)(Connection,
                                                              double,
                                                              Network *)) {
  this->unassignConnection = &Controller::unassignConnectionWCallback;
  this->unassignCallback = callbackFunction;
}

// K-extended Dijkstra
std::vector<std::vector<int>> Controller::KSPDijkstra(int src, int dst, int k) {
  std::vector<std::vector<int>> paths;  // P
  std::vector<float> lengths;
  std::vector<int> shortestPaths(this->network->getNumberOfNodes());  // count_u

  std::priority_queue<std::pair<float, std::vector<int>>,
                      std::vector<std::pair<float, std::vector<int>>>,
                      std::greater<std::pair<float, std::vector<int>>>>
      priorityPaths;  // B

  priorityPaths.push(std::make_pair(0, std::vector<int>({src})));

  while (!priorityPaths.empty() && shortestPaths[dst] < k) {
    std::vector<int> minPath = priorityPaths.top().second;
    float minCost = priorityPaths.top().first;
    priorityPaths.pop();

    shortestPaths[minPath.back()]++;

    if (minPath.back() == dst) {
      paths.push_back(minPath);
      lengths.push_back(minCost);
    }

    if (shortestPaths[minPath.back()] < k + 1) {
      for (auto adjacent : this->network->adjacencyVector[minPath.back()]) {
        int vertex = adjacent.first;

        if (std::find(minPath.begin(), minPath.end(), vertex) != minPath.end())
          continue;

        int weight = adjacent.second;

        std::vector<int> newPath(minPath);
        newPath.push_back(vertex);
        float newCost = minCost + weight;

        std::pair<float, std::vector<int>> concatenatedPath;
        concatenatedPath = std::make_pair(newCost, newPath);

        priorityPaths.push(concatenatedPath);
      }
    }
  }
  return paths;
}

void Controller::findRoutes(int k, std::string filename) {
  int nodeNumber = this->network->getNumberOfNodes();
  this->network->createAdjacencyVector();

  nlohmann::json jsonFile;
  jsonFile["name"] = "Routes autogenerated using K-SP Dijkstra.";
  jsonFile["alias"] = "Feel free to rename both name and alias.";
  jsonFile["routes"] = nlohmann::json::array();

  // Optimizable (?)
  // Source = i
  for (int i = 0; i < nodeNumber; i++) {
    // Destination = j
    for (int j = 0; j < nodeNumber; j++) {
      if (i == j) continue;

      // Routes from Source to Destination
      nlohmann::json currentRoute = nlohmann::json::object();
      nlohmann::json currentPaths =
          nlohmann::json::array({KSPDijkstra(i, j, k)});

      currentRoute["src"] = i;
      currentRoute["dst"] = j;
      currentRoute["paths"] = currentPaths;

      jsonFile["routes"].push_back(currentRoute);
    }
  }

  std::ofstream file(filename);
  file << std::setw(4) << jsonFile << std::endl;
}