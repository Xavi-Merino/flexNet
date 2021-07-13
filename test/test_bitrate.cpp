// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include "../src/bitrate.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Bitrate)") { CHECK_NOTHROW(BitRate(10.0)); }

TEST_CASE("Getting BitRate (Bitrate)") {
  BitRate b1 = BitRate(10.0);
  BitRate b2 = BitRate(40.0);

  CHECK(10.0 == Approx(b1.getBitRate()).epsilon(0.01));
  CHECK(10.0 == Approx(std::stod(b1.getBitRateStr())).epsilon(0.01));
  CHECK(40.0 == Approx(b2.getBitRate()).epsilon(0.01));
  CHECK(40.0 == Approx(std::stod(b2.getBitRateStr())).epsilon(0.01));
}

TEST_CASE("Getting BitRate from file (Bitrate)") {
  std::vector<BitRate> bs =
      BitRate::readBitRateFile(std::string("../bitrate/bitrate.json"));

  // Bitrate 10.0
  CHECK(10.0 == Approx(bs[0].getBitRate()).epsilon(0.01));
  CHECK(10.0 == Approx(std::stod(bs[0].getBitRateStr())).epsilon(0.01));

  CHECK(bs[0].getModulation(0).compare(std::string("BPSK")) == 0);
  CHECK(5520.0 == Approx(bs[0].getReach(0)).epsilon(0.01));
  CHECK(bs[0].getNumberOfSlots(0) == 1);

  CHECK_THROWS(bs[0].getModulation(1));

  // Bitrate 40.0
  CHECK(40.0 == Approx(bs[1].getBitRate()).epsilon(0.01));
  CHECK(40.0 == Approx(std::stod(bs[1].getBitRateStr())).epsilon(0.01));

  CHECK(bs[1].getModulation(0).compare(std::string("BPSK")) == 0);
  CHECK(5520.0 == Approx(bs[1].getReach(0)).epsilon(0.01));
  CHECK(bs[1].getNumberOfSlots(0) == 4);

  CHECK(bs[1].getModulation(1).compare(std::string("QPSK")) == 0);
  CHECK(5520.0 == Approx(bs[1].getReach(1)).epsilon(0.01));
  CHECK(bs[1].getNumberOfSlots(1) == 2);

  CHECK_THROWS(bs[1].getModulation(2));

  // Bitrate 100.0
  CHECK(100.0 == Approx(bs[2].getBitRate()).epsilon(0.01));
  CHECK(100.0 == Approx(std::stod(bs[2].getBitRateStr())).epsilon(0.01));

  CHECK(bs[2].getModulation(0).compare(std::string("BPSK")) == 0);
  CHECK(5520.0 == Approx(bs[2].getReach(0)).epsilon(0.01));
  CHECK(bs[2].getNumberOfSlots(0) == 8);

  CHECK_THROWS(bs[2].getModulation(1));

  // Bitrate 400.0
  CHECK(400.0 == Approx(bs[3].getBitRate()).epsilon(0.01));
  CHECK(400.0 == Approx(std::stod(bs[3].getBitRateStr())).epsilon(0.01));

  CHECK(bs[3].getModulation(0).compare(std::string("BPSK")) == 0);
  CHECK(5520.0 == Approx(bs[3].getReach(0)).epsilon(0.01));
  CHECK(bs[3].getNumberOfSlots(0) == 32);

  CHECK_THROWS(bs[3].getModulation(1));

  // Bitrate 1000.0
  CHECK(1000.0 == Approx(bs[4].getBitRate()).epsilon(0.01));
  CHECK(1000.0 == Approx(std::stod(bs[4].getBitRateStr())).epsilon(0.01));

  CHECK(bs[4].getModulation(0).compare(std::string("BPSK")) == 0);
  CHECK(5520.0 == Approx(bs[4].getReach(0)).epsilon(0.01));
  CHECK(bs[4].getNumberOfSlots(0) == 80);

  CHECK_THROWS(bs[4].getModulation(1));
}

TEST_CASE("Throwing Getter Errors") {
  BitRate b1 = BitRate(40.0);
  b1.addModulation("BPSK", 4, 5520);

  CHECK_THROWS(b1.getModulation(5) == "QPSK");
  CHECK_THROWS(b1.getNumberOfSlots(5) == 2);
  CHECK_THROWS(b1.getReach(5) == 2720);
}
