// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include <vector>

#include "../src/network.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Network)") { CHECK_NOTHROW(Network()); }

TEST_CASE("Constructor (Network)") {
  CHECK_NOTHROW(Network("../networks/NSFNet.json"));
}

TEST_CASE("Getting Node position (Network)") {
  Network n1 = Network();
  n1.addNode(Node(0));
  CHECK(n1.getNode(0).getId() == 0);
  CHECK_THROWS(n1.getNode(1));
};

TEST_CASE("Getting Link position (Network)") {
  Network n1 = Network();
  n1.addLink(Link(0));
  CHECK(n1.getLink(0)->getId() == 0);
  CHECK_THROWS(n1.getLink(1));
};
TEST_CASE("Connect source and destination nodes (Network)") {
  int n1_pos;
  int n2_pos;
  int l1_pos;
  Network net1 = Network();
  net1.addNode(Node(0));
  net1.addNode(Node(1));
  net1.addLink(Link(0));
  n1_pos = net1.getNode(0).getId();
  n2_pos = net1.getNode(1).getId();
  l1_pos = net1.getLink(0)->getId();
  net1.connect(n1_pos, l1_pos, n2_pos);
};

TEST_CASE("Verify if two nodes are connected (Network)") {
  int n0_id;
  int n1_id;
  /*int n2_id;
  int n3_id;
  int n4_id;*/
  int l0_id;
  /*Link *l1_id;
  Link *l2_id;
  Link *l3_id;
  Link *l4_id; */
  Network net1 = Network();
  net1.addNode(Node(0));
  net1.addNode(Node(1));
  /*net1.addNode(Node(2));
  net1.addNode(Node(3));
  net1.addNode(Node(4));*/
  net1.addLink(Link(0));
  /*net1.addLink(Link(1));
  net1.addLink(Link(2));
  net1.addLink(Link(3));
  net1.addLink(Link(4)); */
  n0_id = net1.getNode(0).getId();
  n1_id = net1.getNode(1).getId();
  /*n2_id = net1.getNode(2).getId();
  n3_id = net1.getNode(3).getId();
  n4_id = net1.getNode(4).getId();*/
  l0_id = net1.getLink(0)->getId();
  /*l1_id = net1.getLink(1);
  l2_id = net1.getLink(2);
  l3_id = net1.getLink(3);
  l4_id = net1.getLink(4); */
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
  Network n1 = Network("../networks/5nodeBus.json");
}
