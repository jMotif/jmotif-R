#include <jmotif.h>

// this finds the best discord given a time series, sliding window size, and a
// "registry" of the position which are to be tested
discord_record find_best_discord_brute_force(const NumericVector& series,
                                             int w_size, VisitRegistry* globalRegistry) {

  double best_so_far_distance = -1.0;
  int best_so_far_index = -1;
  unsigned int dist_calls = 0;

  VisitRegistry outerRegistry(series.size() - w_size);
  int outer_idx = outerRegistry.getNextUnvisited();

  while(!(-1==outer_idx)){ // while there is a position to try

    outerRegistry.markVisited(outer_idx);
    if(globalRegistry->isVisited(outer_idx)){
      outer_idx = outerRegistry.getNextUnvisited();
      continue; // break the loop
    }

    NumericVector candidate_seq = subseries(series, outer_idx, outer_idx + w_size);

    double nnDistance = std::numeric_limits<double>::max();
    VisitRegistry innerRegistry(series.size() - w_size);

    int inner_idx = innerRegistry.getNextUnvisited();
    while(!(-1==inner_idx)){
      innerRegistry.markVisited(inner_idx);
      if(std::abs(inner_idx - outer_idx) > w_size) { // if no overlap between two intervals
        NumericVector curr_seq = subseries(series, inner_idx, inner_idx + w_size);
        double dist = early_abandoned_dist(candidate_seq, curr_seq, nnDistance);
        //double dist = euclidean_dist(candidate_seq, curr_seq);
        dist_calls = dist_calls + 1;
        if ( (!std::isnan(dist)) && dist < nnDistance) {
          nnDistance = dist;
        }
      }
      inner_idx = innerRegistry.getNextUnvisited();
    }

    if (!(std::numeric_limits<double>::max() == nnDistance)
          && (nnDistance > best_so_far_distance)) {
      best_so_far_distance = nnDistance;
      best_so_far_index = outer_idx;
    }

    outer_idx = outerRegistry.getNextUnvisited();
  }

  struct discord_record res;
  res.index = best_so_far_index;
  res.nn_distance = best_so_far_distance;
  res.dist_calls = dist_calls;
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

  std::vector<int> positions;
  std::vector<unsigned int> distance_calls;
  std::vector<double > distances;

  VisitRegistry registry(ts.length());
  registry.markVisited(ts.length() - w_size, ts.length());

  int discord_counter = 0;
  while(discord_counter < discords_num){

    discord_record rec = find_best_discord_brute_force(ts, w_size, &registry);

    if(rec.nn_distance == 0 || rec.index == -1){ break; }

    positions.push_back(rec.index);
    distances.push_back(rec.nn_distance);
    distance_calls.push_back(rec.dist_calls);

    int start = rec.index - w_size;
    if(start<0){
      start = 0;
    }

    int end = rec.index + w_size;

    registry.markVisited(start, end);
    discord_counter = discord_counter + 1;
  }

  // make results
  return Rcpp::DataFrame::create(
    Named("nn_distance") = distances,
    Named("position") = positions,
    Named("distance_calls") = distance_calls
  );
}
