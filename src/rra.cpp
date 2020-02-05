#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
#include <limits>
#include <chrono>
#include <ctime>

class rule_interval {
public:
  int rule_id;
  int start;
  int end;
  double cover;
};

struct sort_intervals {
  bool operator()(const rule_interval &left, const rule_interval &right) {
    return (left.cover < right.cover);
  }
};

double _normalized_distance(int start1, int end1, int start2, int end2, std::vector<double> *series){

  double res = 0;
  int count = 0;
  int len1 = end1 - start1;
  int len2 = end2 - start2;

  if(len1 == len2){
    for(int i=0; i<len1; i++){
      res = res + (series->at(start1+i) - series->at(start2+i)) *
                        (series->at(start1+i) - series->at(start2+i));
      count++;
    }
    return sqrt(res) / (double) count;
  }

  int min_length = std::min(len1, len2);
  if(len1 == min_length){
    std::vector<double> subseries(len2);
    for(int i=0; i<len2; i++){
      subseries[i] = series->at(start2+i);
    }
    std::vector<double> subseries_paa = _paa2(subseries, len1);
    for(int i=0; i<len1; i++){
      res = res + (series->at(start1+i) - subseries_paa[i]) *
        (series->at(start1+i) - subseries_paa[i]);
      count++;
    }
    return sqrt(res) / (double) count;

  } else {
    std::vector<double> subseries(len1);
    for(int i=0; i<len1; i++){
      subseries[i] = series->at(start1+i);
    }
    std::vector<double> subseries_paa = _paa2(subseries, len2);
    for(int i=0; i<len2; i++){
      res = res + (subseries_paa[i] - series->at(start2+i)) *
        (subseries_paa[i] - series->at(start2+i));
      count++;
    }
    return sqrt(res) / (double) count;
  }
}

double _shrinked_distance(int start1, int end1, int start2, int end2, std::vector<double> *series){

  double res = 0;
  int count = 0;
  int len1 = end1 - start1;
  int len2 = end2 - start2;

  if(len1 == len2) {
    for(int i=0; i<len1; i++){
      res = res + (series->at(start1+i) - series->at(start2+i)) *
        (series->at(start1+i) - series->at(start2+i));
      count++;
    }
    return sqrt(res) / (double) count;
  } else {
    int min_length = std::min(len1, len2);
    for(int i=0; i<min_length; i++){
      res = res + (series->at(start1+i) - series->at(start2+i)) *
        (series->at(start1+i) - series->at(start2+i));
      count++;
    }
    return sqrt(res) / (double) count;
  }
}

double _mean(std::vector<int> *ts, int *start, int *end){
  int sum = 0;
  for(int i=*start; i<*end; i++){
    sum = sum + ts->at(i);
  }
  return (double) sum / (double) (*end - *start);
}

