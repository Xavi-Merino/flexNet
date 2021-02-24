#include "bitrate.hpp"

#include <fstream>
#include <iostream>

BitRate::BitRate(double bitRate) {
  this->bitRate = bitRate;
  this->bitRateStr = std::to_string(bitRate);
}

BitRate::~BitRate() {}

void BitRate::addModulation(std::string modulation, int slots, double reach) {
  this->modulation.push_back(modulation);
  this->slots.push_back(slots);
  this->reach.push_back(reach);
}

std::string BitRate::getModulation(int pos) { return this->modulation[pos]; }

int BitRate::getNumberOfSlots(int pos) { return this->slots[pos]; }

double BitRate::getReach(int pos) { return this->reach[pos]; }

void BitRate::readBitRateFile(std::string fileName, std::vector<BitRate> vect) {
  std::ifstream file(fileName);
  nlohmann::json bitRate;
  file >> bitRate;

  for (auto& x : bitRate.items()) {
    int bitrate = stoi(x.key());  // BITRATE

    int numberOfModulations = x.value().size();

    for (int i = 0; i < numberOfModulations; i++) {
      for (auto& w : x.value()[i].items()) {
        std::string modulation = w.key();
        int reach = w.value()["reach"];
        int slots = w.value()["slots"];

        BitRate aux = BitRate(bitrate);
        aux.addModulation(modulation, reach, slots);

        vect.push_back(aux);
      }
    }
  }
}
