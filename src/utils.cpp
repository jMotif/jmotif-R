#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
//

//' Computes the column means for a matrix.
//'
//' @param m a matrix to process.
//' @useDynLib jmotif
//' @export
//' @examples
//' x = matrix(rnorm(100), ncol=10)
//' col_means(x)
// [[Rcpp::export]]
NumericVector col_means(NumericMatrix m) {
  NumericVector res(m.ncol());
  for (int j = 0; j < m.ncol(); j++) {
    res[j] = mean(m(_,j));
  }
  return res;
}


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

//
// random generator wrapper
//
int armaRand() {
  arma::ivec x = arma::randi(1);
  return x(0);
}
