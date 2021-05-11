#include "network.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>

Network::Network(void) {
  this->linkCounter = 0;
  this->nodeCounter = 0;

  this->nodes = std::vector<Node *>();
  this->links = std::vector<Link *>();
  this->linksIn = std::vector<Link *>();
  this->linksOut = std::vector<Link *>();
  this->nodesIn = std::vector<int>();
  this->nodesOut = std::vector<int>();

  this->nodesIn.push_back(0);
  this->nodesOut.push_back(0);
}

Network::Network(std::string filename) {
  this->linkCounter = 0;
  this->nodeCounter = 0;

  this->nodes = std::vector<Node *>();
  this->links = std::vector<Link *>();
  this->linksIn = std::vector<Link *>();
  this->linksOut = std::vector<Link *>();
  this->nodesIn = std::vector<int>();
  this->nodesOut = std::vector<int>();

  this->nodesIn.push_back(0);
  this->nodesOut.push_back(0);

  // open JSON file
  std::ifstream file(filename);
  nlohmann::json NSFnet;
  file >> NSFnet;

  // number of nodes
  int numberOfNodes = NSFnet["nodes"].size();

  // number of links
  int numberOfLinks = NSFnet["links"].size();

  // adding nodes to the network
  for (int i = 0; i < numberOfNodes; i++) {
    int id;
    id = NSFnet["nodes"][i]["id"];
    Node *node = new Node(id);
    this->addNode(node);
  }

  // adding links to the network
  for (int i = 0; i < numberOfLinks; i++) {
    int id;
    id = NSFnet["links"][i]["id"];
    float lenght;
    lenght = NSFnet["links"][i]["lenght"];
    float slots;
    slots = NSFnet["links"][i]["slots"];

    Link *link = new Link(id, lenght, slots);
    this->addLink(link);

    // connecting nodes
    int src, dst;
    src = NSFnet["links"][i]["src"];
    id = NSFnet["links"][i]["id"];
    dst = NSFnet["links"][i]["dst"];
    this->connect(src, id, dst);
  }
}

Network::Network(const Network &net) {
  this->linkCounter = net.linkCounter;
  this->nodeCounter = net.nodeCounter;
  this->nodes = std::vector<Node *>(net.nodes.size());
  for (unsigned int i = 0; i < this->nodes.size(); i++) {
    this->nodes[i] = net.nodes[i];
  }
  this->links = std::vector<Link *>(net.links.size());
  for (unsigned int i = 0; i < this->links.size(); i++) {
    this->links[i] = net.links[i];
  }
  this->linksIn = std::vector<Link *>(net.linksIn.size());
  for (unsigned int i = 0; i < this->linksIn.size(); i++) {
    this->linksIn[i] = net.linksIn[i];
  }
  this->linksOut = std::vector<Link *>(net.linksOut.size());
  for (unsigned int i = 0; i < this->linksOut.size(); i++) {
    this->linksOut[i] = net.linksOut[i];
  }
  this->nodesIn = net.nodesIn;
  this->nodesOut = net.nodesOut;
}

Network::~Network() {}

// May be useless
Node *Network::getNode(int nodePos) {
  if (nodePos < 0 || nodePos >= static_cast<int>(this->nodes.size()))
    throw std::runtime_error("Cannot get Node from a position out of bounds.");

  return this->nodes.at(nodePos);
}
// Returns the Node at a "nodePos" index inside Nodes vector.

// May be useless
Link *Network::getLink(int linkPos) {
  if (linkPos < 0 || linkPos >= static_cast<int>(this->links.size()))
    throw std::runtime_error("Cannot get Link from a position out of bounds.");

  return this->links.at(linkPos);
}
// Returns the Link pointer at a "linkPos" index inside Links vector.

void Network::addNode(Node *node) {
  if (node->getId() != this->nodeCounter) {
    throw std::runtime_error(
        "Cannot add a Node to this network with Id mismatching node counter.");
  }
  this->nodeCounter++;
  this->nodes.push_back(node);
  this->nodesIn.push_back(0);
  this->nodesOut.push_back(0);
}
// Add a Node to Nodes vector, increases nodesIn/Out size.

void Network::addLink(Link *link) {
  if (link->getId() != Network::linkCounter) {
    throw std::runtime_error(
        "Cannot add a Link to this network with Id mismatching link counter.");
  }
  this->linkCounter++;
  this->links.push_back(link);
}
// Add a Link to Links vector.

