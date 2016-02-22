#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
//

//' Extracts a subseries.
//'
//' @param ts the input timeseries (0-based, left inclusive).
//' @param start the interval start.
//' @param end the interval end.
//' @useDynLib jmotif
//' @export
//' @examples
//' y = c(-1, -2, -1, 0, 2, 1, 1, 0)
//' subseries(y, 0, 3)
// [[Rcpp::export]]
NumericVector subseries(NumericVector ts, int start, int end) {
  if(start<0 || end>ts.length()){
    stop("provided start and stop indexes are invalid.");
  }

  NumericVector res(end-start);
  for (int i=start; i<end; i++) {
    res[i-start] = ts[i];
  }
  return res;
}

// subseries extraction, more performant way...
//
std::vector<double> _subseries(std::vector<double> *ts, int start, int end) {
  std::vector<double>::const_iterator first = ts->begin() + start;
  std::vector<double>::const_iterator last = ts->begin() + end;
  std::vector<double> res(first, last);
  return res;
}

// the random generator wrapper
//
int armaRand() {
  arma::ivec x = arma::randi(1);
  return x(0);
}
