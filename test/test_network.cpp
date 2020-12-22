// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include "../src/network.hpp"
#include "catch.hpp"
#include <vector>

TEST_CASE("Constructor (Network)") {
  CHECK_NOTHROW(Network());
  CHECK_NOTHROW(Network("../networks/NFSNet.json"));
}

TEST_CASE("Getting Node position (Network)") {
  Network n1 = Network();
  n1.addNode(Node(20));
  CHECK(n1.getNode(0).getId() == 20);
  CHECK_THROWS(n1.getNode(1));
}
/*
CHECK(n2.getNode() == 50);
}

TEST_CASE("Getting Link position (Network)") {
  Link l1 = Link();
  Link l2 = Link(50);

  CHECK(n1.getLink() == -1);
  CHECK(n2.getLink() == 50);
}

TEST_CASE("Add a Node to Node vector"){
  Node n1 = Node(0);
  Node n2 = Node(1);
  Node n3 = Node(2);
  Node n4 = Node(3);
  Node n5 = Node(4);
  std::vector<int> vec1 = {n1,n2,n3,n4,n5};
  Network::addNode(n1);
  Network::addNode(n2);
  Network::addNode(n3);
  Network::addNode(n4);
  Network::addNode(n5);
  CHECK(vec1 == Network::nodes);

}

TEST_CASE("Add a Link to Link vector"){
  Link l1 = Link(0);
  Link l2 = Link(1);
  Link l3 = Link(2);
  Link l4 = Link(3);
  Link l5 = Link(4);
  std::vector<int> vec2 = {n1,n2,n3,n4,n5};
  Network::addLink(l1);
  Network::addLink(l2);
  Network::addLink(l3);
  Network::addLink(l4);
  Network::addLink(l5);
  CHECK(vec2 == Network::links)
}
TEST_CASE("Connect source and destination nodes")
TEST_CASE("Verify if two nodes are connected")


*/