void Network::connect(int src, int linkPos,
                      int dst)  // Using Ids and Link from Nodes/Links vectors
{
  if (src < 0 || src >= this->nodeCounter) {
    throw std::runtime_error(
        "Cannot connect src " + std::to_string(src) +
        " because its ID is not in the network. Number of nodes in network: " +
        std::to_string(this->nodeCounter));
  }
  if (dst < 0 || dst >= this->nodeCounter) {
    throw std::runtime_error(
        "Cannot connect dst " + std::to_string(dst) +
        " because its ID is not in the network. Number of nodes in network: " +
        std::to_string(this->nodeCounter));
  }
  if (linkPos < 0 || linkPos >= this->linkCounter) {
    throw std::runtime_error(
        "Cannot use link " + std::to_string(linkPos) +
        " because its ID is not in the network. Number of links in network: " +
        std::to_string(this->linkCounter));
  }
  this->linksOut.insert(this->linksOut.begin() + this->nodesOut.at(src),
                        this->links.at(linkPos));
  std::for_each(this->nodesOut.begin() + src + 1, this->nodesOut.end(),
                [](int &n) { n += 1; });

  this->linksIn.insert(this->linksIn.begin() + this->nodesIn.at(dst),
                       this->links.at(linkPos));
  std::for_each(this->nodesIn.begin() + dst + 1, this->nodesIn.end(),
                [](int &n) { n += 1; });
  this->links.at(linkPos)->src = src;
  this->links.at(linkPos)->dst = dst;
}
// Connects two Nodes through one Link (order is important: src != dst):
//
//       (Source Node) ---Link---> (Destination Node)

int Network::isConnected(int src, int dst) {
  for (int i = this->nodesOut[src]; i < this->nodesOut[src + 1]; i++) {
    for (int j = nodesIn[dst]; j < nodesIn[dst + 1]; j++) {
      if (linksOut[i]->getId() == linksIn[j]->getId()) {
        return linksOut[i]->getId();
      }
    }
  }
  return -1;
}

void Network::useSlot(int linkPos, int slotPos) {
  if (linkPos < 0 || linkPos >= static_cast<int>(this->links.size()))
    throw std::runtime_error("Link position out of bounds.");

  this->links[linkPos]->setSlot(slotPos, true);
}

void Network::useSlot(int linkPos, int slotFrom, int slotTo) {
  if (linkPos < 0 || linkPos >= static_cast<int>(this->links.size()))
    throw std::runtime_error("Link position out of bounds.");

  if (slotFrom > slotTo)
    throw std::runtime_error(
        "Initial slot position must be lower than the final slot position.");

  for (int i = slotFrom; i < slotTo; i++)
    this->links[linkPos]->setSlot(i, true);
}

void Network::unuseSlot(int linkPos, int slotPos) {
  if (linkPos < 0 || linkPos >= static_cast<int>(this->links.size()))
    throw std::runtime_error("Link position out of bounds.");

  this->links[linkPos]->setSlot(slotPos, false);
}

void Network::unuseSlot(int linkPos, int slotFrom, int slotTo) {
  if (linkPos < 0 || linkPos >= static_cast<int>(this->links.size()))
    throw std::runtime_error("Link position out of bounds.");

  if (slotFrom > slotTo)
    throw std::runtime_error(
        "Initial slot position must be lower than the final slot position.");
  if (slotFrom == slotTo)
    throw std::runtime_error("Slot from and slot To cannot be equals.");

  for (int i = slotFrom; i < slotTo; i++)
    this->links[linkPos]->setSlot(i, false);
}

int Network::getNumberOfLinks() { return this->linkCounter; }

int Network::getNumberOfNodes() { return this->nodeCounter; }

bool Network::isSlotUsed(int linkPos, int slotPos) {
  if (linkPos < 0 || linkPos >= static_cast<int>(this->links.size()))
    throw std::runtime_error("Link position out of bounds.");

  if (slotPos < 0 ||
      slotPos >= static_cast<int>(this->links[linkPos]->getSlots()))
    throw std::runtime_error("slot position out of bounds.");
  return this->links[linkPos]->getSlot(slotPos);
}

