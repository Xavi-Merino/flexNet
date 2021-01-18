#include "request.hpp"

Request::Request(int id, float bitRate) {
  this->id = id;
  this->bitRate = bitRate;
  this->modulationFormat = std::vector<std::string>();
  this->fsu = std::vector<int>();
}
