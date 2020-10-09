#include "node.hpp"

Node::Node() {
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

int Node::getId() { return this->id; }