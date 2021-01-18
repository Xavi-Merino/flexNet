#include "network.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>

Network::Network(void) {
  this->link_counter = 0;
  this->node_counter = 0;

  this->nodes = std::vector<Node *>();
  this->links = std::vector<Link *>();
  this->links_in = std::vector<Link *>();
  this->links_out = std::vector<Link *>();
  this->nodes_in = std::vector<int>();
  this->nodes_out = std::vector<int>();

  this->nodes_in.push_back(0);
  this->nodes_out.push_back(0);
}

Network::Network(std::string filename) {
  this->link_counter = 0;
  this->node_counter = 0;

  this->nodes = std::vector<Node *>();
  this->links = std::vector<Link *>();
  this->links_in = std::vector<Link *>();
  this->links_out = std::vector<Link *>();
  this->nodes_in = std::vector<int>();
  this->nodes_out = std::vector<int>();

  this->nodes_in.push_back(0);
  this->nodes_out.push_back(0);

  // open JSON file
  std::ifstream file(filename);
  nlohmann::json NSFnet;
  file >> NSFnet;

  // number of nodes
  int cantNodos = NSFnet["nodes"].size();

  // number of links
  int cantLinks = NSFnet["links"].size();

  // adding nodes to the netwrok
  for (int i = 0; i < cantNodos; i++) {
    int id;
    id = NSFnet["nodes"][i]["id"];
    Node *node = new Node(id);
    this->addNode(node);
  }

  // adding links to the network
  for (int i = 0; i < cantLinks; i++) {
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
  this->link_counter = 0;
  this->node_counter = 0;
  this->nodes = net.nodes;
  this->links = net.links;
  this->links_in = net.links_in;
  this->links_out = net.links_out;
  this->nodes_in = net.nodes_in;
  this->nodes_out = net.nodes_out;
}

Network::~Network() {
  for (int i = 0; i < this->link_counter; i++) {
    delete this->links[i];
  }
  for (int i = 0; i < this->node_counter; i++) {
    delete this->nodes[i];
  }
}

// May be useless
Node *Network::getNode(int pos) {
  if (pos < 0 || pos >= static_cast<int>(this->nodes.size()))
    throw std::runtime_error("Cannot get Node from a position out of bounds.");

  return this->nodes.at(pos);
}
// Returns the Node at a "pos" index inside Nodes vector.

// May be useless
Link *Network::getLink(int pos) {
  if (pos < 0 || pos >= static_cast<int>(this->links.size()))
    throw std::runtime_error("Cannot get Link from a position out of bounds.");

  return this->links.at(pos);
}
// Returns the Link pointer at a "pos" index inside Links vector.

void Network::addNode(Node *node) {
  if (node->getId() != this->node_counter) {
    throw std::runtime_error(
        "Cannot add a Node to this network with Id mismatching node counter.");
  }
  this->node_counter++;
  this->nodes.push_back(node);
  this->nodes_in.push_back(0);
  this->nodes_out.push_back(0);
}
// Add a Node to Nodes vector, increases Nodes_In/Out size.

void Network::addLink(Link *link) {
  if (link->getId() != Network::link_counter) {
    throw std::runtime_error(
        "Cannot add a Link to this network with Id mismatching link counter.");
  }
  this->link_counter++;
  this->links.push_back(link);
}
// Add a Link to Links vector.

void Network::connect(int src, int link,
                      int dst)  // Using Ids and Link from Nodes/Links vectors
{
  if (src < 0 || src >= this->node_counter) {
    throw std::runtime_error(
        "Cannot connect src " + std::to_string(src) +
        " because its ID is not in the network. Number of nodes in network: " +
        std::to_string(this->node_counter));
  }
  if (dst < 0 || dst >= this->node_counter) {
    throw std::runtime_error(
        "Cannot connect dst " + std::to_string(dst) +
        " because its ID is not in the network. Number of nodes in network: " +
        std::to_string(this->node_counter));
  }
  if (link < 0 || link >= this->link_counter) {
    throw std::runtime_error(
        "Cannot use link " + std::to_string(link) +
        " because its ID is not in the network. Number of links in network: " +
        std::to_string(this->link_counter));
  }
  this->links_out.insert(this->links_out.begin() + this->nodes_out.at(src),
                         this->links.at(link));
  std::for_each(this->nodes_out.begin() + src + 1, this->nodes_out.end(),
                [](int &n) { n += 1; });

  this->links_in.insert(this->links_in.begin() + this->nodes_in.at(dst),
                        this->links.at(link));
  std::for_each(this->nodes_in.begin() + dst + 1, this->nodes_in.end(),
                [](int &n) { n += 1; });
}
// Connects two Nodes through one Link (order is important: src != dst):
//
//       (Source Node) ---Link---> (Destination Node)

int Network::isConnected(int src, int dst) {
  for (int i = this->nodes_out[src]; i < this->nodes_out[src + 1]; i++) {
    for (int j = nodes_in[dst]; j < nodes_in[dst + 1]; j++) {
      if (links_out[i]->getId() == links_in[j]->getId()) {
        return links_out[i]->getId();
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

  for (int i = slotFrom; i < slotTo; i++)
    this->links[linkPos]->setSlot(i, false);
}

int Network::getNumberOfLinks() { return this->link_counter; }

int Network::getNumberOfNodes() { return this->node_counter; }

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
    for (int i = node_counter; i != this->nodes.size(); ++i)
      distanceClassUntil(i, d, path_counter);
  }
};
*/