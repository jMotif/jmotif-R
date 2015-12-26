#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>

//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector ts_to_intervals(NumericVector ts, int w_size, int paa_size,
       int a_size, CharacterVector nr_strategy, double n_threshold = 0.01){

  std::unordered_map<int, std::string> sax_map = _sax_via_window(
    Rcpp::as<std::vector<double>>(ts), w_size, paa_size, a_size,
    Rcpp::as<std::string>(nr_strategy), n_threshold);

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

  return wrap(indexes);

//   std::string s = Rcpp::as<std::string>(str);
//
//   std::map<int, RuleRecord> rules = _str_to_repair_grammar(s);
//
//   // make results
//   Rcpp::List res(rules.size());
//   for(std::map<int, RuleRecord>::iterator it = rules.begin(); it != rules.end(); ++it) {
//
//     std::stringstream ss;
//     ss << it->second.rule_id;
//     Rcpp::CharacterVector rule_name = "R" + ss.str();
//
//     Rcpp::CharacterVector rule_string = it->second.rule_string;
//     Rcpp::CharacterVector expanded_rule_string = it->second.expanded_rule_string;
//     Rcpp::NumericVector rule_interval_starts = Rcpp::wrap(it->second.rule_occurrences);
//     Rcpp::NumericVector rule_interval_ends = rule_interval_starts +
//       count_spaces(&it->second.expanded_rule_string);
//
//     res[it->first] = List::create(
//       _["rule_name"]  = rule_name,
//       _["rule_string"]  = rule_string,
//       _["expanded_rule_string"] = expanded_rule_string,
//       _["rule_interval_starts"] = rule_interval_starts,
//       _["rule_interval_ends"] = rule_interval_ends
//     ) ;
//   }

  // return Rcpp::List();

}
