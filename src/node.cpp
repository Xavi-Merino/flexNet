#include "node.hpp"

Node::Node(void) {
  this->id = -1;
  this->label = "";
}

Node::Node(int id) {
  this->id = id;
  this->label = "";
}

Node::Node(int id, std::string label) {
  this->id = id;
  this->label = label;
}

Node::~Node() {}

void Node::setId(int id) { this->id = id; }

int Node::getId(void) const { return this->id; }

void Node::setLabel(std::string label) { this->label = label; }

std::string Node::getLabel(void) const { return this->label; }