#include "network.hpp"

#include <algorithm>

Network::Network(void) {}

Network::Network(std::string filename) {}

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
