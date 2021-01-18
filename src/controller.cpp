#include "controller.hpp"

#include <fstream>

Controller::Controller(){};

Controller::Controller(Network network){};

Controller::~Controller(){};

int Controller::assignConnection(int src, int dst, int numberOfSlots) {
  return 0;
};

void Controller::setPaths(std::string filename) {
  // open JSON file
  std::ifstream file(filename);
  nlohmann::json NSFnet;
  file >> NSFnet;

  int numberOfNodes;
  numberOfNodes = this->network.getNumberOfNodes();

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
    this->path[src][dst].resize(linksInPath);

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
        idLink = this->network.isConnected(actNode, nextNode);

        this->path[src][dst][b].push_back(this->network.getLink(idLink));
      }
    }
  }
}

void Controller::setNetwork(Network network) { this->network = network; };