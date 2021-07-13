// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include "../src/controller.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Controller)") { CHECK_NOTHROW(Controller()); }

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

TEST_CASE("Test Connection AddLink") {
  int testConnectionId = 1;
  int testLinkId = 0;
  std::vector<int> testSlots(3, 0);

  Connection connectionExample(testConnectionId);
  CHECK_NOTHROW(connectionExample.addLink(testLinkId, testSlots));
}