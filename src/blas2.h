#pragma once

#include "traits.h"

namespace marlib {

struct trans{};
struct notrans{};

struct cf1_matrix{};

template<typename T1, typename T2, typename T3>
void dgemv(cf1_matrix, trans, double alpha, const T1& A, const T2& x, double beta, T3& y) {
  using traits1 = double_vector<T1>;
  const int n = traits1::size(A);
  for (int i=n-1; i>=1; i--) {
    y[i] = alpha * (x[i] * (1 - A[i]) + x[i-1] * A[i-1]) + beta * y[i];
  }
  y[0] = alpha * x[0] * (1 - A[0]) + beta * y[0];
}

template<typename T1, typename T2, typename T3>
void dgemv(cf1_matrix, notrans, double alpha, const T1& A, const T2& x, double beta, T3& y) {
  using traits1 = double_vector<T1>;
  const int n = traits1::size(A);
  for (int i=0; i<n-1; i++) {
    y[i] = alpha * (x[i] * (1 - A[i]) + x[i+1] * A[i]) + beta * y[i];
  }
  y[n-1] = alpha * x[n-1] * (1 - A[n-1]) + beta * y[n-1];
}

template<typename T1, typename T2, typename T3>
void dger(cf1_matrix, double alpha, const T1& x, const T2& y, T3& A) {
  using traits1 = double_vector<T1>;
  const int n = traits1::size(x);
  for (int i=0, p=0; i<n-1; i++, p+=2) {
    A[p] += alpha * x[i] * y[i];
    A[p+1] += alpha * x[i] * y[i+1];
  }
  A[2*n-2] += alpha * x[n-1] * y[n-1];
}

}