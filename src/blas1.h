#pragma once

#include <cmath>

#include "traits.h"

namespace marlib {

  template<typename T1, typename T2>
  double ddot(const T1& x, const T2& y) {
    using traits1 = double_vector<T1>;
    const int n = traits1::size(x);
    double retval = 0;
    for (int i=0; i<n; i++) {
      retval += x[i] * y[i];
    }
    return retval;
  }
  
  template<typename T>
  double dasum(const T& x) {
    using traits = double_vector<T>;
    const int n = traits::size(x);
    double sum = 0;
    for (int i=0; i<n; i++) {
      sum += std::abs(x[i]);
    }
    return sum;
  }
  
  template<typename T>
  int idamax(const T& x) {
    using traits = double_vector<T>;
    const int n = traits::size(x);
    int maxi = 0;
    double maxv = abs(x[0]);
    for (int i=1; i<n; i++) {
      if (std::abs(x[i]) > maxv) {
        maxv = std::abs(x[i]);
        maxi = i;
      }
    }
    return maxi;
  }
  
  template<typename T>
  void dfill(T& x, double v) {
    using traits = double_vector<T>;
    const int n = traits::size(x);
    for (int i=0; i<n; i++) {
      x[i] = v;
    }
  }
  
  template<typename T1, typename T2>
  void dcopy(const T1& x, T2& y) {
    using traits1 = double_vector<T1>;
    const int n = traits1::size(x);
    for (int i=0; i<n; i++) {
      y[i] = x[i];
    }
  }
  
  template<typename T>
  void dscal(double alpha, T& x) {
    using traits = double_vector<T>;
    const int n = traits::size(x);
    for (int i=0; i<n; i++) {
      x[i] *= alpha;
    }
  }
  
  template<typename T1, typename T2>
  void daxpy(double alpha, const T1& x, T2& y) {
    using traits1 = double_vector<T1>;
    const int n = traits1::size(x);
    for (int i=0; i<n; i++) {
      y[i] += alpha * x[i];
    }
  }

}
