// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include "../src/link.hpp"
#include "catch.hpp"

TEST_CASE("#1 Constructor (Link)") {
  CHECK_NOTHROW(Link());
  CHECK_NOTHROW(Link(0));
}

TEST_CASE("#2 Constructor (Link)") { CHECK_NOTHROW(Link(1, 70.0)); }

TEST_CASE("#3 Constructor (Link)") { CHECK_NOTHROW(Link(2, 70.0, 200)); }

TEST_CASE("#4 Constructor (Link)") {
  CHECK_NOTHROW(Link());
  CHECK_NOTHROW(Link(3));
  CHECK_NOTHROW(Link(4, 70.0));
  CHECK_NOTHROW(Link(5, 70.0, 200));
  CHECK_NOTHROW(Link(6, 70.0, 200));
}

TEST_CASE("Constructor Exception(Link)") {
  CHECK_THROWS(Link(7, 0.0));
  CHECK_THROWS(Link(8, 1.1, 0));
  CHECK_THROWS(Link(9, -0.1, 11));
}

TEST_CASE("Getting ID (Link)") {
  Link l1 = Link();
  Link l2 = Link(10);
  Link l3 = Link(11, 70.0);
  Link l4 = Link(12, 70.0, 200);

  CHECK(l1.getId() == -1);
  CHECK(l2.getId() == 10);
  CHECK(l3.getId() == 11);
  CHECK(l4.getId() == 12);
}

TEST_CASE("Setting ID (Link)") {
  Link l1 = Link();
  l1.setId(13);
  CHECK(l1.getId() == 13);
  Link l2 = Link();
  l2.setId(14);
  CHECK(l2.getId() == 14);
  Link l3 = Link(15);
  CHECK_THROWS(l3.setId(16));
}

TEST_CASE("Getting length (Link)") {
  Link l1 = Link(16, 70.0, 200);
  Link l2 = Link(17, 80.0, 200);
  CHECK(l1.getLength() == 70.0);
  CHECK(l2.getLength() == 80.0);
}

TEST_CASE("Setting length (Link)") {
  Link l1 = Link(18, 70.0, 200);

  l1.setLength(10.0);
  CHECK_THROWS(l1.setLength(-10.0));
  CHECK(l1.getLength() == 10.0);
}

TEST_CASE("Getting slots (Link)") {
  Link l1 = Link(19, 70.0, 200);

  CHECK(l1.getSlots() == 200);
}

TEST_CASE("Setting slots (Link)") {
  Link l1 = Link(20, 70.0, 200);

  l1.setSlots(100);

  CHECK(l1.getSlots() == 100);

  CHECK_THROWS(l1.setSlots(0));
}

TEST_CASE("Setting slot (Link)") {
  Link l1 = Link(21, 70.0, 200);

  l1.setSlots(100);
  l1.setSlot(50, true);
  CHECK_THROWS(l1.setSlot(-50, true));
  CHECK_THROWS(l1.setSlot(50, true));

  CHECK(l1.getSlot(50) == true);
  CHECK(l1.getSlot(51) == false);

  CHECK_THROWS(l1.setSlots(0));
  CHECK_THROWS(l1.setSlots(10));
  CHECK_THROWS(l1.setSlots(-1));
}

TEST_CASE("getting slot (Link)") {
  Link l1 = Link(22, 70.0, 200);

  CHECK(l1.getSlot(50) == false);
  CHECK_THROWS(l1.getSlot(-1) == false);
  CHECK_THROWS(l1.getSlot(3000) == false);
}

TEST_CASE("getting source and destination") {
  Link l1 = Link(23, 70.0, 200);

  CHECK(l1.getSrc() == -1);
  CHECK(l1.getDst() == -1);
}

// New tests

TEST_CASE("#5 Constructor (multi-mode and multi-core Link)") { CHECK_NOTHROW(Link(24, 70.0, 200, 3)); }

TEST_CASE("#6 Constructor (multi-mode and multi-core Link)") { CHECK_NOTHROW(Link(25, 70.0, 200, 3, 2)); }

