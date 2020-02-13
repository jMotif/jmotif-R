#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
//

//' Z-normalizes a time series by subtracting its mean and dividing by the standard deviation.
//'
//' @param ts the input time series.
//' @param threshold the z-normalization threshold value, if the input time series' standard
//' deviation will be found less than this value, the procedure will not be applied,
//' so the "under-threshold-noise" would not get amplified.
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
  // double ts_sd = sd(ts);
  // if (ts_sd < threshold){
  //  return clone(ts);
  // }
  // Rcout << " mean1 " << mean(ts)  << "\n";
  // Rcout << " stdev1 " << ts_sd << "\n";
  // return (ts - mean(ts)) / ts_sd;
  return wrap(_znorm(Rcpp::as< std::vector<double> >(ts), threshold));
}

// this is the main implementation, above is the wrapper
//
std::vector<double> _znorm(std::vector<double> ts, double threshold) {

  double sum = std::accumulate(std::begin(ts), std::end(ts), 0.0);
  double mean =  sum / ts.size();
  // Rcout << " mean2 " << mean << "\n";

  std::vector<double> diff(ts.size());
  //std::transform(ts.begin(), ts.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean));
  for(unsigned i=0; i<ts.size(); i++)
    diff[i] = ts[i]-mean;

  double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
  double stdev = std::sqrt(sq_sum / (ts.size()-1));
  // Rcout << " stdev2 " << stdev << "\n";

  if (stdev < threshold){
    std::vector<double> res(ts);
    return res;
  }

  std::vector<double> res(ts.size());
  for(unsigned i=0; i<ts.size(); i++){
    res[i] = (ts[i]-mean)/stdev;
  }
  return res;

}
