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

std::string BitRate::getModulation(int pos) {
  if (pos >= this->modulation.size()) {
    throw std::runtime_error(
        "Bitrate " + this->bitRateStr + " does not have more than " +
        std::to_string(this->modulation.size()) + " modulations.");
  }
  return this->modulation[pos];
}

int BitRate::getNumberOfSlots(int pos) {
  if (pos >= this->slots.size()) {
    throw std::runtime_error(
        "Bitrate " + this->bitRateStr + " does not have more than " +
        std::to_string(this->slots.size()) + " modulations.");
  }
  return this->slots[pos];
}

double BitRate::getReach(int pos) {
  if (pos >= this->reach.size()) {
    throw std::runtime_error(
        "Bitrate " + this->bitRateStr + " does not have more than " +
        std::to_string(this->reach.size()) + " modulations.");
  }
  return this->reach[pos];
}

std::vector<BitRate> BitRate::readBitRateFile(std::string fileName) {
  std::ifstream file(fileName);
  nlohmann::ordered_json bitRate;
  std::vector<BitRate> vect = std::vector<BitRate>();

  file >> bitRate;

  for (auto& x : bitRate.items()) {
    int bitrate = stoi(x.key());  // BITRATE

    int numberOfModulations = x.value().size();

    BitRate aux = BitRate(bitrate);
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

        /*Suggestion: these last 3 exceptions may be condensed into one:
         *
         *if ((slots || reach) < 0 ) {
         * throw ...
         *}
         */

        // end of exceptions
        aux.addModulation(modulation, slots, reach);
      }
    }
    vect.push_back(aux);
  }

  return vect;
}

std::string BitRate::getBitRateStr() { return this->bitRateStr; }

double BitRate::getBitRate() { return this->bitRate; }
