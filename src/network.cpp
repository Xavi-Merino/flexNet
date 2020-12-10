#include "network.hpp"

Network::Network(void) {}

Network::~Network() {}

void Network::addNode(Node node) { this->nodes.push_back(node); }

void Network::addLink(Link link) { this->links.push_back(link); }