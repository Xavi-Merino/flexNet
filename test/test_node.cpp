// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include "../src/node.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Node)") {
  CHECK_NOTHROW(Node());
  CHECK_NOTHROW(Node(0));
  CHECK_NOTHROW(Node(1, "Test node 1"));
}

TEST_CASE("Getting ID (Node)") {
  Node n1 = Node();
  Node n2 = Node(2);

  CHECK(n1.getId() == -1);
  CHECK(n2.getId() == 2);
}

TEST_CASE("Setting ID (Node)") {
  Node n1 = Node();
  n1.setId(3);
  CHECK(n1.getId() == 3);
  Node n2 = Node();
  n2.setId(4);
  CHECK(n2.getId() == 4);
  Node n3 = Node(10);
  CHECK_THROWS(n3.setId(5));
}

TEST_CASE("Setting label (Node)") {
  Node n1 = Node();

  n1.setLabel(std::string("testing node"));

  CHECK(n1.getLabel().compare("testing node") == 0);
}
