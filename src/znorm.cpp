#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
//
//' Z-normalizes a time series by subtracting the mean value and dividing by the standard deviation value.
//'
//' @param ts a time series to process.
//' @param threshold a z-normalization threshold value, if the input time series' standard deviation is
//' found less than this value, the procedure is not applied, so the noise would not get overamplified.
//' @useDynLib jmotif
//' @export
//' @references Dina Goldin and Paris Kanellakis,
//' On similarity queries for time-series data: Constraint specification and implementation.
//' In Principles and Practice of Constraint Programming (CP 1995), pages 137-153. (1995)
//' @examples
//' x = seq(0, pi*4, 0.02)
//' y = sin(x) * 5 + rnorm(length(x))
//' plot(x, y, type="l", col="blue")
//' lines(x, znorm(y, 0.01), type="l", col="red")
// [[Rcpp::export]]
NumericVector znorm(NumericVector ts, double threshold  = 0.01) {
  double ts_sd = sd(ts);
  if (ts_sd < threshold){
    return clone(ts);
  }
  return (ts - mean(ts)) / ts_sd;
}
