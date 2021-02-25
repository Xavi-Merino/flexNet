#ifndef __RANDOM_VARIABLE_H__
#define __RANDOM_VARIABLE_H__

#include <random>
/**
 * @brief Class that generates random numbers.
 *
 * This class is used to generate a random variable, it can be used with default
 * or personalized parameters. It's inherited by the UniformVariable and
 * ExpVariable classes.
 *
 */
class RandomVariable {
 public:
  /**
   * @brief Constructs a new Random Variable object. Default values are assigned
   * to set it up.
   *
   */
  RandomVariable();
  /**
   * @brief Constructs a new Random Variable object.
   *
   * @param seed the seed that is used as a source of randomness by a random
   * number engine to generate a new value inside the object.
   * @param parameter1 the parameter value.
   */
  RandomVariable(unsigned int seed, double parameter1);
  /**
   * @brief Virtual method to obtain the next value on the random variable
   * object. It must be implemented by an inherited class.
   *
   * @return double the number 0. If implemented, it would return a
   * random value.
   */
  virtual double getNextValue(void) = 0;

 protected:
  /**
   * @brief A random number engine that generates a pseudo-random number from a
   * seed.
   *
   */
  std::mt19937 generator;
  /**
   * @brief A random number distribution that can generate values according to
   * an uniform distribution.
   *
   */
  std::uniform_real_distribution<double> dist;
  /**
   * @brief A parameter used to generate the random numbers.
   *
   */
  double parameter1;
};

#endif