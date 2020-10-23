// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include "../src/link.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Link)") {
  CHECK_NOTHROW(Link());
  CHECK_NOTHROW(Link(0));
}

TEST_CASE("Getting ID (Link)") {
  Link l1 = Link();
  Link l2 = Link(50);

  CHECK(l1.getId() == -1);
  CHECK(l2.getId() == 50);
}

TEST_CASE("Setting ID (Link)") {
  Link l1 = Link();

  l1.setId(45);

  CHECK(l1.getId() == 45);
}
