/**
 * @file poisson.h
 * @brief Compute p.m.f. of poisson distribution
 * */

#pragma once

#include <cmath>
#include <cassert>

#include "traits.h"

namespace marlib {
  /**
  * @brief Compute the right bound of Poisson range
  * for a given error tolerance
  * @param lambda a Poisson rate (mean)
  * @param eps a value of error tolerance
  * @return right bound is a return value
  * */
  int rightbound(double lambda, double eps);
  
  /**
  * @brief Compute poisson probability vector
  * @param lambda a Poisson parameter
  * @param left an int of left bound
  * @param right an int of right bound
  * @param prob Poisson probabilities from left to right bounds
  * @return weight value, i.e., exact pmf is prob[i]/weight
  * */
  
  template<typename T>
  double pmf(double lambda, int left, int right, T& prob) {
    static const double pi = 4.0 * atan(1.0);
    static const double LOG2PIOVER2 = log(2*pi) / 2.0;

    using traits = double_vector<T>;
    const int n = traits::size(prob);
    assert(n >= right - left + 1);
    
    int mode = static_cast<int>(lambda);
    if (mode >= 1) {
      prob[mode-left] = std::exp(-lambda + mode * std::log(lambda)
                                   - LOG2PIOVER2 - (mode + 1.0/2.0) * std::log(mode) + mode);
    } else {
      prob[mode-left] = std::exp(-lambda);
    }
    // -- down --
    for (int j=mode; j>=left+1; j--) {
      prob[j-1-left] = prob[j-left] * j / lambda;
    }
    // -- up --
    for (int j=mode; j<=right-1; j++) {
      prob[j+1-left] = prob[j-left] * lambda / (j+1);
    }
    // -- compute W --
    double weight = 0.0;
    int s = left;
    int t = right;
    while (s < t) {
      if (prob[s-left] <= prob[t-left]) {
        weight += prob[s-left];
        s++;
      } else {
        weight += prob[t-left];
        t--;
      }
    }
    weight += prob[s-left];
    return weight;
  }
}
