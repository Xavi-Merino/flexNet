#include "random_variable.hpp"

RandomVariable::RandomVariable() {
  this->generator = std::mt19937(1234567);
  this->parameter1 = 10;
  this->dist =
      std::uniform_real_distribution<double>(0, std::nextafter(1.0, 1.1));
}

RandomVariable::RandomVariable(unsigned int seed, double parameter1) {
  this->generator = std::mt19937(seed);
  this->parameter1 = parameter1;
  this->dist =
      std::uniform_real_distribution<double>(0, std::nextafter(1.0, 1.1));
}
