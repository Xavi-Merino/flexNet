#include "controller.hpp"

#include <fstream>

Controller::Controller() {
  this->connections = std::vector<Connection>();
  this->network = nullptr;
  this->allocator = new Allocator;
};

Controller::Controller(Network *network) {
  this->network = network;
  this->connections = std::vector<Connection>();
  this->allocator = new Allocator;
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
                                              long long idConnection) {
  Connection con = Connection(idConnection);
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

int Controller::unassignConnection(long long idConnection) {
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

void Controller::setPaths(std::string filename) {
  // open JSON file
  std::ifstream file(filename);
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
