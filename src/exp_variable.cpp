#include "exp_variable.hpp"

ExpVariable::ExpVariable(unsigned int seed, double parameter1)
    : RandomVariable(seed, parameter1) {}

double ExpVariable::getNextValue() {
  return (-log(1 - dist(this->generator)) / this->parameter1);
}