bool Network::isSlotUsed(int linkPos, int slotFrom, int slotTo) {
  if (linkPos < 0 || linkPos >= static_cast<int>(this->links.size()))
    throw std::runtime_error("Link position out of bounds.");

  if (slotFrom < 0 ||
      slotFrom >= static_cast<int>(this->links[linkPos]->getSlots()))
    throw std::runtime_error("slot position out of bounds.");
  if (slotTo < 0 ||
      slotTo >= static_cast<int>(this->links[linkPos]->getSlots()))
    throw std::runtime_error("slot position out of bounds.");
  if (slotFrom > slotTo)
    throw std::runtime_error(
        "Initial slot position must be lower than the final slot position.");

  if (slotFrom == slotTo)
    throw std::runtime_error("Slot from and slot To cannot be equals.");

  // Loop through all the Slots in range
  for (int i = slotFrom; i < slotTo; i++) {
    // If it finds a single used slot...
    if (this->links[linkPos]->getSlot(i)) {
      //...then the entire slot range is considered "used".
      return true;
    }
  }
  // Otherwise, the entire slot range is free to allocate.
  return false;
}

float Network::averageNeighborhood() {
  if (this->getNumberOfNodes() == 0)
    throw std::runtime_error("The network must be have at least one node.");
  float result = 0;
  result = this->getNumberOfLinks() / this->getNumberOfNodes();
  return result;
}

float Network::normalAverageNeighborhood() {
  if (this->getNumberOfNodes() == 0)
    throw std::runtime_error("The network must be have at least one node.");
  float result = 0;
  result = this->getNumberOfLinks() /
           (this->getNumberOfNodes() * (this->getNumberOfNodes() - 1));
  return result;
}

float Network::nodalVariance() {
  if (this->getNumberOfNodes() == 0)
    throw std::runtime_error("The network must be have at least one node.");
  float result = 0;
  float average = this->averageNeighborhood();
  for (int i = 0; i < this->getNumberOfNodes(); i++) {
    result += pow((this->nodesOut[i + 1] - this->nodesOut[i]) - average, 2);
  }
  result /= this->getNumberOfNodes();
  return result;
}

bool Network::existNodeIsolated() {
  if (this->getNumberOfNodes() == 0)
    throw std::runtime_error("The network must be have at least one node.");
  bool value = true;  // <del>
  for (int i = 0; i < this->getNumberOfNodes(); i++) {
    if (this->nodesIn[i] == this->nodesIn[i + 1]) {
      value = false;  // return false
      break;          // <del>
    }
  }
  return value;  // return true
}

bool Network::isGraphRelated() {
  if (this->getNumberOfNodes() == 0)
    throw std::runtime_error("The network must be have at least one node.");
  int _n = this->getNumberOfNodes();
  if (_n <= 1) return true;

  std::vector<bool> visit(_n);
  std::vector<bool>::iterator iter;

  for (iter = visit.begin(); iter != visit.end(); iter++) *iter = false;

  std::set<int> forvisit;
  std::set<int>::iterator current;
  int visitedId;
  forvisit.insert(0);
  while (!forvisit.empty()) {
    current = forvisit.begin();
    if (!visit[*current]) {
      for (int i = this->nodesOut[*current]; i < this->nodesOut[*current + 1];
           i++) {
        // TODO: visitedId, must be the destiny Node ID, not the link ID.
        // Waiting the src/dst feature.
        visitedId = this->linksOut[i]->getId();
        if (!visit[visitedId]) forvisit.insert(visitedId);
      }
      /*
      for (int i = 0; i < _n; i++) {
        if (_graph[*current][i] == 1 && !visit[i]) forvisit.insert(i);
      }*/
    }
    visit[*current] = true;
    forvisit.erase(current);
  }

  bool result;
  for (iter = visit.begin(); iter != visit.end(); iter++)
    result = result && *iter;
  return result;
}

/*
int Network::distanceClass(int src, int dst) {
  int path_counter = 0;
  distanceClassUntil(src, dst, path_counter);
  return path_counter;
};
*/
/*
void Network::distanceClassUntil(int s, int d, int &path_counter) {
  // If current node is same as destination,
  if (s == d) path_counter++;  // then increment count

  // If current node is not destination
  else {
    // Recur for all the nodes adjacent to
    // current node
    for (int i = nodeCounter; i != this->nodes.size(); ++i)
      distanceClassUntil(i, d, path_counter);
  }
};
*/