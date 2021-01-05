#include "network.hpp"

#include <algorithm>
#include <fstream>
#include <unordered_map>

int Network::link_counter = 0;
int Network::node_counter = 0;

Network::Network(void) {}

Network::Network(std::string filename) {
  // open JSON file
  std::ifstream file(filename);
  nlohmann::json NSFnet;
  file >> NSFnet;

  // allocate space based on the number of nodes
  int cantNodos = NSFnet["nodes"].size();
  this->nodes.resize(cantNodos);

  // allocate space based on the number of links
  int cantLinks = NSFnet["links"].size();
  this->links.resize(cantLinks);
  this->nodes_in.resize(cantLinks);
  this->nodes_out.resize(cantLinks);

  for (int i = 0; i < cantNodos; i++) {
    int id;
    id = NSFnet["nodes"][i]["id"];
    Node node = Node(id);
    this->addNode(node);
  }

  for (int i = 0; i < cantLinks; i++) {
    int id;
    id = NSFnet["links"][i]["id"];
    float lenght;
    lenght = NSFnet["links"][i]["lenght"];
    Link link = Link(id, lenght);
    this->addLink(link);
  }
}

Network::Network(const Network &net) {
  this->nodes = net.nodes;
  this->links = net.links;
  this->links_in = net.links_in;
  this->links_out = net.links_out;
  this->nodes_in = net.nodes_in;
  this->nodes_out = net.nodes_out;
}

Network::~Network() {}

// May be useless
Node Network::getNode(int pos) {
  if (pos < 0 || pos >= this->nodes.size())
    throw std::runtime_error("Cannot get Node from a position out of bounds.");

  return this->nodes.at(pos);
}
// Returns the Node at a "pos" index inside Nodes vector.

// May be useless
Link *Network::getLink(int pos) {
  if (pos < 0 || pos >= this->links.size())
    throw std::runtime_error("Cannot get Link from a position out of bounds.");

  return &(this->links.at(pos));
}
// Returns the Link pointer at a "pos" index inside Links vector.

void Network::addNode(Node node) {
  if (node.getId() != Network::node_counter) {
    throw std::runtime_error(
        "Cannot add a Node to this network with Id mismatching node counter.");
  }
  this->nodes.push_back(node);
  this->nodes_in.push_back(0);
  this->nodes_out.push_back(0);
}
// Add a Node to Nodes vector, increases Nodes_In/Out size.

void Network::addLink(Link link) { this->links.push_back(link); }
// Add a Link to Links vector.

void Network::connect(int src, int link,
                      int dst)  // Using Ids and Link from Nodes/Links vectors
{
  this->links_out.insert(this->links_out.begin() + this->nodes_out.at(src),
                         &this->links.at(link));
  std::for_each(this->nodes_out.begin() + src, this->nodes_out.end(),
                [](int &n) { n += 1; });

  this->links_in.insert(this->links_in.begin() + this->nodes_in.at(dst),
                        &this->links.at(link));
  std::for_each(this->nodes_in.begin() + dst, this->nodes_in.end(),
                [](int &n) { n += 1; });
}
// Connects two Nodes through one Link (order is important: src != dst):
//
//       (Source Node) ---Link---> (Destination Node)

bool Network::isConnected(int src, int dst) {
  std::unordered_map<int, int> hash;

  for (int j = nodes_out[src - 1]; j < nodes_out[src]; j++) {
    hash[links_out[j]->getId()]++;

    if (hash[links_out[j]->getId()] == 2) return true;
  }

  for (int j = nodes_in[dst - 1]; j < nodes_in[dst]; j++) {
    hash[links_in[j]->getId()]++;

    if (hash[links_in[j]->getId()] == 2) return true;
  }

  return false;
}

void Network::useSlot(int linkPos, int slotPos) {
  this->links[linkPos].setSlot(slotPos, true);
}

void Network::useSlot(int linkPos, int slotPos1, int slotPos2) {
  // TODO: Check if slotPos1 < slotPos2
  for (int i = slotPos1; i < slotPos2; i++)
    this->links[linkPos].setSlot(i, true);
}