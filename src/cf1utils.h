/**
 * @file cf1utils.h
 * @brief Utils for CF1
 * */

#pragma once

#include <cmath>
#include <limits>

#include "traits.h"
#include "array.h"

#include "debug.h"

namespace marlib {

template<typename T1, typename T2>
void cf1_swap(int i, int j, T1& alpha, T2& rate) {
  double w = rate[j] / rate[i];
  alpha[i] += (1.0 - w) * alpha[j];
  alpha[j] *= w;
  double tmp = rate[j];
  rate[j] = rate[i];
  rate[i] = tmp;
}

template<typename T1, typename T2>
void cf1_sort(T1& alpha, T2& rate) {
  using traits1 = double_vector<T1>;
  const int n = traits1::size(alpha);
  for (int i=0; i<n-1; i++) {
    if (rate[i] > rate[i+1]) {
      cf1_swap(i, i+1, alpha, rate);
      int j = i;
      while (j>0 && rate[j-1] > rate[j]) {
        cf1_swap(j-1, j, alpha, rate);
        j--;
      }
    }
  }
}

template <typename MatT,
          typename T1, typename T2, typename T4, typename T5,
          typename T6, typename T7, typename T8, typename T9, typename T10,
          typename T11, typename T12, typename T13, typename T14, typename T15,
          typename T16, typename T17, typename T18, typename T19, typename T20,
          typename T21, typename T22, typename T23, typename T24>
double emstep(MatT, trans, double omega, const T1& alpha, const T2& rate,
               double& new_omega, T4& new_alpha, T5& new_rate,
               const T6& time, const T7& num, const T8& type,
               const T9& P, double qv, T10& prob, double eps,
               T11& tmp, T12& pi2,
               double& en0, T13& eb, T14& eb2, T15& ey, T16& en, T17& h0,
               T18& blf, T19& blf2, T20& vb, T21& vb2, T22& xi, T23& vctmp, T24& lscal) {
  
#ifdef DEBUG
  // for debug
  std::string filename = LOG_FILE;
  std::ofstream writing_file;
  writing_file.open(filename, std::ios::app);
  writing_file << "-------- emstep ---------" << std::endl;
#endif

  using traits1 = double_vector<T1>;
  using traits2 = double_vector<T6>;
  const int n = traits1::size(alpha);
  const int dsize = traits2::size(time);
  int right;
  double weight;
  
  lscal[0] = 0.0;

  en0 = 0.0;
  dfill(eb, 0.0);
  dfill(eb2, 0.0);
  dfill(en, 0.0);
  
  dfill(vb[0], 1.0);
  dfill(vb2[0], 0.0);
  vb2[0][n-1] = rate[n-1];

  double llf = 0.0;
  for (int k=1; k<=dsize; k++) {
    double t = time[k-1];
    double x = num[k-1];
    double u = type[k-1];
    dcopy(vb[k-1], vb[k]);
    right = rightbound(qv*t, eps);
    weight = pmf(qv*t, 0, right, prob);
    mexpv(cf1_matrix(), notrans(), P, prob, right, weight, vb[k], vb[k], xi);
    if (std::fpclassify(x) != FP_ZERO) {
      dcopy(vb[k-1], tmp);
      daxpy(-1.0, vb[k], tmp);
      blf[k] = ddot(alpha, tmp);
      if (!std::isfinite(blf[k])) {
        new_omega = omega;
        dcopy(alpha, new_alpha);
        dcopy(rate, new_rate);
        return std::numeric_limits<double>::quiet_NaN();
      }
      llf += x * std::log(omega * blf[k]) + x * lscal[k-1] - std::lgamma(x+1.0);
      en0 += x;
      daxpy(x/blf[k], tmp, eb);
    } else {
      blf[k] = 1.0; // to avoid NaN
    }
    dcopy(vb2[k-1], vb2[k]);
    mexpv(cf1_matrix(), notrans(), P, prob, right, weight, vb2[k], vb2[k], xi);
    if (u == 1) {
      blf2[k] = ddot(alpha, vb2[k]);
      if (!std::isfinite(blf2[k])) {
        new_omega = omega;
        dcopy(alpha, new_alpha);
        dcopy(rate, new_rate);
        return std::numeric_limits<double>::quiet_NaN();
      }
      llf += std::log(omega * blf2[k]) + lscal[k-1];
      en0 += 1;
      daxpy(1/blf2[k], vb2[k], eb2);
    }
    double scale = dasum(vb2[k]);
    dscal(1/scale, vb[k]);
    dscal(1/scale, vb2[k]);
    lscal[k] = lscal[k-1] + std::log(scale);
    // lscal[k] = 0.0;

#ifdef DEBUG
    // for debug
    writing_file << "backward k=" << k << " t=" << t << " x=" << x << " u=" << u << " " << std::endl;
    printvec2(writing_file, "vb[k-1]: ", vb[k-1]);
    printvec2(writing_file, "vb[k]: ", vb[k]);
    printvec2(writing_file, "vb2[k-1]: ", vb2[k-1]);
    printvec2(writing_file, "vb2[k]: ", vb2[k]);
    printvec2(writing_file, "eb: ", eb);
    printvec2(writing_file, "eb2: ", eb2);
    writing_file << "blf[k]: " << blf[k] << " " << exp(lscal[k-1]) * blf[k] << std::endl;
    writing_file << "llf=" << llf << std::endl;
#endif
  }
  double barblf = ddot(alpha, vb[dsize])*exp(lscal[dsize]);
  llf += - omega * (1 - barblf);
  daxpy(omega * exp(lscal[dsize]), vb[dsize], eb);

#ifdef DEBUG
  // for debug
  writing_file << "lastllf=" << llf << std::endl;
#endif
  
  // compute pi2
  double tmpv = 0.0;
  for (int i=0; i<n-1; i++) {
    tmpv += alpha[i];
    pi2[i] = tmpv / rate[i];
  }
  pi2[n-1] = 1.0 / rate[n-1];

  // sojourn
  // scaling => blf[k], blf2[k] are divided by exp(lscal[k-1])
  // scaling => vb2[k] (vb[k]) are divided by exp(lscal[k])
  dfill(tmp, 0);
  // original step => daxpy((-num[dsize-1]/blf[dsize] + omega, pi2, tmp);
  // tmp will be multiplied by exp(lscal[dsize-1])
  daxpy(omega*exp(lscal[dsize-1]) - num[dsize-1]/blf[dsize], pi2, tmp);
  if (type[dsize-1] == 1) {
    daxpy(1.0/blf2[dsize], alpha, tmp);
  }
  right = rightbound(qv*time[dsize-1], eps);
  weight = pmf(qv*time[dsize-1], 0, right+1, prob);
  dfill(h0, 0.0);
  mexp_conv(cf1_matrix(), trans(), P, qv, prob, right, weight,
            tmp, vb2[dsize-1], tmp, h0, xi, vctmp);
  daxpy(1.0, h0, en);
  for (int k=dsize-1; k>=1; k--) {
    double t = time[k-1];
    double u = type[k-1];
    // scale of tmp will be changed; tmp will be multiplied by exp(lscal[k-1])
    // original => daxpy(num[k]/blf[k+1] - num[k-1]/blf[k], pi2, tmp);
    dscal(exp(lscal[k-1]-lscal[k]), tmp);
    daxpy(num[k]/blf[k+1]*exp(lscal[k-1]-lscal[k]) - num[k-1]/blf[k], pi2, tmp);
    if (u == 1) {
      daxpy(1.0/blf2[k], alpha, tmp);
    }
    right = rightbound(qv*t, eps);
    weight = pmf(qv*t, 0, right+1, prob);
    dfill(h0, 0.0);
    mexp_conv(cf1_matrix(), trans(), P, qv, prob, right, weight,
              tmp, vb2[k-1], tmp, h0, xi, vctmp);
    daxpy(1.0, h0, en);
    
#ifdef DEBUG
    // for debug
    writing_file << "sojourn k=" << k << " t=" << t << " x=" << num[k-1] << " u=" << u << " " << std::endl;
    printvec2(writing_file, "en: ", en);
#endif
  }


  /* concrete algorithm: M-step */
  for (int i=0; i<n-1; i++) {
    ey[i] = rate[i] * (en[2 * i + 1] + eb[i + 1] * pi2[i]) / (en[2 * i] + eb[i] * pi2[i]);
  }
  tmpv = en[2 * (n - 1)] + eb[n - 1] * pi2[n - 1];
  double sum = 0.0;
  for (int i=0; i<n; i++) {
    eb[i] = alpha[i] * (eb[i] + eb2[i]);
    sum += eb[i];
  }
  ey[n - 1] = sum / tmpv;
  for (int i=0; i < n; i++) {
    eb[i] /= sum;
  }
  new_omega = en0 + omega * barblf;
  dcopy(eb, new_alpha);
  dcopy(ey, new_rate);

#ifdef DEBUG
  // for debug
  writing_file << "new_omega: " << new_omega << std::endl;
  printvec2(writing_file, "new_alpha: ", new_alpha);
  printvec2(writing_file, "new_rate: ", new_rate);
  writing_file << "-------- END emstep ---------" << std::endl;
#endif

  return llf;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7>
double cf1emstep(double omega, const T1& alpha, const T2& rate,
            double& new_omega, T3& new_alpha, T4& new_rate,
            const T5& time, const T6& num, const T7& type,
            double eps, double ufactor) {
  using traits1 = double_vector<T1>;
  using traits2 = double_vector<T5>;
  const int n = traits1::size(alpha);
  const int dsize = traits2::size(time);
  tmp_array1 tmp(n);
  tmp_array1 pi2(n);
  tmp_array1 P(n);
  double en0;
  tmp_array1 eb(n);
  tmp_array1 eb2(n);
  tmp_array1 en(n+n);
  tmp_array1 h0(n+n);
  tmp_array1 ey(n);
  tmp_array1 blf(dsize+2);
  tmp_array1 blf2(dsize+2);
  tmp_array2 vb(dsize+2, n);
  tmp_array2 vb2(dsize+2, n);
  const double tmax = time[idamax(time)];
  dcopy(rate, P);
  double qv = unif(cf1_matrix(), P, ufactor);
  const int max_right = rightbound(qv*tmax, eps)+1;
  tmp_array1 prob(max_right+2);
  tmp_array1 xi(n);
  tmp_array2 vctmp(max_right+2, n);
  tmp_array1 lscal(dsize+1);

  double llf = emstep(cf1_matrix(), trans(),
                      omega, alpha, rate,
                      new_omega, new_alpha, new_rate,
                      time, num, type,
                      P, qv, prob, eps,
                      tmp, pi2,
                      en0, eb, eb2, ey, en, h0,
                      blf, blf2, vb, vb2, xi, vctmp, lscal);
  return llf;
}

// /// test
// double cf1estep2(
//     int n,
//     double omega,
//     const double* alpha,
//     const double* baralpha,
//     const double* xi,
//     const double* one,
//     const double* Q,
//     const double* P,
//     double qv,
//     int m,
//     const double* tdat,
//     const int* gdat,
//     int gdatlast,
//     const int* idat,
//     double& etotal,
//     double* eb,
//     double* ey,
//     double* ez,
//     double* en,
//     double poi_eps,
//     double* vf,
//     double* barvf_work,
//     double* tildevf,
//     double* vb_work,
//     double* barvb_work,
//     double* tildevb,
//     double* wg,
//     double* wp,
//     double* vc_work,
//     double* prob,
//     double* vtmp,
//     double* vtmpNR) {
//   int right;
//   double weight;
//   double tmp;
//   // sci::vector<double> vf(n);
//   // sci::array< sci::vector<double> > barvf(m+1, sci::vector<double>(n));
//   // sci::vector<double> tildevf(n);
//   // 
//   // sci::array< sci::vector<double> > vb(m+1, sci::vector<double>(n));
//   // sci::array< sci::vector<double> > barvb(m+1, sci::vector<double>(n));
//   // sci::vector<double> tildevb(n);
//   // 
//   // sci::vector<double> wg(m+1);
//   // sci::vector<double> wp(m+1);
//   // sci::array< sci::vector<double> > vc(m+1, sci::vector<double>(n));
//   
//   std::vector<double*> barvf(m+1);
//   std::vector<double*> barvb(m+1);
//   std::vector<double*> vb(m+1);
//   std::vector<double*> vc(m+1);
//   
//   myblas::dfill(n, eb, 0.0);
//   myblas::dfill(n, ey, 0.0);
//   myblas::dfill(n, ez, 0.0);
//   myblas::dfill(n+n, en, 0.0);
//   
//   // double tmax = dmax(tdat);
//   // double weight;
//   // sci::range r(0, pois::rightbound(qv*tmax, poi_eps) + 1);
//   // sci::vector<double> poi(r.size());
//   
//   myblas::dcopy(n, baralpha, barvf[0]);
//   myblas::dcopy(n, one, barvb[0]);
//   myblas::dcopy(n, xi, vb[0]);
//   double nn = 0.0;
//   double uu = 0.0;
//   double llf = 0.0;
//   
//   for (int k=1; k<=m; k++) {
//     right = poisson::rightbound(qv*tdat[k-1], poi_eps) + 1;
//     poisson::prob(qv*tdat[k-1], 0, right, prob, weight);
//     
//     forward2(n, P, prob, right, weight, barvf[k-1], barvf[k], vtmp);
//     backward2(n, P, prob, right, weight, barvb[k-1], barvb[k], vtmp);
//     dgemvN(n, -1.0, Q, barvb[k], 0.0, vb[k]);
//     
//     myblas::dcopy(n, barvf[k-1], tildevf);
//     myblas::daxpy(n, -1.0, barvf[k], tildevf);
//     myblas::dcopy(n, barvb[k-1], tildevb);
//     myblas::daxpy(n, -1.0, barvb[k], tildevb);
//     
//     if (gdat[k-1] >= 0 && tdat[k-1] != 0.0) {
//       double tmp = myblas::ddot(n, alpha, tildevb);
//       llf += gdat[k-1] * std::log(tmp) - lfact(gdat[k-1]);
//       nn += gdat[k-1];
//       uu += tmp;
//       myblas::daxpy(n, gdat[k-1]/tmp, tildevb, eb);
//       myblas::daxpy(n, gdat[k-1]/tmp, tildevf, ey);
//       wg[k] = gdat[k-1] / tmp;
//     }
//     if (idat[k-1] == 1) {
//       dgemvT(n, -1.0, Q, barvf[k], 0.0, vf);
//       double tmp = myblas::ddot(n, alpha, vb[k]);
//       llf += std::log(tmp);
//       nn += 1.0;
//       myblas::daxpy(n, 1.0/tmp, vb[k], eb);
//       myblas::daxpy(n, 1.0/tmp, vf, ey);
//       wp[k] = 1.0 / tmp;
//     }
//   }
//   // for the interval [t_m, infinity)
//   if (gdatlast >= 0) {
//     double tmp = myblas::ddot(n, alpha, barvb[m]);
//     llf += gdatlast * std::log(tmp) - lfact(gdatlast);
//     nn += gdatlast;
//     uu += tmp;
//     myblas::daxpy(n, gdatlast/tmp, barvb[m], eb);
//     myblas::daxpy(n, gdatlast/tmp, barvf[m], ey);
//     wg[m+1] = gdatlast / tmp;
//   }
//   // compupte weights for unobserved periods
//   for (int k=1; k<=m; k++) {
//     if (gdat[k-1] == -1) {
//       myblas::dcopy(n, barvb[k-1], vtmp);
//       myblas::daxpy(n, -1.0, barvb[k], vtmp);
//       myblas::daxpy(n, omega, vtmp, eb);
//       myblas::dcopy(n, barvf[k-1], vtmp);
//       myblas::daxpy(n, -1.0, barvf[k], vtmp);
//       myblas::daxpy(n, omega, vtmp, ey);
//       wg[k] = omega;
//     }
//   }
//   if (gdatlast == -1) {
//     myblas::daxpy(n, omega, barvb[m], eb);
//     myblas::daxpy(n, omega, barvf[m], ey);
//     wg[m+1] = omega;
//   }
//   llf += nn * std::log(omega) - omega * uu;
//   // compute vectors for convolution
//   // vc[m] = 0.0;
//   myblas::daxpy(n, wg[m+1]-wg[m], baralpha, vc[m]);
//   if (idat[m-1] == 1) {
//     myblas::daxpy(n, wp[m], alpha, vc[m]);
//   }
//   for (int k=m-1; k>=1; k--) {
//     right = poisson::rightbound(qv*tdat[k], poi_eps) + 1;
//     poisson::prob(qv*tdat[k], 0, right, prob, weight);
//     
//     forward2(n, P, prob, right, weight, vc[k+1], vc[k], vtmp);
//     myblas::daxpy(n, wg[k+1]-wg[k], baralpha, vc[k]);
//     if (idat[k-1] == 1) {
//       myblas::daxpy(n, wp[k], alpha, vc[k]);
//     }
//   }
//   for (int k=1; k<=m; k++) {
//     right = poisson::rightbound(qv*tdat[k-1], poi_eps) + 1;
//     poisson::prob(qv*tdat[k-1], 0, right, prob, weight);
//     
//     dger(n, wg[k+1]-wg[k], baralpha, barvb[k], en);
//     
//     forward_sojourn2(n, P, qv, prob, right, weight, vc[k], vb[k-1], vb[k-1], en, vtmp, vtmpNR);
//   }
//   dger(n, wg[1], baralpha, barvb[0], en);
//   
//   etotal = nn + omega * (1.0 - uu);
//   dxmy(n, alpha, eb);
//   diag(n, en, ez);
//   dxmy(n+n, Q, en);
//   dxmy(n, xi, ey);
//   // eb *= alpha;
//   // ez = sci::diag(en);
//   // en *= Q;
//   // ey *= xi;
//   return llf;
// }

}
