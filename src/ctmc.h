#pragma once

#include <vector>
#include "blas1.h"
#include "blas2.h"
#include "poisson.h"
#include "traits.h"

#include <Rcpp.h>

namespace marlib {

template<typename MatT, typename TR,
         typename T1, typename T2, typename T3, typename T4, typename T5>
void mexpv(MatT, TR, const T1& P, const T2& poi, int right, double weight,
           const T3& x, T4& y, T5& xi) {
  dcopy(x, xi);
  dfill(y, 0.0);
  daxpy(poi[0], xi, y);
  for (int k=1; k<=right; k++) {
    dgemv(MatT(), TR(), 1.0, P, xi, 0.0, xi);
    daxpy(poi[k], xi, y);
  }
  dscal(1.0/weight, y);
}

template<typename T1>
double unif(cf1_matrix, T1& rate, double ufactor) {
  double max_value = rate[idamax(rate)];
  double qv = max_value * ufactor;
  dscal(1/qv, rate);
  return qv;
}

template<typename TR>
struct not_{};

template<>
struct not_<trans>{
  using Type = notrans;
};

template<>
struct not_<notrans>{
  using Type = trans;
};

//
template<typename MatT, typename TR,
         typename T1, typename T2, typename T3, typename T4,
         typename T5, typename T6, typename T7, typename T8>
void mexp_conv(MatT, TR, const T1& P, double qv,
               const T2& poi, int right, double weight,
               const T3& x, const T4& y, T5& z, T6& H, T7& xi, T8& vc) {
  using notTR = not_<TR>;
  for (int i=0; i<=right+1; i++) {
    dfill(vc[i], 0.0);
  }

  // dfill(n, vc[right+1], 0.0);
  daxpy(poi[right+1], y, vc[right+1]);
  for (int l=right; l>=1; l--) {
    dgemv(MatT(), typename notTR::Type(), 1.0, P, vc[l+1], 0.0, vc[l]);
    daxpy(poi[l], y, vc[l]);
  }
  
  dcopy(x, xi);
  dfill(z, 0.0);
  daxpy(poi[0], xi, z);
  // dfill(H, 0.0);
  dger(MatT(), 1.0/qv/weight, xi, vc[1], H);
  for (int l=1; l<=right; l++) {
    dgemv(MatT(), TR(), 1.0, P, xi, 0.0, xi);
    daxpy(poi[l], xi, z);
    dger(MatT(), 1.0/qv/weight, xi, vc[l+1], H);
  }
  dscal(1.0/weight, z);
  // dscal(1.0/qv/weight, H);
}

}
