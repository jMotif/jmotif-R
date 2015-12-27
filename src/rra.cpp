#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
#include <limits>

class RuleInterval {
public:
  int rule_id;
  int start;
  int end;
  int cover;
};

struct sort_intervals {
  bool operator()(const RuleInterval &left,
                const RuleInterval &right) {
    return left.cover < right.cover;
  }
};

double normalized_distance(int start1, int end1, int start2, int end2, std::vector<double> *series){
  double res = 0;
  int count = 0;
  int length = std::min(end1-start1,end2-start2);
  for(int i=0; i<length; i++){
    res = res + (series->at(start1+i) - series->at(start2+i)) *
      (series->at(start1+i) - series->at(start2+i));
    count++;
  }
  return sqrt(res) / (double) count;
}

//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
Rcpp::DataFrame ts_to_intervals(NumericVector series, int w_size, int paa_size,
       int a_size, CharacterVector nr_strategy, double n_threshold = 0.01){

  std::vector<double> ts = Rcpp::as<std::vector<double>>(series);
  std::vector<int> visit_array(ts.size(), -1);

  std::unordered_map<int, std::string> sax_map = _sax_via_window(
    ts, w_size, paa_size, a_size, Rcpp::as<std::string>(nr_strategy), n_threshold);

  // sax_map maps time-series positions to corresponding SAX words
  // to compose the string we need to order keys
  //
  std::vector<int> indexes(sax_map.size());
  int i=0;
  for(auto it = sax_map.begin(); it != sax_map.end(); ++it) {
    indexes[i] = it->first;
    i++;
  }
  sort( indexes.begin(), indexes.end() );
  Rcout << "  there are " << indexes.size() << " SAX words..." << std::endl;

  // now compose the string
  //
  std::string sax_str;
  for(int i=0; i<indexes.size(); i++){
    sax_str.append(sax_map[ indexes[i] ]).append(" ");
  }
  sax_str.erase( sax_str.end()-1 );

  // grammar
  //
  std::map<int, RuleRecord> grammar = _str_to_repair_grammar(sax_str);
  Rcout << "  there are " << grammar.size() << " RePair rules including R0..." << std::endl;

  // making intervals and ranking by the rule use
  // meanwhile build the coverage curve
  //
  std::vector<RuleInterval> intervals;
  std::vector<int> coverage_array(ts.size(), 0);
  for(auto it = grammar.begin(); it != grammar.end(); ++it) {
    if(0 == it->first){
      continue;
    }
    for(auto rit = (it->second).rule_intervals.begin(); rit != (it->second).rule_intervals.end(); ++rit) {
      int t_start = rit->first;
      int t_end = rit->second;
      // start and end here is for the string tokens, not for time series points
      //
      int start = indexes[t_start];
      int end = indexes[t_end] + w_size;
      // Rcout << " * rule interval " << start << " " << end << std::endl;
      for(int i=start; i<end; ++i){ // rule coverage
        coverage_array[i] = coverage_array[i] + 1;
      }
      RuleInterval rr;
      rr.rule_id = it->first;
      rr.start = start;
      rr.end = end;
      rr.cover = it->second.rule_use; // a shortcut
      intervals.push_back(rr);
    }
  }

  Rcout << "  there are " << intervals.size() << " rule intervals to consider..." << std::endl;
  // we need to examine the coverage curve for continous zero intervals and mark those
  //
  int start = -1;
  bool in_interval = false;
  for (int i = 0; i < coverage_array.size(); i++) {
    if (0 == coverage_array[i] && !in_interval) {
      start = i;
      in_interval = true;
    }
    if (coverage_array[i] > 0 && in_interval) {
      RuleInterval ri;
      ri.cover=0;
      ri.start = start;
      ri.end=i;
      ri.rule_id=-1;
      intervals.push_back(ri);
      in_interval = false;
      Rcout << " zero coverage" << start << " to " << i << std::endl;
    }
  }
  Rcout << "  there are " << intervals.size() << " rule intervals including non-covered..." << std::endl;

  // sort the intervals rare < frequent
  std::sort(intervals.begin(), intervals.end(), sort_intervals());

  //   for(auto it=intervals.begin(); it!=intervals.end(); ++it){
  //     Rcout << "R" << it->rule_id << " covr " << it->cover << std::endl;
  //   }

  // init variables
  int bestSoFarPosition = -1;
  int bestSoFarLength = -1;
  int bestSoFarRule = 0;
  double bestSoFarDistance = -1;

  // outer loop over all intervals
  for(int i = 0; i < intervals.size(); i++){

    RuleInterval c_interval = intervals[i];

    // mark the location
    std::unordered_set<int> visited_locations;
    int markStart = c_interval.start - (c_interval.end - c_interval.start);
    if (markStart < 0) {
      markStart = 0;
    }
    int markEnd = c_interval.end;
    if (markEnd > ts.size()) {
      markEnd = ts.size();
    }
    for(int j=markStart;j<markEnd;j++){
      visited_locations.insert(j);
    }

    double nn_distance = std::numeric_limits<double>::max();
    bool do_random_search = true;

    Rcout << " considering interval " << c_interval.start << "-" << c_interval.end <<
      " for rule " << c_interval.rule_id <<
      ", best so far dist " << bestSoFarDistance << std::endl;

    if(c_interval.rule_id>0){
      auto this_rule_occurrences = grammar[c_interval.rule_id].rule_intervals;
      Rcout << "   going to iterate over " << this_rule_occurrences.size() <<
        " rule occurrences first " << std::endl;
      for(auto it=this_rule_occurrences.begin(); it !=this_rule_occurrences.end(); ++it) {
        int start = indexes[it->first];
        auto found = visited_locations.find(start);
        if (found == visited_locations.end()) {
          visited_locations.insert(start);
          int end = indexes[it->second] + w_size;
          Rcout << "    examining a candidate at " << start << "-" <<
            end << std::endl;
          double dist = normalized_distance(c_interval.start, c_interval.end,
                                            start, end, &ts);
          // keep track of best so far distance
          if (dist < nn_distance) {
            Rcout << "    better nn distance found " << dist << std::endl;
            nn_distance = dist;
          }
          if (dist < bestSoFarDistance) {
            Rcout << "    dist " << dist <<
              " is less than best so far, breaking off the search" << std::endl;
            do_random_search = false;
            break;
          }

        } else {
          continue;
        }
    }
  }

  if(do_random_search){
    Rcout << " starting the random search ..." <<
    " nn dist " << nn_distance << std::endl;
    Rcout << "visited locations ";
    for(auto it=visited_locations.begin(); it != visited_locations.end(); ++it){
      Rcout << *it << ", ";
    }
    Rcout << std::endl;

    // init the visit array
    int visitCounter = 0;
    int cIndex = 0;
    for (int j = 0; j < intervals.size(); j++) {
      RuleInterval interval = intervals[j];
      auto found = visited_locations.find(interval.start);
      if (found == visited_locations.end()) {
        visit_array[cIndex] = j;
        cIndex++;
      } else {
        Rcout << "    - skipped " << interval.start << std::endl;
      }
    }
    cIndex--;

    // shuffle the visit array
    for (int j = cIndex; j > 0; j--) {
      int index = armaRand() % (j + 1);
      int a = visit_array[index];
      visit_array[index] = visit_array[j];
      visit_array[j] = a;
    }

    // while there are unvisited locations
    while (cIndex >= 0) {

      RuleInterval randomInterval = intervals[visit_array[cIndex]];
      cIndex--;

      Rcout << "    random candidate " << randomInterval.start << "-" <<
        randomInterval.end << ", cindex " << cIndex << std::endl;

      double dist = normalized_distance(c_interval.start, c_interval.end,
                                        randomInterval.start, randomInterval.end, &ts);
      if (dist < bestSoFarDistance) {
        Rcout << "    dist " << dist <<
          " is less than best so far, breaking off the search" << std::endl;
        nn_distance = dist;
        break;
      }
      if (dist < nn_distance) {
        Rcout << "    better nn distance found " << dist << std::endl;
        nn_distance = dist;
      }

    }

  } // random search

  if(nn_distance > bestSoFarDistance){
    bestSoFarDistance = nn_distance;
    bestSoFarPosition = c_interval.start;
    bestSoFarLength = c_interval.end - c_interval.start;
    bestSoFarRule = c_interval.rule_id;
    Rcout << "    updating the discord " << nn_distance << " at " << bestSoFarPosition <<
      " of length " << bestSoFarLength << " for rule " << bestSoFarRule << std::endl;
  }

 }

  // make results
  return Rcpp::DataFrame::create(
    Named("nn_distance") = bestSoFarDistance,
    Named("position") = bestSoFarPosition,
    Named("Length") = bestSoFarLength,
    Named("RuleId") = bestSoFarRule
  );

}
