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
  CHECK_THROWS(n1.addNode(new Node(10)));
  n1.addNode(new Node(0));
  CHECK(n1.getNode(0)->getId() == 0);
  CHECK_THROWS(n1.getNode(-1));
  CHECK_THROWS(n1.getNode(50));
};

TEST_CASE("Getting Link position (Network)") {
  Network n1 = Network();
  CHECK_THROWS(n1.addLink(new Link(10)));
  n1.addLink(new Link(0));
  CHECK(n1.getLink(0)->getId() == 0);
  CHECK_THROWS(n1.getLink(-1));
  CHECK_THROWS(n1.getLink(-50));
};

TEST_CASE("Copy constructor (Network)") {
  Network n1 = Network();
  n1.addNode(new Node(0));
  n1.addLink(new Link(0));
  Network n2 = Network(n1);
  CHECK(n1.getNode(0)->getId() == n2.getNode(0)->getId());
  CHECK(n1.getLink(0)->getId() == n2.getLink(0)->getId());
  CHECK_THROWS(n1.getNode(-1)->getId() == n2.getNode(-1)->getId());
};

TEST_CASE("Connect source and destination nodes (Network)") {
  int n1_pos;
  int n2_pos;
  int l1_pos;
  Network net1 = Network();
  net1.addNode(new Node(0));
  n1_pos = net1.getNode(0)->getId();
  net1.addNode(new Node(1));
  n2_pos = net1.getNode(1)->getId();
  net1.addLink(new Link(0));
  l1_pos = net1.getLink(0)->getId();
  net1.connect(n1_pos, l1_pos, n2_pos);
  CHECK_THROWS(net1.connect(-1, l1_pos, n2_pos));
  CHECK_THROWS(net1.connect(n1_pos, l1_pos, 3));
  CHECK_THROWS(net1.connect(n1_pos, 2, n2_pos));
  CHECK(net1.isConnected(n1_pos, n2_pos) == 0);
  CHECK(net1.getNumberOfLinks() == 1);
  CHECK(net1.getNumberOfNodes() == 2);
};

TEST_CASE("JSON constructor") {
  Network n1 = Network("../networks/NSFNet_routes.json");
}

TEST_CASE("Use slot (Network)") {
  Network net1 = Network();
  Link *l1 = new Link(0, 70.0, 200);
  net1.addLink(l1);
  CHECK(l1->getSlot(100) == false);
  CHECK_THROWS(net1.useSlot(-10, 100));
  net1.useSlot(0, 100);
  CHECK(l1->getSlot(100) == true);
  CHECK(net1.getLink(0)->getSlot(100) == true);
}

TEST_CASE("Use slot #2 (Network)") {
  Network net2 = Network();
  Link *l2 = new Link(0, 70.0, 200);
  net2.addLink(l2);
  CHECK(l2->getSlot(100) == false);
  net2.useSlot(0, 100, 104);
  CHECK_THROWS(net2.useSlot(-10, 100, 104));
  CHECK_THROWS(net2.useSlot(0, 104, 100));
  CHECK(l2->getSlot(100) == true);
  CHECK(net2.getLink(0)->getSlot(100) == true);
  CHECK(net2.getLink(0)->getSlot(101) == true);
  CHECK(net2.getLink(0)->getSlot(102) == true);
  CHECK(net2.getLink(0)->getSlot(103) == true);
}

TEST_CASE("JSON of NSFNet") {
  Network net = Network("../networks/NSFNet.json");
  // int numberOfNodes = 14;
  int adjacencyMatrix[14][14] = {
      {-1, 0, 2, -1, -1, -1, -1, 4, -1, -1, -1, -1, -1, -1},
      {1, -1, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {3, 7, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, 9, -1, -1, 12, -1, -1, -1, -1, -1, 14, -1, -1, -1},
      {-1, -1, -1, 13, -1, 16, 18, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, 11, -1, 17, -1, -1, -1, -1, 20, -1, -1, -1, 22},
      {-1, -1, -1, -1, 19, -1, -1, 24, -1, -1, -1, -1, -1, -1},
      {5, -1, -1, -1, -1, -1, 25, -1, 26, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, 27, -1, 28, -1, 30, 32, -1},
      {-1, -1, -1, -1, -1, 21, -1, -1, 29, -1, -1, -1, -1, -1},
      {-1, -1, -1, 15, -1, -1, -1, -1, -1, -1, -1, 34, 36, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, 31, -1, 35, -1, -1, 38},
      {-1, -1, -1, -1, -1, -1, -1, -1, 33, -1, 37, -1, -1, 40},
      {-1, -1, -1, -1, -1, 23, -1, -1, -1, -1, -1, 39, 41, -1}};

  for (int i = 0; i < 14; i++) {
    for (int j = 0; j < 14; j++) {
      CHECK(net.isConnected(i, j) == adjacencyMatrix[i][j]);
    }
  }
}

TEST_CASE("Connect links") {
  Network net = Network();
  Node *n1 = new Node(0);
  Node *n2 = new Node(1);
  Link *l1 = new Link(0, 70.0, 200);
  CHECK(l1->getDst() == -1);
  CHECK(l1->getSrc() == -1);

  net.addNode(n1);
  net.addNode(n2);
  net.addLink(l1);
  net.connect(0, 0, 1);

  CHECK(l1->getSrc() == n1->getId());
  CHECK(l1->getDst() == n2->getId());
}

TEST_CASE("Check Metrics Features") {
  Network netClean = Network();
  CHECK_THROWS(netClean.averageNeighborhood());
  CHECK_THROWS(netClean.normalAverageNeighborhood());
  CHECK_THROWS(netClean.nodalVariance());
  CHECK_THROWS(netClean.isGraphRelated());
  CHECK_THROWS(netClean.existNodeIsolated());
  Network net = Network("../networks/NSFNet.json");
  CHECK(net.averageNeighborhood() == Approx(6).epsilon(0.01));  // 1% error
  CHECK(net.normalAverageNeighborhood() ==
        Approx(0.23).epsilon(0.01));                           // 1% error
  CHECK(net.nodalVariance() == Approx(0.2857).epsilon(0.01));  // 1% error
  CHECK(net.isGraphRelated() == true);
  CHECK(net.existNodeIsolated() == false);
}