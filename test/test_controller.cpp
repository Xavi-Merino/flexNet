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
  Network *net = Network();

  net.addNode(new Node(0));
  n1_pos = net.getNode(0)->getId();

  net.addNode(new Node(1));
  n2_pos = net.getNode(1)->getId();
  net.addLink(new Link(0));

  l1_pos = net.getLink(0)->getId();
  net.connect(n1_pos, l1_pos, n2_pos);

  CHECK_NOTHROW(Controller(net));
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