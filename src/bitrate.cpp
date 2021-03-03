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

std::vector<BitRate> BitRate::readBitRateFile(std::string fileName) {
  std::ifstream file(fileName);
  nlohmann::json bitRate;
  std::vector<BitRate> vect = std::vector<BitRate>();

  file >> bitRate;

  for (auto& x : bitRate.items()) {
    int bitrate = stoi(x.key());  // BITRATE

    int numberOfModulations = x.value().size();

    for (int i = 0; i < numberOfModulations; i++) {
      for (auto& w : x.value()[i].items()) {
        std::string modulation = w.key();
        int reach = w.value()["reach"];
        int slots = w.value()["slots"];

        // exceptions
        if (reach && slots < 0) {
          throw std::runtime_error(
              "value entered for slots and reach is less than zero");
        }

        if (reach < 0) {
          throw std::runtime_error("value entered for reach is less than zero");
          ;
        }

        if (slots < 0) {
          throw std::runtime_error("value entered for slots is less than zero");
          ;
        }
        // end of exceptions

        BitRate aux = BitRate(bitrate);
        aux.addModulation(modulation, slots, reach);

        vect.push_back(aux);
      }
    }
  }

  return vect;
}

std::string BitRate::getBitRateStr() { return this->bitRateStr; }

double BitRate::getBitRate() { return this->bitRate; }
