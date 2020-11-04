// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include "../src/link.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Link)") {
  CHECK_NOTHROW(Link());
  CHECK_NOTHROW(Link(0));
  CHECK_NOTHROW(Link(0, 70.0));
  CHECK_NOTHROW(Link(0, 70.0, 200));
}

TEST_CASE("Constructor Exception(Link)") {
  CHECK_THROWS(Link(0, 0.0));
  CHECK_THROWS(Link(0, 1.1, 0));
  CHECK_THROWS(Link(0, -0.1, 11));
}

TEST_CASE("Getting ID (Link)") {
  Link l1 = Link();
  Link l2 = Link(50);
  Link l3 = Link(40, 70.0);
  Link l4 = Link(30, 70.0, 200);

  CHECK(l1.getId() == -1);
  CHECK(l2.getId() == 50);
  CHECK(l3.getId() == 40);
  CHECK(l4.getId() == 30);
}

TEST_CASE("Setting ID (Link)") {
  Link l1 = Link();

  l1.setId(45);

  CHECK(l1.getId() == 45);
}

TEST_CASE("Getting length (Link)") {
  Link l1 = Link(30, 70.0, 200);

  CHECK(l1.getLength() == 70.0);
}

TEST_CASE("Setting length (Link)") {
  Link l1 = Link(30, 70.0, 200);

  l1.setLength(10.0);

  CHECK(l1.getLength() == 10.0);
}

TEST_CASE("Getting slots (Link)") {
  Link l1 = Link(30, 70.0, 200);

  CHECK(l1.getSlots() == 200);
}

TEST_CASE("Setting slots (Link)") {
  Link l1 = Link(30, 70.0, 200);

  l1.setSlots(100);

  CHECK(l1.getSlots() == 100);

  CHECK_THROWS(l1.setSlots(0));
}

TEST_CASE("Setting slot (Link)") {
  Link l1 = Link(30, 70.0, 200);

  l1.setSlots(100);
  l1.setSlot(50, true);

  CHECK(l1.getSlot(50) == true);
  CHECK(l1.getSlot(51) == false);

  CHECK_THROWS(l1.setSlots(0));
  CHECK_THROWS(l1.setSlots(10));
}

TEST_CASE("getting slot (Link)") {
  Link l1 = Link(30, 70.0, 200);

  CHECK(l1.getSlot(50) == true);
  CHECK_THROWS(l1.getSlot(-1) == false);
  CHECK_THROWS(l1.getSlot(3000) == false);
}