#include <Rcpp.h>
using namespace Rcpp;

//' z-Normalize a time series
//'
//' @param x A time series.
//' @param threshold A z-normalization threshold.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
NumericVector znorm_cpp(NumericVector x, double threshold = 0.01) {
  double x_sd = sd(x);
   if (x_sd < threshold){
    return clone(x);
  }
  return (x - mean(x)) / x_sd;
}


// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically
// run after the compilation.
//

// /*** R
// timesTwo(42)
// */
