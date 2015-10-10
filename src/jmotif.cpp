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

//' Reshape matrix
//'
//' @param a A matrix to reshape.
//' @param n new row size.
//' @param m new column size.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
NumericMatrix reshape_cpp(NumericMatrix a, int n, int m) {

  int ce = 0;
  int n_rows = a.nrow();

  NumericMatrix res(n, m);

  for (int j = 0; j < m; j++) {
    for (int i = 0; i < n; i++) {
      res(i,j) = a(ce % n_rows, ce / n_rows);
      ce++;
    }
  }

  return res;

}
