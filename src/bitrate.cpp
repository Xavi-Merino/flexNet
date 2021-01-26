#include "bitrate.hpp"

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
