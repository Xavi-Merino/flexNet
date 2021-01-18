#include "request.hpp"

Request::Request(int id, float bitRate) {
  this->id = id;
  this->bitRate = bitRate;
  this->modulationFormat = std::vector<std::string>();
  this->fsu = std::vector<int>();
  this->numberOfFormats = 0;
}

void Request::addFormat(std::string modulationFormat, int numberOfSlots) {
  this->modulationFormat.push_back(modulationFormat);
  this->fsu.push_back(numberOfSlots);
  this->numberOfFormats++;
}

int Request::getNumberOfSlots(int pos) {
  if (pos < 0 || pos >= this->numberOfFormats) {
    throw std::runtime_error(
        "You are trying to access a format modulation position that not "
        "exists.");
  }
  return this->fsu[pos];
}

int Request::getNumberOfSlots(std::string modulationFormat) {
  for (int i = 0; i < this->numberOfFormats; i++) {
    if (modulationFormat.compare(this->modulationFormat[i])) {
      return this->fsu[i];
    }
  }
  throw std::runtime_error("You are trying to access a format modulation (" +
                           modulationFormat +
                           ") that not "
                           "exists.");
}
