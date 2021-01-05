// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include <vector>

#include "../src/network.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Network)") { CHECK_NOTHROW(Network()); }

TEST_CASE("Constructor (Network) with JSON file") {
  CHECK_NOTHROW(Network("../networks/NSFNet.json"));
};

TEST_CASE("Getting Node position (Network)") {
  Network n1 = Network();
  n1.addNode(Node(0));
  CHECK(n1.getNode(0).getId() == 0);
  CHECK_THROWS(n1.getNode(-1));
  CHECK_THROWS(n1.getNode(50));
};

TEST_CASE("Getting Link position (Network)") {
  Network n1 = Network();
  n1.addLink(Link(0));
  CHECK(n1.getLink(0)->getId() == 0);
  CHECK_THROWS(n1.getLink(-1));
  CHECK_THROWS(n1.getLink(-50));
};

TEST_CASE("Copy constructor (Network)") {
  Network n1 = Network();
  n1.addNode(Node(0));
  n1.addLink(Link(0));
  Network n2 = Network(n1);
  CHECK(n1.getNode(0).getId() == n2.getNode(0).getId());
  CHECK(n1.getLink(0)->getId() == n2.getLink(0)->getId());
  CHECK_THROWS(n1.getNode(-1).getId() == n2.getNode(-1).getId());
};

TEST_CASE("Connect source and destination nodes (Network)") {
  int n1_pos;
  int n2_pos;
  int l1_pos;
  Network net1 = Network();
  net1.addNode(Node(0));
  n1_pos = net1.getNode(0).getId();
  net1.addNode(Node(1));
  n2_pos = net1.getNode(1).getId();
  net1.addLink(Link(0));
  l1_pos = net1.getLink(0)->getId();
  net1.connect(n1_pos, l1_pos, n2_pos);
  CHECK(net1.isConnected(n1_pos, n2_pos) == true);
};

TEST_CASE("JSON constructor") {
  Network n1 = Network("../networks/NSFNet_routes.json");
}

TEST_CASE("Use slot (Network)") {
  Network net1 = Network();
  Link l1 = Link(20, 70.0, 200);
  CHECK(l1.getSlot(100) == false);
  net1.useSlot(0, 100);
  CHECK(l1.getSlot(100) == true);
}