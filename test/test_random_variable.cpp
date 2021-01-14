// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include <cmath>
#include <vector>

#include "../src/exp_variable.hpp"
#include "catch.hpp"

TEST_CASE("Constructor (Exponential)") {
  CHECK_NOTHROW(ExpVariable(12345, 5.0));
  CHECK_THROWS(ExpVariable(12345, -5.0));
}

TEST_CASE("Mean (Exponential)") {
  double lambda = 2.0;
  ExpVariable e = ExpVariable(1, lambda);
  double mean = 0;
  int number_of_samples = 1000000;
  double sample;
  for (int i = 0; i < number_of_samples; i++) {
    sample = e.getNextValue();
    // std::cout << sample << " ";
    mean += sample;
  }
  mean /= number_of_samples;
  CHECK(mean == Approx(1 / lambda).epsilon(0.01));  // 1% difference
}

TEST_CASE("Variance (Exponential)") {
  double lambda = 2.0;
  ExpVariable e = ExpVariable(1, lambda);
  double mean = 0, mean_prev = 0;
  int number_of_samples = 1000000;
  auto samples = std::vector<double>(number_of_samples);
  double sample;
  double sn1, sn;
  for (int i = 0; i < number_of_samples; i++) {
    sample = e.getNextValue();
    mean += sample;
    samples[i] = sample;
  }
  mean /= number_of_samples;

  sn = 0;
  CHECK(mean == Approx(1 / lambda).epsilon(0.01));  // 1% difference

  for (int i = 0; i < number_of_samples; i++) {
    sn += (mean - samples[i]) * (mean - samples[i]);
  }
  sn /= number_of_samples;
  CHECK(sn == Approx(1 / pow(lambda, 2.0)).epsilon(0.01));  // 1% difference
}
