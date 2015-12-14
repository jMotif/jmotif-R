#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
//
//' Computes a Piecewise Aggregate Approximation (PAA) for a time series.
//'
//' @param ts a timeseries to compute the PAA for.
//' @param paa_num the desired PAA size.
//' @useDynLib jmotif
//' @export
//' @references Keogh, E., Chakrabarti, K., Pazzani, M., Mehrotra, S.,
//' Dimensionality reduction for fast similarity search in large time series databases.
//' Knowledge and information Systems, 3(3), 263-286. (2001)
//' @examples
//' x = c(-1, -2, -1, 0, 2, 1, 1, 0)
//' plot(x, type = "l", main = "8-points time series and it PAA transform into three points")
//' points(x,pch = 16, lwd = 5)
//' # segments
//' abline(v = c(1, 1+7/3, 1+7/3 * 2, 8), lty = 3, lwd = 2)
// [[Rcpp::export]]
NumericVector paa(NumericVector ts, int paa_num) {

  // fix the length
  int len = ts.length();

  // check for the trivial case
  if (len == paa_num) {
    return clone(ts);
  }
  else {
    // if the number of points in a segment is even
    if (len % paa_num == 0) {
      std::vector<double> res(paa_num);
      int inc = len / paa_num;
      for (int i = 0; i < len; i++) {
        int idx = i / inc; // the spot
        res[idx] = res[idx] + ts[i];
      }
      double dl = (double) (inc);
      for (int i = 0; i < paa_num; i++) {
        res[i] = res[i] / dl;
      }
      return wrap(res);
    }else{
      // if the number of points in a segment is odd
      std::vector<double> res(paa_num);
      for (int i = 0; i < len * paa_num; i++) {
        int idx = i / len; // the spot
        int pos = i / paa_num; // the col spot
        res[idx] = res[idx] + ts[pos];
      }
      double dl = (double) len;
      for (int i = 0; i < paa_num; i++) {
        res[i] = res[i] / dl;
      }
      return wrap(res);
    }
  }

}
