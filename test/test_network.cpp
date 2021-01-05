// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include <vector>

#include "../src/network.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Network)") { CHECK_NOTHROW(Network()); }

TEST_CASE("Constructor (Network) with JSON file") {
  CHECK_NOTHROW(Network("../networks/NSFNet.json"));
}

TEST_CASE("Getting Node position (Network)") {
  Network n1 = Network();
  n1.addNode(Node());
  CHECK(n1.getNode(0).getId() == -1);
  CHECK_THROWS(n1.getNode(-1));
  CHECK_THROWS(n1.getNode(50));
};

TEST_CASE("Getting Link position (Network)") {
  Network n1 = Network();
  n1.addLink(Link());
  CHECK(n1.getLink(0)->getId() == -1);
  CHECK_THROWS(n1.getLink(-1));
  CHECK_THROWS(n1.getLink(-50));
};

TEST_CASE("Copy constructor (Network)") {
  Network n1 = Network();
  n1.addNode(Node());
  n1.addLink(Link());
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
  net1.addNode(Node(5));
  n1_pos = net1.getNode(5).getId();
  net1.addNode(Node(6));
  n2_pos = net1.getNode(6).getId();
  net1.addLink(Link(0));
  l1_pos = net1.getLink(0)->getId();
  net1.connect(n1_pos, l1_pos, n2_pos);
};

TEST_CASE("Verify if two nodes are connected (Network)") {
  int n0_id;
  int n1_id;
  int n2_id;
  int n3_id;
  int n4_id;
  int l0_id;
  int l1_id;
  int l2_id;
  int l3_id;
  int l4_id;
  Network net1 = Network();
  net1.addNode(Node());
  net1.addNode(Node());
  net1.addNode(Node());
  net1.addNode(Node());
  net1.addNode(Node());
  net1.addLink(Link());
  net1.addLink(Link());
  net1.addLink(Link());
  net1.addLink(Link());
  net1.addLink(Link());
  n0_id = net1.getNode(0).getId();
  n1_id = net1.getNode(1).getId();
  n2_id = net1.getNode(2).getId();
  n3_id = net1.getNode(3).getId();
  n4_id = net1.getNode(4).getId();
  l0_id = net1.getLink(0)->getId();
  l1_id = net1.getLink(1)->getId();
  l2_id = net1.getLink(2)->getId();
  l3_id = net1.getLink(3)->getId();
  l4_id = net1.getLink(4)->getId();
  net1.connect(n0_id, l0_id, n1_id);
  /*net1.connect(n1_id, l1_id, n2_id);
  net1.connect(n2_id, l2_id, n3_id);
  net1.connect(n3_id, l3_id, n4_id);
  net1.connect(n4_id, l4_id, n0_id); */

  CHECK(net1.isConnected(n0_id, n1_id) == true);
  /*CHECK(net1.isConnected(n2_id, n3_id) == true);
  CHECK(net1.isConnected(n3_id, n4_id) == true);
  CHECK(net1.isConnected(n4_id, n1_id) == true);
  CHECK(net1.isConnected(n1_id, n0_id) == true); */
}

TEST_CASE("JSON constructor") {
  Network n1 = Network("../networks/NSFNet_routes.json");
}
