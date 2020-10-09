// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include "../src/node.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Node)") {
  CHECK_NOTHROW(Node());
  CHECK_NOTHROW(Node(0));
  CHECK_NOTHROW(Node(0, "Test node 0"));
}

TEST_CASE("Getting ID (Node)") {
  Node n1 = Node();
  Node n2 = Node(50);

  CHECK(n1.getId() == -1);
  CHECK(n2.getId() == 50);
}

TEST_CASE("Setting ID (Node)") {
  Node n1 = Node();

  n1.setId(45);

  CHECK(n1.getId() == 45);
}

/*
TEST_CASE("Setting label (Node)") {
  Node n1 = Node();

  n1.setLabel(std::string("testing node"));

  CHECK(n1.getLabel().compare("testing node") == 0);
}
*/