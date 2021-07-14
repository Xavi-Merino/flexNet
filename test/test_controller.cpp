// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include "../src/controller.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Controller)")
{
    CHECK_NOTHROW(Controller());

    Network net = Network();
    CHECK_NOTHROW(Controller(net))
}
