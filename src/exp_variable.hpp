#ifndef __EXP_VARIABLE_H__
#define __EXP_VARIABLE_H__

#include <stdexcept>

#include "random_variable.hpp"

class ExpVariable : public RandomVariable {
 public:
  ExpVariable(unsigned int seed, double parameter1);
  double getNextValue(void);
};

#endif