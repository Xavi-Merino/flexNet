#include "controller.hpp"

#include <fstream>

Controller::Controller() { this->connections = std::vector<Connection>(); };

Controller::Controller(Network *network) {
  this->network = network;
  this->connections = std::vector<Connection>();
};

Controller::~Controller() {
  for (int i = 0; i < this->network->getNumberOfLinks(); i++) {
    delete this->network->getLink(i);
  }
  for (int i = 0; i < this->network->getNumberOfNodes(); i++) {
    delete this->network->getNode(i);
  }
};

allocationStatus Controller::assignConnection(int src, int dst, int bitRate,
                                              long long idConnection) {
  Connection con = Connection(idConnection);
  this->rtnAllocation = this->allocator->exec(src, dst, bitRate, con);
  if (this->rtnAllocation == ALLOCATED) {
    this->connections.push_back(con);
  }
  return this->rtnAllocation;
}

int Controller::unassignConnection(long long idConnection) {
  for (unsigned int i = 0; i < this->connections.size(); i++) {
    if (this->connections[i].id == idConnection) {
      for (unsigned int j = 0; j < this->connections[i].links.size(); j++) {
        for (unsigned int k = 0; k < this->connections[i].slots[j].size();
             j++) {
          this->network->unuseSlot(connections[i].links[j],
                                   this->connections[i].slots[j][k]);
        }
      }
    }
  }
  return 0;
}

void Controller::setPaths(std::string filename) {
  // open JSON file
  std::ifstream file(filename);
  nlohmann::json NSFnet;
  file >> NSFnet;

  int numberOfNodes;
  numberOfNodes = this->network->getNumberOfNodes();

  // allocate space for path[src]
  this->path.resize(numberOfNodes);

  // allocate space for path[src][dst]
  for (int t = 0; t < numberOfNodes; t++) {
    this->path[t].resize(numberOfNodes);
  }

  int routesNumber;
  routesNumber = NSFnet["routes"].size();

  for (int i = 0; i < routesNumber; i++) {
    int pathsNumber;
    pathsNumber = NSFnet["routes"][i]["paths"].size();
    int src, dst;
    src = NSFnet["routes"][i]["src"];
    dst = NSFnet["routes"][i]["dst"];

    // allocate path[src][dst][pathsNumber]
    int linksInPath;
    linksInPath = pathsNumber - 1;
    // beacuase 3 nodes has 2 links
    this->path[src][dst].resize(pathsNumber);

    // go through available routes
    for (int b = 0; b < pathsNumber; b++) {
      int nodesPathNumber;
      nodesPathNumber = NSFnet["routes"][i]["paths"][b].size();
      int lastNode = nodesPathNumber - 1;

      for (int c = 0; c < lastNode; c++) {
        int actNode, nextNode;
        actNode = NSFnet["routes"][i]["paths"][b][c];
        nextNode = NSFnet["routes"][i]["paths"][b][c + 1];

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
