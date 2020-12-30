#include "network.hpp"

#include <algorithm>
#include <fstream>

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

Network::~Network() {}

Node Network::getNode(int pos) {
  if (pos < 0 || pos >= this->nodes.size())
    throw std::runtime_error("Cannot get Node from a position out of bounds.");

  return this->nodes.at(pos);
}
// Returns the Node at a "pos" index inside Nodes vector.

Link* Network::getLink(int pos) {
  if (pos < 0 || pos >= this->links.size())
    throw std::runtime_error("Cannot get Link from a position out of bounds.");

  return &(this->links.at(pos));
}
// Returns the Link pointer at a "pos" index inside Links vector.

void Network::addNode(Node node) {
  this->nodes.push_back(node);
  this->nodes_in.push_back(0);
  this->nodes_out.push_back(0);
}
// Add a Node to Nodes vector, increases Nodes_In/Out size.

void Network::addLink(Link link) { this->links.push_back(link); }
// Add a Link to Links vector.

void Network::connect(int src, Link* ptr, int dst)
// Using Nodes Ids and Link from Nodes/Links vectors
{
  for (int i = 0; i < this->nodes.size(); i++)
  // Traverse nodes array searching for matching Id (for both src and dst)
  {
    if (this->nodes[i].getId() == src) {
      this->links_out.insert(this->links_out.begin() + this->nodes_out.at(i),
                             ptr);
      std::for_each(this->nodes_out.begin() + i, this->nodes_out.end(),
                    [](int& n) { n += 1; });
    }
    // If finds matching Id for src:
    //-inserts Link address into Links_Outvector
    //-increases numbers inside Nodes_Out (offset positions for Links_Out)

    if (this->nodes[i].getId() == dst) {
      this->links_in.insert(this->links_in.begin() + this->nodes_in.at(i), ptr);
      std::for_each(this->nodes_in.begin() + i, this->nodes_in.end(),
                    [](int& n) { n += 1; });
    }
    // If finds matching Id for dst:
    //-inserts Link address into Links_In vector
    //-increases numbers inside Nodes_In (offset positions for Links_In)
  }
}
// Connects two Nodes through one Link (order is important: src != dst):
//
//       (Source Node) ---Link---> (Destination Node)

bool Network::isConnected(int id1, int id2) {
  // Check if two Nodes are directly connected.
  std::vector<int> numbers;  // Int vector to add all links' Ids

  for (int i = 0; i < this->nodes.size(); i++) {  // Traverse all nodes
    if (this->nodes[i].getId() ==
        id1) {  // If finds matching Id with node Id 1...
      for (int j = nodes_out[i - 1]; j < nodes_out[i]; j++)
        numbers.push_back(
            links_out[j]->getId());  //...Add all it's outcoming links Ids

      for (int j = nodes_in[i - 1]; j < nodes_in[i]; j++)
        numbers.push_back(
            links_in[j]->getId());  //...Add all it's incoming links Ids
    }

    if (this->nodes[i].getId() ==
        id2) {  // If finds matching Id with node Id 2...
      for (int j = nodes_out[i - 1]; j < nodes_out[i]; j++)
        numbers.push_back(
            links_out[j]->getId());  //...Add all it's outcoming links Ids

      for (int j = nodes_in[i - 1]; j < nodes_in[i]; j++)
        numbers.push_back(
            links_in[j]->getId());  //...Add all it's incoming links Ids
    }
  }

  std::sort(numbers.begin(), numbers.end());  // Sort links Ids
  for (int i = 0; i < numbers.size() - 1; i++) {
    if (numbers[i] == numbers[i + 1]) {  // If there are two equal Ids...
      return true;                       //...both nodes are connected.
    }
  }
  return false;  // Otherwise false.
}