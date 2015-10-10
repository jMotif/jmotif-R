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

//' Computes column means
//'
//' @param a A matrix to use.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
NumericVector col_means_cpp(NumericMatrix a) {
  NumericVector res(a.ncol());
  for (int j = 0; j < a.ncol(); j++) {
    res[j] = mean(a(_,j));
  }
  return res;
}

//' Compute PAA
//'
//' @param ts A timeseries to convert into PAA.
//' @param paa_num the desired PAA size.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
NumericVector paa_cpp(NumericVector ts, int paa_num) {

  // fix the length
  int len = ts.length();

  // check for the trivial case
  if (len == paa_num) {
    return clone(ts);
  }
  else {
      NumericVector res(paa_num);
      for (int i = 0; i < len * paa_num; i++) {
        int idx = i / len; // the spot
        int pos = i / paa_num; // the col spot
        res[idx] = res[idx] + ts[pos];
      }
      for (int i = 0; i < paa_num; i++) {
        res[i] = res[i] / (double) len;
      }
      return res;
  }

}
