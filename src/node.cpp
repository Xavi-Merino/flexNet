#include "node.hpp"

int node_counter = 0;

Node::Node(void) {
  this->id = -1;
  this->label = "";
}

Node::Node(int id) {
  if (id != node_counter)
    throw std::runtime_error(
        "Cannot create a Node with Id mismatching node counter.");
  this->id = id;
  node_counter++;

  this->label = "";
}

Node::Node(int id, std::string label) {
  if (id != node_counter)
    throw std::runtime_error(
        "Cannot create a Node with Id mismatching node counter.");
  this->id = id;
  node_counter++;

  this->label = label;
}

Node::~Node() {}

void Node::setId(int id) {
  if (this->id != -1)
    throw std::runtime_error(
        "Cannot set Id to a Node with Id different than -1.");

  if (id != node_counter)
    throw std::runtime_error(
        "Cannot set a Node with Id mismatching node counter.");
  this->id = id;
  node_counter++;
}

int Node::getId(void) const { return this->id; }

void Node::setLabel(std::string label) { this->label = label; }

std::string Node::getLabel(void) const { return this->label; }