rra_discord_record find_best_rra_discord(std::vector<double> *ts, int w_size,
      std::unordered_map<int, rule_record*> *grammar, std::vector<int> *indexes,
      std::vector<rule_interval> *intervals,
      std::unordered_set<int> *global_visited_positions){

  // *****
  // std::chrono::time_point<std::chrono::system_clock> tstart0, tstart, tend;
  // tstart0 = std::chrono::system_clock::now();
  int distance_calls_counter = 0;

  std::vector<int> visit_array(ts->size(), -1);

  //   for(auto it=intervals.begin(); it!=intervals.end(); ++it){
  //     Rcout << "R" << it->rule_id << " covr " << it->cover << std::endl;
  //   }

  // init variables
  int bestSoFarPosition = -1;
  int bestSoFarLength = -1;
  int bestSoFarRule = 0;
  double bestSoFarDistance = -1;

  // outer loop over all intervals
  for(int i = 0; i < (int) intervals->size(); i++){

    // ****
    // tstart = std::chrono::system_clock::now();

    rule_interval c_interval = intervals->at(i);

    // Rcout << c_interval.rule_id << ", " << c_interval.cover << std::endl;

    auto find = global_visited_positions->find(c_interval.start);
    if(find != global_visited_positions->end()){
      continue;
    }

    // mark the interval location
    std::unordered_set<int> visited_locations;
    visited_locations.reserve(ts->size());

    int markStart = c_interval.start - (c_interval.end - c_interval.start);
    if (markStart < 0) {
      markStart = 0;
    }
    int markEnd = c_interval.end;
    if (markEnd > (int) ts->size()) {
      markEnd = ts->size();
    }
    for(int j=markStart;j<markEnd;j++){
      visited_locations.emplace(j);
    }

    // initialize the distance
    double nn_distance = std::numeric_limits<double>::max();

    // by default, we engage the random search
    bool do_random_search = true;

     //Rcout << " considering interval " << c_interval.start << "-" << c_interval.end <<
    //   " for rule " << c_interval.rule_id <<
    //    ", best so far dist " << bestSoFarDistance << std::endl;

    auto this_rule_occurrences = grammar->at(c_interval.rule_id)->rule_intervals;
//     Rcout << "   going to iterate over " << this_rule_occurrences.size() <<
//      " rule occurrences first " << std::endl;

    for(auto it=this_rule_occurrences.begin(); it !=this_rule_occurrences.end(); ++it) {
      // Rcout << "0.0\n";
      int start = indexes->at(it->first);
      // Rcout << "0.1" << start << "\n";
      auto found = visited_locations.find(start);
      if (found == visited_locations.end()) {
        visited_locations.emplace(start);
        int end = indexes->at(it->second) + w_size;
        // Rcout << "    examining a candidate at " << start << "-" <<
         // end << std::endl;
        distance_calls_counter++;
        double dist = _normalized_distance(c_interval.start, c_interval.end,
                                          start, end, ts);
        // keep track of best so far distance
        if (dist < nn_distance) {
          // Rcout << "    better nn distance found " << dist << std::endl;
          nn_distance = dist;
        }
        if (dist < bestSoFarDistance) {
//           Rcout << "   R " << c_interval.rule_id << ", dist " << dist <<
//            " is less than best so far, breaking off the search" << std::endl;
          do_random_search = false;
          break;
        }
      } else {
        continue;
      }
    }

    // ****
    // tend = std::chrono::system_clock::now();
    // std::chrono::duration<double> elapsed_seconds = tend - tstart;
    // std::cout << "    . pre-search part done in " << elapsed_seconds.count() << "s\n";
    // tstart = std::chrono::system_clock::now();

    if(do_random_search){
//       Rcout << " starting the random search ..." <<
//         " nn dist " << nn_distance << std::endl;
      // Rcout << "visited locations ";
      // for(auto it=visited_locations.begin(); it != visited_locations.end(); ++it){
      //  Rcout << *it << ", ";
      //}
      //Rcout << std::endl;

      // init the visit array
      int cIndex = 0;
      for (unsigned j = 0; j < intervals->size(); j++) {
        rule_interval interval = intervals->at(j);
        auto found = visited_locations.find(interval.start);
        if (found == visited_locations.end()) {
          visit_array[cIndex] = j;
          cIndex++;
          //} else {
          // Rcout << "    - skipped " << interval.start << std::endl;
          //}
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

        rule_interval randomInterval = intervals->at(visit_array[cIndex]);
        cIndex--;

        //Rcout << "    random candidate " << randomInterval.start << "-" <<
        //  randomInterval.end << ", cindex " << cIndex << std::endl;

        distance_calls_counter++;
        double dist = _normalized_distance(c_interval.start, c_interval.end,
                        randomInterval.start, randomInterval.end, ts);
        if (dist < bestSoFarDistance) {
          // Rcout << "    dist " << dist <<
          //   " is less than best so far, breaking off the search" << std::endl;
          nn_distance = dist;
          break;
        }
        if (dist < nn_distance) {
          // Rcout << "    better nn distance found " << dist << std::endl;
          nn_distance = dist;
        }

      }

      // ****
      // tend = std::chrono::system_clock::now();
      // std::chrono::duration<double> elapsed_seconds = tend - tstart;
      // std::cout << "    . random search done in " << elapsed_seconds.count() << "s\n";
      // tstart = std::chrono::system_clock::now();

    } // random search

    if(nn_distance > bestSoFarDistance){
      bestSoFarDistance = nn_distance;
      bestSoFarPosition = c_interval.start;
      bestSoFarLength = c_interval.end - c_interval.start;
      bestSoFarRule = c_interval.rule_id;
      // Rcout << "    updating the discord " << nn_distance << " at " << bestSoFarPosition <<
       // " of length " << bestSoFarLength << " for rule " << bestSoFarRule << std::endl;
    }


  }

  // Rcout << "  RRA, distance calls: " << distance_calls_counter << std::endl;

  rra_discord_record res;
  res.rule = bestSoFarRule;
  res.start = bestSoFarPosition;
  res.end = bestSoFarPosition + bestSoFarLength;
  res.nn_distance = bestSoFarDistance;

  return res;
}

//' Finds a discord with RRA (Rare Rule Anomaly) algorithm.
//' Usually works the best with higher than that for HOT-SAX sizes of discretization parameters
//' (i.e., PAA and Alphabet sizes).
//'
//' @param series the input timeseries.
//' @param w_size the sliding window size.
//' @param paa_size the PAA size.
//' @param a_size the alphabet size.
//' @param nr_strategy the numerosity reduction strategy ("none", "exact", "mindist").
//' @param n_threshold the normalization threshold.
//' @param discords_num the number of discords to report.
//' @useDynLib jmotif
//' @export
//' @references Senin Pavel and Malinchik Sergey,
//' SAX-VSM: Interpretable Time Series Classification Using SAX and Vector Space Model.,
//' Data Mining (ICDM), 2013 IEEE 13th International Conference on.
//' @examples
//' discords = find_discords_rra(ecg0606, 100, 4, 4, "none", 0.01, 1)
//' plot(ecg0606, type = "l", col = "cornflowerblue", main = "ECG 0606")
//' lines(x=c(discords[1,2]:(discords[1,2]+100)),
//'    y=ecg0606[discords[1,2]:(discords[1,2]+100)], col="red")
// [[Rcpp::export]]
Rcpp::DataFrame find_discords_rra(NumericVector series, int w_size, int paa_size,
  int a_size, CharacterVector nr_strategy, double n_threshold,
  int discords_num){

  // *****
  // std::chrono::time_point<std::chrono::system_clock> tstart0, tstart, tend;
  // tstart0 = std::chrono::system_clock::now();
  // tstart = std::chrono::system_clock::now();

  std::vector<double> ts = Rcpp::as<std::vector<double>>(series);
  std::vector<int> visit_array(ts.size(), -1);

  std::unordered_map<int, std::string> sax_map = _sax_via_window(
    ts, w_size, paa_size, a_size, Rcpp::as<std::string>(nr_strategy), n_threshold);

  // ****
  // tend = std::chrono::system_clock::now();
  // std::chrono::duration<double> elapsed_seconds = tend - tstart;
  // Rcout << "  sax conversion: " << elapsed_seconds.count() << "s\n";
  // tstart = std::chrono::system_clock::now();

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

  // Rcout << "  there are " << indexes.size() << " SAX words..." << std::endl;

  // now compose the string
  //
  std::string sax_str;
  for(unsigned i=0; i<indexes.size(); i++){
    sax_str.append(sax_map[ indexes[i] ]).append(" ");
  }
  sax_str.erase( sax_str.end()-1 );

  // ****
  // tend = std::chrono::system_clock::now();
  // elapsed_seconds = tend - tstart;
  // Rcout << "  sax string composition: " << elapsed_seconds.count() << "s\n";

  // grammar
  //
  // *****
  // tstart = std::chrono::system_clock::now();
  std::unordered_map<int, rule_record*> grammar = _str_to_repair_grammar(sax_str);

  // ****
  // tend = std::chrono::system_clock::now();
  // elapsed_seconds = tend - tstart;
  // Rcout << "  grammar inferred in: " << elapsed_seconds.count() << "s\n";
  // Rcout << "  there are " << grammar.size() << " RePair rules including R0..." << std::endl;

  // *****
  // tstart = std::chrono::system_clock::now();

  // making intervals and ranking by the rule use
  // meanwhile build the coverage curve
  //
  std::vector<rule_interval> intervals;
  std::vector<int> coverage_array(ts.size(), 0);
  for(auto it = grammar.begin(); it != grammar.end(); ++it) {
    if(0 == it->first){
      continue; // skip R0
    }
    for(auto rit = (it->second)->rule_intervals.begin(); rit != (it->second)->rule_intervals.end(); ++rit) {
      int t_start = rit->first;
      int t_end = rit->second;
      // start and end here is for the string tokens, not for time series points
      //
      int start = indexes[t_start];
      int end = indexes[t_end] + w_size;
      // Rcout << start << "_" << end << std::endl;
      // Rcout << " * rule interval " << start << " " << end << std::endl;
      for(int i=start; i<end; ++i){ // rule coverage
        coverage_array[i] = coverage_array[i] + 1;
      }
      rule_interval rr;
      rr.rule_id = it->first;
      rr.start = start;
      rr.end = end;
      // rr.cover = it->second->rule_use; // a shortcut
      intervals.push_back(rr);
    }
  }

  // we need to examine the coverage curve for continous zero intervals and mark those
  //
  rule_record* rec_zero_cover = new rule_record();
  rec_zero_cover->rule_id = -1;
  rec_zero_cover->rule_string = "xxx";
  rec_zero_cover->expanded_rule_string = "xxx";
  bool need_placement = false;
  int start = -1;
  bool in_interval = false;
  for (unsigned i = 0; i < coverage_array.size(); i++) {
    if (0 == coverage_array[i] && !in_interval) {
      start = i;
      in_interval = true;
    }
    if (coverage_array[i] > 0 && in_interval) {

      need_placement = true;

      rule_interval ri;
      ri.cover=0;
      ri.start = start;
      ri.end=i;
      ri.rule_id=-1;
      intervals.push_back(ri);

      rec_zero_cover->rule_occurrences.push_back(start);
      rec_zero_cover->rule_intervals.push_back(std::make_pair(start, i));

      intervals.push_back(ri);

      in_interval = false;
      // Rcout << " zero coverage from " << start << " to " << i << std::endl;
    }
  }

  if(need_placement) {
    grammar.emplace(std::make_pair(-1, rec_zero_cover));
  }

  // compute the coverage
  for(auto it=intervals.begin(); it !=intervals.end(); ++it){
    it->cover = _mean(&coverage_array, &it->start, &it->end);
  }

  // sort the intervals rare < frequent
  std::sort(intervals.begin(), intervals.end(), sort_intervals());


  // ******
  // tend = std::chrono::system_clock::now();
  // elapsed_seconds = tend - tstart;
  // Rcout << "  there are " << intervals.size() <<" rule intervals inferred in "
  //       << elapsed_seconds.count() << std::endl;
  // Rcout << "  top coverage for interval of rule " << intervals[0].rule_id << " starting at "
  //       << intervals[0].start << " ending at " << intervals[0].end << " : " << intervals[0].cover
  //       << std::endl;
  // int last_idx = intervals.size() - 1;
  // Rcout << "  bottom coverage for interval of rule " << intervals[last_idx].rule_id
  //       << " starting at " << intervals[last_idx].start << " ending at " << intervals[last_idx].end
  //       << " : " << intervals[last_idx].cover << std::endl;
  // tstart = std::chrono::system_clock::now();

  // from here on we'll be calling find best discord...
  std::unordered_set<int> global_visited_positions;
  // global_visited_positions.reserve(ts.size());

  std::vector<rra_discord_record> discords;

  while((int) discords.size() < discords_num){

    // tstart = std::chrono::system_clock::now();

    rra_discord_record d = find_best_rra_discord(&ts, w_size, &grammar,
                              &indexes, &intervals, &global_visited_positions);
    // Rcout << d.nn_distance;

    if(d.nn_distance<0){
      break;
    }
    discords.push_back(d);

    // mark the location
    int markStart = d.start - (d.end - d.start);
    if (markStart < 0) {
      markStart = 0;
    }
    int markEnd = d.end;
    if (markEnd > (int) ts.size()) {
      markEnd = ts.size();
    }
    for(int j=markStart;j<markEnd;j++){
      global_visited_positions.emplace(j);
    }

    // *****
    // tend = std::chrono::system_clock::now();
    // elapsed_seconds = tend - tstart;
    // Rcout << "  search for discord " << discords.size() - 1 <<" finished in "
    //       << elapsed_seconds.count() << std::endl;
  }

//   tend = std::chrono::system_clock::now();
//   elapsed_seconds = tend - tstart;
//   std::cout << "discord found in: " << elapsed_seconds.count() << "s\n";

  // make results
  std::vector<int> rule_ids;
  std::vector<int> starts;
  std::vector<int> ends;
  std::vector<int> lengths;
  std::vector<double > nn_distances;


  for(auto it = discords.begin(); it != discords.end(); it++) {
    rule_ids.push_back(it->rule);
    starts.push_back(it->start);
    ends.push_back(it->end);
    lengths.push_back(it->end - it->start);
    nn_distances.push_back(it->nn_distance);
  }

  // make results
  return Rcpp::DataFrame::create(
    Named("rule_id") = rule_ids,
    Named("start") = starts,
    Named("end") = ends,
    Named("length") = lengths,
    Named("nn_distance") = nn_distances
  );

}
