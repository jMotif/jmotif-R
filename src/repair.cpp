#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
#include <stdlib.h>

// this is the digram priority queue sorter
//
struct sort_pred {
  bool operator()(const std::pair<std::string,int> &left,
                const std::pair<std::string,int> &right) {
    return left.second < right.second;
  }
};

// Runs the repair on a string.
std::map<int, RuleRecord> _str_to_repair_grammar(std::string s){

  // Rcout << "input string \'" << s << "\'\n making a digram table...\n";

  // define the objects we are working with
  std::string delimiter = " ";
  s.append(delimiter);

  // global to the function data structure
  std::vector<Token> R0; // this is the R0 tokens sequence
  R0.reserve(_count_spaces(&s));

  std::map<int, Rule> rules; // the grammar rules dictionary
  rules.insert(std::make_pair(0, Rule(0, "\0", "\0"))); // insert the R0 placeholder

  std::map<std::string, int> digrams_map; // digram counts map
  std::vector< std::pair<std::string, int> > digrams_vector; // digram - count pairs

  // tokenizer variables and counters
  std::string old_token;
  std::string token;
  int token_counter = 0;
  int pos = 0;
  while ((pos = s.find(delimiter)) != std::string::npos) {

    token = s.substr(0, pos);
    // Rcout << "current token: " << token << std::endl;
    R0.push_back( Token(token, token_counter) );

    if (!old_token.empty()) {
      std::string d_str = old_token + " " + token;
      // Rcout << "new digram: " << d_str << std::endl;
      if (digrams_map.find(d_str) == digrams_map.end()){
        digrams_map.insert(std::make_pair(d_str, 1));
      }else{
        digrams_map[d_str]++;
      }
    }

    s.erase(0, pos + delimiter.length());
    old_token = token;
    token_counter++;
  }

  // all digrams are accounted for... print their state
  // Rcout << "\n\nthe digrams table\n=================" << std::endl;
  // for(std::map<std::string, int>::iterator it = digrams_map.begin(); it != digrams_map.end(); ++it) {
  //   Rcout << it->first << " " << it->second << std::endl;
  // }

  // initialize the fake priority queue which is the vector of pairs
  for(auto it = digrams_map.begin(); it != digrams_map.end(); ++it) {
    digrams_vector.push_back(std::make_pair(it->first, it->second) ); // frequencies vec new element
  }
  // sort the frequencies vector
  std::sort(digrams_vector.begin(), digrams_vector.end(), sort_pred());

  // remove the entries wich below 2, to shorten time for sorting, etc


  // Rcout << "\n\nsorted vec:\n=================" << std::endl;
  // for (std::vector<std::pair<std::string,int> >::iterator it = digrams_vector.begin();
  //      it != digrams_vector.end(); ++it) Rcout << it->first << " " << it->second << std::endl;
  // Rcout << '\n';

  // Rcout << "\n\nstarting Re-Pair on the string:\n=================" << std::endl;
  // Rcout << "R0 -> ";
  // for (std::vector<Token>::iterator it = R0.begin(); it != R0.end(); ++it) Rcout << it->payload << " ";
  // Rcout << std::endl;

  // start the main re-pair loop which continues until all the digrams have a frequency less than 2
  std::pair<std::string,int> top_digram = digrams_vector.back();
  while(top_digram.second > 1){

    // Rcout << "working with " << top_digram.first << " of freq " << top_digram.second << std::endl;

    // remove this digram
    std::string digram_str = top_digram.first;
    int digram_freq = top_digram.second;
    int space_idx = digram_str.find_first_of(" ", 0);
    std::string p = digram_str.substr(0, space_idx);
    std::string n = digram_str.substr(space_idx+1, digram_str.size());
    digrams_vector.pop_back(); // **************** removes the digram token from the table *********

    // create the new rule
    int rule_id = rules.size();
    Rule r(rule_id, digram_str, "\0");

    // update the rule use, if found
    if(p[0]=='R'){
      int rule_id = atoi(p.substr(1,p.size()-1).c_str());
      rules[rule_id].rule_use++;
      // Rcout << digram_str << " " << p << " updating " << rule_id << std::endl;
    }
    if(n[0]=='R'){
      int rule_id = atoi(n.substr(1,n.size()-1).c_str());
      rules[rule_id].rule_use++;
      // Rcout << digram_str << " " << n << " updating " << rule_id << std::endl;
    }

    // do a pass over the R0 substituting the digram string with R#
    int start = 0;
    int end = R0.size() - 1;
    int cp = start + 1;
    while(cp <= end){

      if(R0[cp-1].payload == p && R0[cp].payload == n) { // if the digram has been found

        // Rcout << "hit @ " << cp-1 << "\n";

        // update the first digram's token with the rule [cp-1]
        R0[cp-1] = Guard(r, R0[cp-1].str_idx);
        r.occurrences.push_back(R0[cp-1].str_idx);
        // and remove its second token
        R0.erase(R0.begin() + cp); // ******* removes the token at [cp] *******
        // update limits
        end = end - 1;

        // update digrams count table
        // Rcout << "  * decr. count of " << digram_str << ": " << digrams_map[digram_str] << " to ";
        digrams_map[digram_str]--;
        // Rcout << digrams_map[digram_str] << std::endl;

        // if not at the very beginning
        if(cp-1 > 0) {

          // update the NEW left digram
          std::string left_str = R0[cp-2].payload + " " + R0[cp-1].payload;
          if (digrams_map.find(left_str) == digrams_map.end()){
            digrams_map.insert(std::make_pair(left_str, 1));
            // Rcout << "  added a digram: " << left_str << ":1" << std::endl;
          }else{
            // Rcout << "  incr. count for a digram " << left_str << " from " << digrams_map[left_str];
            digrams_map[left_str]++;
            // Rcout << " to " << digrams_map[left_str] << std::endl;
          }

          // update the OLD left digram
          std::string old_left =  R0[cp-2].payload + " " + p;
          // Rcout << "  decr. count for a digram " << old_left << " from " << digrams_map[old_left];
          digrams_map[old_left]--;
          // Rcout << " to " << digrams_map[old_left] << std::endl;
          if(0 == digrams_map[old_left]){
            // Rcout << "  removing digram: " << old_left << std::endl;
            digrams_map.erase(old_left);
          }

        }

        // if not at the very end
        if(cp <= end) {

          // update the NEW right digram
          std::string right_str = R0[cp-1].payload + " " + R0[cp].payload;
          if (digrams_map.find(right_str) == digrams_map.end()){
            digrams_map.insert(std::make_pair(right_str, 1));
            // Rcout << "  added a digram: " << right_str << ":1" << std::endl;
          }else{
            // Rcout << "  incr. count for a digram " << right_str << " from " << digrams_map[right_str];
            digrams_map[right_str]++;
            // Rcout << " to " << digrams_map[right_str] << std::endl;
          }

          // update the OLD right digram
          std::string old_right =  n + " " + R0[cp].payload;
          // Rcout << "  decr. count for a digram " << old_right << " from " << digrams_map[old_right];
          digrams_map[old_right]--;
          // Rcout << " to " << digrams_map[old_right] << std::endl;
          if(0 == digrams_map[old_right]){
            // Rcout << "  removing digram: " << old_right << std::endl;
            digrams_map.erase(old_right);
          }

        }

        // need to reshuffle frequencies
        // Rcout << "\n\nvec before corrections:\n=================" << std::endl;
        // for (std::vector<std::pair<std::string,int> >::iterator it = digrams_vector.begin();
        //              it != digrams_vector.end(); ++it)
        // Rcout << it->first << " " << it->second << std::endl;
        // Rcout << '\n';

        // std::vector<int> elements_to_delete;
        // for (int i = 0; i< digrams_vector.size(); ++i){
        //  std::map<std::string, int>::iterator im =  digrams_map.find(digrams_vector[i].first);
        //   if(digrams_map.end() == im){
        //     Rcout << "  a digram " << digrams_vector[i].first <<
        //       " is in the vec but not map, cleaning elm at " << i << std::endl;
        //     elements_to_delete.push_back(i);
        //   } else {
        //     Rcout << "  updating vector counts for a digram " << digrams_vector[i].first <<
        //        " from " << digrams_vector[i].second << " to " << im->second << std::endl;
        //     digrams_vector[i].second = im->second;
        //   }
        // }
        // Rcout << "\n\nvec after corrections:\n=================" << std::endl;
        // for (std::vector<std::pair<std::string,int> >::iterator it = digrams_vector.begin();
        //      it != digrams_vector.end(); ++it)
        //   Rcout << it->first << " " << it->second << std::endl;
        // Rcout << '\n';
        // for (int i=elements_to_delete.size()-1; i>=0; --i){
        //   Rcout << "   deleting vector element at " << elements_to_delete[i] << ", i.e. " <<
        //     digrams_vector[elements_to_delete[i]].first << ":" << digrams_vector[elements_to_delete[i]].second <<
        //       std::endl;
        //   digrams_vector.erase(digrams_vector.begin() + elements_to_delete[i]);
        // }

        // Rcout << "\n\nvec after corrections:\n=================" << std::endl;
        // for (std::vector<std::pair<std::string,int> >::iterator it = digrams_vector.begin();
        //      it != digrams_vector.end(); ++it)
        // Rcout << it->first << " " << it->second << std::endl;
        // Rcout << '\n';
        // Rcout << "  *** working digram " << digram_str << ":" << digrams_map[digram_str] << std::endl;

        // if this was the last instance
        if(digrams_map[digram_str]==0){
          // Rcout << "  breaking the digram substitute loop and erasing dd" << std::endl;
          auto im =  digrams_map.find(digram_str);
          digrams_map.erase(digram_str);
          cp = end + 1; // this shall break the outer loop
        }

        // all digrams are accounted for, re-populate the priority queue
        digrams_vector.clear();
        digrams_vector.reserve(digrams_map.size());
        for(auto it = digrams_map.begin();
            it != digrams_map.end(); ++it) {
          digrams_vector.push_back(std::make_pair(it->first, it->second) );
        }

      } else {
        cp++; // continue with the search
      }

    } // while cp <= end

    rules.insert(std::make_pair(r.id, r));

    // Rcout << "\n\nthe digrams table\n=================" << std::endl;
    // for(std::map<std::string, int>::iterator it = digrams_map.begin();
    // it != digrams_map.end(); ++it) {
    // Rcout << it->first << " " << it->second << std::endl;
    // }

    // Rcout << "\n\nsorted vec:\n=================" << std::endl;
    // for (std::vector<std::pair<std::string,int> >::iterator it = digrams_vector.begin();
    // it != digrams_vector.end(); ++it)
    // Rcout << it->first << " " << it->second << std::endl;
    // Rcout << '\n';

    // Rcout << "R0 -> ";
    // for (std::vector<Token>::iterator it = R0.begin();
    // it != R0.end(); ++it)
    // Rcout << it->payload << " ";
    // Rcout << std::endl;

    // sort the priority queue
    std::sort(digrams_vector.begin(), digrams_vector.end(), sort_pred());
    // and select the new digram to work with
    top_digram = digrams_vector.back();
  }

  // re-populate R0 according to the rules table
  std::string new_r0 = "";
  for (std::vector<Token>::iterator it = R0.begin(); it != R0.end(); ++it) new_r0.append(it->payload + " ");
  new_r0.erase(new_r0.length()-1, new_r0.length());
  rules[0].rule_string = new_r0;

  // print the grammar
  //   Rcout << "\n\nInput: " << str << "\n\nInferred Grammar:\n";
  //   for(std::map<int, Rule>::iterator it = rules.begin(); it != rules.end(); ++it) {
  //   Rcout << it->second;
  //   for(std::vector<int>::iterator ito = it->second.occurrences.begin();
  //       ito != it->second.occurrences.end(); ++ito) {
  //     Rcout << *ito << ", ";
  //   }
  //   Rcout << std::endl;
  //   }
  //   Rcout << std::endl;

  // trying to expand the rules
  //
  for(std::map<int, Rule>::iterator it = rules.begin(); it != rules.end(); ++it) {
    it->second.expanded_rule_string = it->second.rule_string;
  }
  bool seen_r = true;
  while(seen_r){
    seen_r = false;
    for(std::map<int, Rule>::iterator it = rules.begin(); it != rules.end(); ++it) {
      std::string pre_expanded = it->second.expanded_rule_string;
      int pos = 0;
      // Rcout << "pre-expanded: " << pre_expanded << "\n";
      while ((pos = pre_expanded.find("R", pos)) != std::string::npos) {
        int space_pos = pre_expanded.find(" ", pos);
        if(space_pos == std::string::npos){
          space_pos = pre_expanded.length();
        }
        int rule_id = atoi(pre_expanded.substr(pos+1, space_pos-pos-1).c_str());
        // Rcout << pre_expanded.substr(pos, space_pos-pos) << " @ " << pos << ", rule " << rule_id << "\n";
        pre_expanded.erase(pos, space_pos-pos);
        std::string replacement = rules[rule_id].expanded_rule_string;
        pre_expanded.insert(pos, replacement);
        pos = pos + replacement.length();
      }
      // Rcout << "post-expanded " << pre_expanded << "\n";
      it->second.expanded_rule_string = pre_expanded;
      if(pre_expanded.find("R") != std::string::npos) {
        seen_r = true;
      }
    }
  }

  // print the grammar with expansion
  // Rcout << "\n\nFull Grammar:\n";
  // for(std::map<int, Rule>::iterator it = rules.begin(); it != rules.end(); ++it) {
  // Rcout << it->second << std::endl;
  // }
  // Rcout << std::endl;rule_str

  // make results
  std::map<int, RuleRecord> res;

  for(std::map<int, Rule>::iterator it = rules.begin(); it != rules.end(); ++it) {
    RuleRecord rr;
    rr.rule_id = it->first;
    rr.rule_string = it->second.rule_string;
    rr.expanded_rule_string = it->second.expanded_rule_string;
    rr.rule_use = it->second.rule_use;
    rr.rule_occurrences = it->second.occurrences;
    for(std::vector<int>::iterator oi = rr.rule_occurrences.begin();
        oi != rr.rule_occurrences.end(); ++oi) {
      rr.rule_intervals.push_back(std::make_pair(*oi,
                *oi + _count_spaces(&rr.expanded_rule_string)));
    }
    res.insert(std::make_pair(it->first, rr));
  }

  return res;

}


