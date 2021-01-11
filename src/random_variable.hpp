#ifndef __RANDOM_VARIABLE_H__
#define __RANDOM_VARIABLE_H__

#include <random>

class RandomVariable {
 public:
  RandomVariable(unsigned int seed, double parameter1);
  virtual double getNextValue(void) = 0;

 protected:
  std::mt19937 generator;
  std::uniform_real_distribution<double> dist;
  double parameter1;
};

#endif