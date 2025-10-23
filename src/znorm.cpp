#include <jmotif.h>

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
NumericVector znorm(NumericVector ts, double threshold = 0.01) {

  if (ts.size() == 0) stop("Input vector cannot be empty");

  std::vector<double> ts_std = as<std::vector<double>>(ts);
  std::vector<double> out = _znorm(ts_std, threshold);
  return wrap(out);
}

std::vector<double> _znorm(const std::vector<double>& ts, double threshold) {

  double sum = std::accumulate(ts.begin(), ts.end(), 0.0);
  double mean =  sum / ts.size();
  // Rcout << " mean2 " << mean << "\n";

  std::vector<double> diff(ts.size());
  //std::transform(ts.begin(), ts.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean));
  for(unsigned i=0; i<ts.size(); i++)
    diff[i] = ts[i]-mean;

  double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
  double stdev = std::sqrt(sq_sum / (ts.size()-1));
  // Rcout << " stdev2 " << stdev << "\n";

  if (stdev < threshold) return ts; // return clone

  for (unsigned i = 0; i < ts.size(); i++)
    diff[i] = (ts[i] - mean) / stdev;

  return diff;

}