//' Runs the repair on a string.
//'
//' @param str the input string.
//' @useDynLib jmotif
//' @export
//' @references  N.J. Larsson and A. Moffat. Offline dictionary-based compression.
//' In Data Compression Conference, 1999.
//' @examples
//' str_to_repair_grammar("abc abc cba cba bac xxx abc abc cba cba bac")
// [[Rcpp::export]]
Rcpp::List str_to_repair_grammar(CharacterVector str){

  // convert the string
  std::string s = Rcpp::as<std::string>(str);

  // run the c++ implementation
  std::map<int, RuleRecord> rules = _str_to_repair_grammar(s);

  // make results
  Rcpp::List res(rules.size());
  for(std::map<int, RuleRecord>::iterator it = rules.begin(); it != rules.end(); ++it) {

    std::stringstream ss;
    ss << it->second.rule_id;
    Rcpp::CharacterVector rule_name = "R" + ss.str();

    Rcpp::CharacterVector rule_string = it->second.rule_string;
    Rcpp::CharacterVector expanded_rule_string = it->second.expanded_rule_string;
    Rcpp::NumericVector rule_interval_starts = Rcpp::wrap(it->second.rule_occurrences);
    Rcpp::NumericVector rule_interval_ends = rule_interval_starts +
      _count_spaces(&it->second.expanded_rule_string);

    res[it->first] = List::create(
      _["rule_name"]  = rule_name,
      _["rule_string"]  = rule_string,
      _["expanded_rule_string"] = expanded_rule_string,
      _["rule_interval_starts"] = rule_interval_starts,
      _["rule_interval_ends"] = rule_interval_ends
    ) ;
  }

  return res;

}
// library(jmotif); str_to_repair_grammar("abc abc cba cba bac xxx abc abc cba cba bac")

