// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include <vector>

#include "../src/network.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Network)") {
  CHECK_NOTHROW(Network());
  CHECK_NOTHROW(Network("../networks/NSFNet.json"));
}

TEST_CASE("Getting Node position (Network)") {
  Network n1 = Network();
  n1.addNode(Node(20));
  CHECK(n1.getNode(0).getId() == 20);
  CHECK_THROWS(n1.getNode(1));
};

TEST_CASE("Getting Link position (Network)") {
  Network n1 = Network();
  n1.addLink(Link(20));
  CHECK(n1.getLink(0)->getId() == 20);
  CHECK_THROWS(n1.getLink(1));
};
TEST_CASE("Connect source and destination nodes") {
  int n1_pos;
  int n2_pos;
  Link *l1_pos;
  Network net1 = Network();
  net1.addNode(Node(0));
  net1.addNode(Node(1));
  net1.addLink(Link(0));
  n1_pos = net1.getNode(0).getId();
  n2_pos = net1.getNode(1).getId();
  l1_pos = net1.getLink(0);
  net1.connect(n1_pos, l1_pos, n2_pos);
};

TEST_CASE("Verify if two nodes are connected") {
  int node1_id;
  int node2_id;
  Network n1 = Network("../networks/5nodeBus.json");
  node1_id = n1.getNode(0).getId();
  node2_id = n1.getNode(2).getId();
  CHECK(n1.isConnected(node1_id, node2_id) == true);
}
