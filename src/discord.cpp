#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
//

// NumericVector subseries(const NumericVector& ts, int start, int end) {
//   if(start < 0 || end > ts.length()){
//     stop("provided start and stop indexes are invalid.");
//   }
//   NumericVector res(end-start);
//   for (int i=start; i<end; i++) {
//     res[i-start] = ts[i];
//   }
//   return res;
// }

discord_record find_best_discord_brute_force(const NumericVector& series,
                                             int w_size, VisitRegistry* globalRegistry) {

  // Rcout << "looking for the best discord, series length " << series.size() << "\n";

  double best_so_far_distance = -1.0;
  int best_so_far_index = -1;

  VisitRegistry outerRegistry(series.size() - w_size);

  int outer_idx = outerRegistry.getNextUnvisited();

  while(!(-1==outer_idx)){

    outerRegistry.markVisited(outer_idx);
    if(globalRegistry->isVisited(outer_idx)){
      // Rcout << " skipping " << outer_idx << ", marked as visited in global\n";
      outer_idx = outerRegistry.getNextUnvisited();
      continue;
    }
    // Rcout << " outer unvisited candidate at " << outer_idx << "\n";


    NumericVector candidate_seq = subseries(series, outer_idx, outer_idx + w_size);

    double nnDistance = std::numeric_limits<double>::max();
    VisitRegistry innerRegistry(series.size() - w_size);

    int inner_idx = innerRegistry.getNextUnvisited();
    while(!(-1==inner_idx)){
      innerRegistry.markVisited(inner_idx);
      // Rcout << "examining the subsequences starting at outer " << outer_idx << " and inner " << inner_idx << "\n";

      if(std::abs(inner_idx - outer_idx) > w_size) {

        NumericVector curr_seq = subseries(series, inner_idx, inner_idx + w_size);
        double dist = early_abandoned_dist(candidate_seq, curr_seq, nnDistance);
        // Rcout << "  .. dist:  " << dist << " best dist " << nnDistance << "\n";

        if ( (!std::isnan(dist)) && dist < nnDistance) {
          nnDistance = dist;
        }

      }

      inner_idx = innerRegistry.getNextUnvisited();
    }

    if (!(std::numeric_limits<double>::max() == nnDistance)
          && nnDistance > best_so_far_distance) {
      // Rcout << "** updating discord " << nnDistance << " at " << outer_idx << "\n";
      best_so_far_distance = nnDistance;
      best_so_far_index = outer_idx;
    }

    outer_idx = outerRegistry.getNextUnvisited();
  }

  struct discord_record res;
  res.index = best_so_far_index;
  res.nn_distance = best_so_far_distance;
  return res;
}


//' Finds a discord using brute force algorithm.
//'
//' @param ts the input timeseries.
//' @param w_size the sliding window size.
//' @param discords_num the number of discords to report.
//' @useDynLib jmotif
//' @export
//' @references Keogh, E., Lin, J., Fu, A.,
//' HOT SAX: Efficiently finding the most unusual time series subsequence.
//' Proceeding ICDM '05 Proceedings of the Fifth IEEE International Conference on Data Mining
//' @examples
//' discords = find_discords_brute_force(ecg0606[1:600], 100, 1)
//' plot(ecg0606[1:600], type = "l", col = "cornflowerblue", main = "ECG 0606")
//' lines(x=c(discords[1,2]:(discords[1,2]+100)),
//'    y=ecg0606[discords[1,2]:(discords[1,2]+100)], col="red")
// [[Rcpp::export]]
Rcpp::DataFrame find_discords_brute_force(
    NumericVector ts, int w_size, int discords_num) {

  std::map<int, double> res;

  VisitRegistry registry(ts.length());
  registry.markVisited(ts.length() - w_size, ts.length());

  // Rcout << "starting search of " << discords_num << " discords..." << "\n";

  int discord_counter = 0;
  while(discord_counter < discords_num){

    discord_record rec = find_best_discord_brute_force(ts, w_size, &registry);

    //     Rcout << "found a discord " << discord_counter << " at " << rec.index;
    //     Rcout << ", NN distance: " << rec.nn_distance << "\n";

    if(rec.nn_distance == 0 || rec.index == -1){ break; }

    res.insert(std::make_pair(rec.index, rec.nn_distance));

    int start = rec.index - w_size;
    if(start<0){
      start = 0;
    }

    int end = rec.index + w_size;
    // it can't be greater
    // if(end>=ts.length()){
    //  end = ts.length();
    //}

    // Rcout << "marking as visited from " << start << " to " << end << "\n";
    registry.markVisited(start, end);
    discord_counter = discord_counter + 1;
  }

  std::vector<int> positions;
  std::vector<double > distances;

  for(std::map<int, double>::iterator it = res.begin(); it != res.end(); it++) {
    positions.push_back(it->first);
    distances.push_back(it->second);
  }
  // make results
  return Rcpp::DataFrame::create(
    Named("nn_distance") = distances,
    Named("position") = positions
  );
}
