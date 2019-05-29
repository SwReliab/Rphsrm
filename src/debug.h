#include <Rcpp.h>
using namespace Rcpp;

namespace marlib {

template<typename T>
void printvec(const char* s, const T& v) {
  using traits1 = double_vector<T>;
  const int n = traits1::size(v);
  Rcpp::Rcout << s << " ";
  for (int i=0; i<n; i++) {
    Rcpp::Rcout << v[i] << " ";
  }
  Rcpp::Rcout << std::endl;
}

}
