#ifndef __UNIFORM_VARIABLE_H__
#define __UNIFORM_VARIABLE_H__

#include <stdexcept>

#include "random_variable.hpp"

class UniformVariable : public RandomVariable {
 public:
  UniformVariable(void);
  UniformVariable(unsigned int seed, double parameter1);
  double getNextValue(void);
  double getNextIntValue(void);

 private:
  std::uniform_int_distribution<int> dist;
};

#endif