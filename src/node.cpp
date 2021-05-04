#include "node.hpp"

int node_counter = 0;

Node::Node(void) {
  this->id = -1;
  this->label = "";
}

Node::Node(int id) {
  this->id = id;
  this->label = "";
  // node_counter++;
}

Node::Node(int id, std::string label) {
  this->id = id;
  node_counter++;

  this->label = label;
}

Node::~Node() {}

void Node::setId(int id) {
  if (this->id != -1)
    throw std::runtime_error(
        "Cannot set Id to a Node with Id different than -1.");
  this->id = id;
  node_counter++;
}

int Node::getId(void) const { return this->id; }

void Node::setLabel(std::string label) { this->label = label; }

std::string Node::getLabel(void) const { return this->label; }