TEST_CASE("#7 Constructor (multi-mode and multi-core Link)") { // TODO: porque ambos constructores juntos?
  CHECK_NOTHROW(Link(26, 70.0, 200, 3));
  CHECK_NOTHROW(Link(27, 70.0, 200, 3, 2));
}

TEST_CASE("Constructor Exception (multi-mode and multi-core Link)") {
  CHECK_THROWS(Link(28, 1.1, 0, 3));
  CHECK_THROWS(Link(29, 1.1, 100, -1));
  CHECK_THROWS(Link(30, -100.0, 100, 3));

  CHECK_THROWS(Link(31, 1.1, 0, 3, 2));
  CHECK_THROWS(Link(32, 1.1, 100, 0, 2));
  CHECK_THROWS(Link(33, 1.1, 100, 3, 0));
  CHECK_THROWS(Link(34, -1.1, 100, 3, 2));    
}

TEST_CASE("Getting slots (multi-mode and multi-core Link)") {
  Link l1 = Link(35, 70.0, 200, 3, 2);

  CHECK(l1.getSlots(0, 1) == 200);
}

TEST_CASE("Setting slots (multi-mode and multi-core Link)") {
  Link l1 = Link(36, 70.0, 200, 3, 2);

  CHECK_THROWS(l1.setSlots(200, 0, 1));

  l1.setSlots(100, 0, 1);

  CHECK(l1.getSlots(0, 1) == 100);

  CHECK_THROWS(l1.setSlots(0, 0, 1));
  CHECK_THROWS(l1.setSlots(100, 5, 1));
  CHECK_THROWS(l1.setSlots(100, 0, 1));
  CHECK_THROWS(l1.setSlots(100, 0, 5));
}

TEST_CASE("getting slot (multi-mode and multi-core Link)") {
  Link l1 = Link(37, 70.0, 200, 3, 2);

  CHECK(l1.getSlot(0, 1, 50) == false);
  CHECK_THROWS(l1.getSlot(0, 1, -1) == false);
  CHECK_THROWS(l1.getSlot(0, 1, 3000) == false);
  CHECK_THROWS(l1.getSlot(0, 3000, 150) == false);
  CHECK_THROWS(l1.getSlot(3000, 1, 150) == false);
}

TEST_CASE("Setting slot (multi-mode and multi-core Link)") {
  Link l1 = Link(38, 70.0, 200, 3, 2);

  l1.setSlots(100, 2, 1);
  l1.setSlot(0, 1, 50, true);
  CHECK_THROWS(l1.setSlot(0, 1, -50, true));
  CHECK_THROWS(l1.setSlot(0, 1, 50, true));
  CHECK_THROWS(l1.setSlot(0, -1, 50, true));
  CHECK_THROWS(l1.setSlot(-1, 0, 50, true));
  CHECK_THROWS(l1.setSlots(10, 0, 1));
  
  CHECK(l1.getSlot(0, 1, 50) == true);
  CHECK(l1.getSlot(0, 1, 51) == false);

  CHECK_THROWS(l1.setSlots(10, 0, 1)); 
}

TEST_CASE("Getting cores (multi-mode and multi-core Link)") {
  Link l1 = Link(39, 70.0, 200, 3, 2);

  CHECK(l1.getCores() == 3);
}

TEST_CASE("Setting cores (multi-mode and multi-core Link)") {
  Link l1 = Link(40, 70.0, 200, 3, 2);

  l1.setCores(6);
  l1.setSlot(0, 1, 20, true);

  CHECK(l1.getCores() == 6);

  CHECK_THROWS(l1.setCores(0));
  CHECK_THROWS(l1.setCores(5));
  CHECK_THROWS(l1.setCores(6));
}

TEST_CASE("Getting modes (multi-mode and multi-core Link)") {
  Link l1 = Link(41, 70.0, 200, 3, 2);

  CHECK(l1.getModes() == 2);
}

TEST_CASE("Setting modes (multi-mode and multi-core Link)") {
  Link l1 = Link(42, 70.0, 200, 3, 2);

  l1.setModes(6);
  l1.setSlot(0, 1, 20, true);

  CHECK(l1.getModes() == 6);

  CHECK_THROWS(l1.setModes(0));
  CHECK_THROWS(l1.setModes(5));
  CHECK_THROWS(l1.setModes(6));
}