#include "exp_variable.hpp"

ExpVariable::ExpVariable(unsigned int seed, double parameter1)
    : RandomVariable(seed, parameter1) {
  if (parameter1 <= 0) {
    throw std::runtime_error("Lambda parameter must be positive.");
  }
}

double ExpVariable::getNextValue() {
  return (-log(1 - dist(this->generator)) / this->parameter1);
}