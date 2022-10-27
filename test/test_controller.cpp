// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include <fstream>

#include "../src/controller.hpp"
#include "../src/json.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Controller)") {
  CHECK_NOTHROW(Controller());
  int n1_pos;
  int n2_pos;
  int l1_pos;
  Network net = Network();

  net.addNode(new Node(0));
  n1_pos = net.getNode(0)->getId();

  net.addNode(new Node(1));
  n2_pos = net.getNode(1)->getId();
  net.addLink(new Link(0));

  l1_pos = net.getLink(0)->getId();
  net.connect(n1_pos, l1_pos, n2_pos);

  CHECK_NOTHROW(Controller(&net));
}

// Allocator Testing
TEST_CASE("Constructor (Allocator)") { CHECK_NOTHROW(Allocator()); }

TEST_CASE("Constructor (Network) with JSON file") {
  Network *networkExample;
  CHECK_NOTHROW(Allocator(networkExample));
};

TEST_CASE("Destructor (Allocator)") {
  Allocator *asignador = new Allocator;
  CHECK_NOTHROW(delete asignador);
}

TEST_CASE("AllocationStatus (Allocator) through Controller") {
  Controller controllerExample;
  int src = 0;
  int dst = 1;
  long long idConnection = 0;
  // Connection con = Connection(idConnection);
  BitRate bitRate(100.0);
  /* CHECK_THROWS(
      controllerExample.assignConnection(src, dst, bitRate, idConnection) ==
      (ALLOCATED || NOT_ALLOCATED));*/
  CHECK_THROWS(
      controllerExample.assignConnection(src, dst, bitRate, idConnection));
}

TEST_CASE("Correct format JSON file paths (Controller)") {
  std::ifstream file("../networks/NSFNet_routes.json");
  nlohmann::json jsonRoutes;
  file >> jsonRoutes;

  CHECK_NOTHROW(jsonRoutes["routes"]);
  CHECK_NOTHROW(jsonRoutes["routes"][0]["paths"]);
  CHECK_NOTHROW(jsonRoutes["routes"][0]["src"]);
  CHECK_NOTHROW(jsonRoutes["routes"][0]["dst"]);
}

TEST_CASE("Test Connection AddLink") {
  int testConnectionId = 1;
  int testLinkId = 0;
  std::vector<int> testSlots(3, 0);

  Connection connectionExample(testConnectionId);
  CHECK_NOTHROW(connectionExample.addLink(testLinkId, testSlots));
}
