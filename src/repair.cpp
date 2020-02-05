#include <jmotif.h>
using namespace Rcpp;
//
#include <iomanip>
#include <chrono>
#include <ctime>

#include <string>
#include <iostream>
#include <sstream>

std::unordered_map<int, rule_record*> _str_to_repair_grammar(std::string s) {
  // Rcout<<s<<"\n";

  // define the objects we are working with
  std::string delimiter = " ";
  s.append(delimiter);

  int str_length = _count_spaces(&s);

  // std::clock_t c_start0 = std::clock();
  // Rcout << "input string of " << str_length << " tokens \n";

  // the grammar keeper
  std::map<int, repair_rule*> grammar;

  // the input string as a vector...
  // this will get transformed into the final R0 eventually
  std::vector<repair_symbol_record*> r0; // this is the R0 tokens sequence
  r0.reserve(str_length);

  // the digrams table
  std::unordered_map<std::string, std::vector<int>> digram_table;

  // tokenizer variables and counters
  std::stringstream ss_input(s);
  std::string old_token;
  std::string token;
  int token_counter = 0;
  while (std::getline(ss_input, token, ' ')) {

    // Rcout << "current token: " << token << std::endl;

    // create the symbol
    repair_symbol* rs = new repair_symbol( token, token_counter );
    // wrap the symbol into the record
    repair_symbol_record* rec = new repair_symbol_record( rs );
    r0.emplace_back( rec ); // and place into the work string

    if(token_counter > 0){ // if symbol has been already added, work out digrams

      // create the digram
      std::string digram_str = old_token + " " + token;
      // Rcout << " . new digram: " << d_str << std::endl;

      // save the digram occurrence
      if (digram_table.find(digram_str) == digram_table.end()){
        std::vector<int> occurrences;
        occurrences.push_back(token_counter - 1);
        digram_table.insert(std::make_pair(digram_str, occurrences));
      }else{
        digram_table[digram_str].push_back(token_counter - 1);
      }

      r0[token_counter - 1]->next = rec;
      rec->prev = r0[token_counter - 1];
    } // if digram needs to be created

    old_token = token;
    token_counter++;
  }

  // DEBUG:: walk over the R0
  // Rcout << "\nthe R0: ";
  // repair_symbol_record* ptr = r0[0];
  // do {
  //    Rcout << ptr->payload->payload << " ";
  //    ptr = ptr->next;
  //  } while(ptr != nullptr);
  //  Rcout << std::endl;

  // DEBUG:: all digrams are accounted for... print their state
  // Rcout << "\nthe digrams table\n=================" << std::endl;
  // for(std::unordered_map<std::string, std::vector<int>>::iterator it = digram_table.begin();
  //     it != digram_table.end(); ++it) {
  //   Rcout << it->first << " [";
  //   for (auto i = it->second.begin(); i != it->second.end(); ++i)
  //     Rcout << *i << ", ";
  //   Rcout << "]" << std::endl;
  // }

  // populate the priority queue and the index -> digram record map
  repair_priority_queue digram_queue;
  for(std::unordered_map<std::string, std::vector<int>>::iterator it = digram_table.begin();
      it != digram_table.end(); ++it) {
    if(it->second.size() > 1) {
      repair_digram* digram = new repair_digram( it->first, it->second.size() );
      digram_queue.enqueue(digram);
    }
  }

  // DEBUG:: all digrams are pushed to the queue, see those
  // Rcout << "\nthe digrams queue\n=================" << std::endl;
  // Rcout << digram_queue.to_string() << std::endl;


  /// the main loop begins...
  //  1.0 pop the digram from the queue
  //  2.0 process it by creating a rule and substituting occurrences
  //  3.0. go to #1
  //
  int i=0;
  repair_digram* entry = digram_queue.dequeue();
  while (entry != nullptr) {

    // first, get all the digram's occurrences
    std::vector<int> occurrences;
    occurrences.reserve(entry->freq);

    // the digram entry from the master table
    std::unordered_map<std::string, std::vector<int>>::iterator it =
      digram_table.find(entry->digram);

    // DEBUG:: a digram info message
    // Rcout <<"\n *** popped a digram: " << it->first << " [";
    for (auto i = it->second.begin(); i != it->second.end(); ++i) {
    //    Rcout << *i << ", ";
      occurrences.push_back(*i);
    }
    //  Rcout << "]" << std::endl;

    // work on the NEW RULE construction
    repair_symbol_record* first_symbol = r0[it->second[0]];
    repair_symbol_record* second_symbol = first_symbol->next; // make sure we don't use an index
    // Rcout << "   * the very first digram instance " << std::endl;
    // Rcout << "   * " << first_symbol->payload->payload << " @" << first_symbol->payload->str_index;
    // Rcout << "   * " << second_symbol->payload->payload << " @" << second_symbol->payload->str_index <<
    // std::endl;

    repair_rule* r = new repair_rule();
    r->id = grammar.size() + 1;
    r->first = first_symbol->payload; // first string
    r->second = second_symbol->payload; // second string

    // figure out the rule's expanded string
    std::stringstream ss;
    if(r->first->is_guard()){ // in case the symbol is a guard
      repair_guard* ptr = static_cast<repair_guard*>( r->first );
      ss << ptr->get_expanded_string();
      // note the use
      // ptr->r->rule_use++;
    } else {
      repair_symbol* ptr = r->first;
      ss << ptr->to_string();
    }
    ss << " ";
    if(r->second->is_guard()){ // in case the symbol is a guard
      repair_guard* ptr = static_cast<repair_guard*>( r->second );
      ss << ptr->get_expanded_string();
      // note the use
      // ptr->r->rule_use++;
    } else {
      repair_symbol* ptr = r->second;
      ss << ptr->to_string();
    }
    r->expanded_rule_string = ss.str();
    grammar.insert(std::pair<int, repair_rule*>(r->id, r)); // voila

    // DEBUG:: the new rule info
    // Rcout << " *** created the rule: " << r->get_rule_string() << " -> "
    //       << r->expanded_rule_string << std::endl;

    // we keep track of all newly created digrams
    std::unordered_set<std::string> new_digrams;

    // Rcout << " *** starting occurrences loop ..." << std::endl;

    // digram occurrences processing, iterating over each position...
    while(!occurrences.empty()){

      // secure the current index
      int occ = occurrences[occurrences.size()-1];
      occurrences.pop_back();

      // Rcout << "    ** processing the digram occurrence at " << occ << std::endl;

      // save the positions we work with
      repair_symbol_record* curr_sym = r0[occ];
      repair_symbol_record* next_sym = curr_sym->next;
      repair_symbol* old_first = curr_sym->payload;
      // Rcout << "    ** sym1: "
      //       << curr_sym->payload->to_string() << " @" << curr_sym->payload->str_index
      //       << " sym2: "
      //       << next_sym->payload->to_string() << " @" << next_sym->payload->str_index
      //       << std::endl;

      // make up a guard for the rule
      repair_guard* guard = new repair_guard(r, occ);
      r->occurrences.push_back(occ);

      // alter the R0 by placing the guard...
      curr_sym->payload = guard;
      // Rcout << "    ** placed guard at " << occ << ": " << guard->to_string() << std::endl;

      // and fixing the OLE NEXT symbol link
      repair_symbol_record* next_not_null = next_sym->next;
      curr_sym->next = next_not_null;
      if( nullptr != next_not_null ){
        next_not_null->prev = curr_sym;
        // Rcout << "    ** next not null at " << next_not_null->payload->str_index << ", "
        //       << next_not_null->payload->to_string() << std::endl;
      }
      // else {
      //   Rcout << "    ** next is NULL" << std::endl;
      // }

      // and fixing the OLE prev symbol link
      repair_symbol_record* prev_not_null = curr_sym->prev;
      curr_sym->prev = prev_not_null;
      if(nullptr!=prev_not_null){
        prev_not_null->next = curr_sym;
//         Rcout << "   ** prev not null at " << prev_not_null->payload->str_index << ": "
//               << prev_not_null->payload->to_string() << std::endl;
      }
//       else {
//         Rcout << "   ** prev is NULL" << std::endl;
//       }

      // ### now need to fix the OLE left digram
      // ###
      if(occ > 0 && nullptr!=prev_not_null){
        // Rcout << " *** fixing old left digram: ";

        std::string ole_left_digram = prev_not_null->payload->to_string() + " "
        + old_first->to_string();
        // Rcout << ole_left_digram << std::endl;

        std::unordered_map<std::string, std::vector<int>>::iterator it =
          digram_table.find(ole_left_digram);

        // Rcout << "  *** old occurrences: ";
//         for (auto i = it->second.begin(); i != it->second.end(); ++i)
//           Rcout << *i << ' ';
//         Rcout << std::endl;

        // clean up the specific index in the occurrences array
        it->second.erase(std::remove(it->second.begin(), it->second.end(),
                   prev_not_null->payload->str_index), it->second.end());

        // Rcout << "  *** new occurrences: ";
//         for (auto i = it->second.begin(); i != it->second.end(); ++i)
//           Rcout << *i << ' ';
//         Rcout << std::endl;

        int new_freq = it->second.size();

        // take a look if the digram is actually the one we work with...
        if (0 == ole_left_digram.compare(entry->digram)) {
//           Rcout << "  ***** the old digram is like the new one, cleaning up ..." << std::endl;
//           Rcout << "  *** old ext. loop: ";
//           for (auto i = occurrences.begin(); i != occurrences.end(); ++i) Rcout << *i << ' ';
//           Rcout << std::endl;
          occurrences.erase(std::remove(occurrences.begin(), occurrences.end(),
                       prev_not_null->payload->str_index), occurrences.end());
          // Rcout << "  *** new ext. loop: ";
//           for (auto i = occurrences.begin(); i != occurrences.end(); ++i) Rcout << *i << ' ';
//           Rcout << std::endl;
        }
        // Rcout << "updating the digram freq to " << new_freq;
        digram_queue.update_digram_frequency(&ole_left_digram, new_freq);

        // if it was the last entry...
        if (0 == new_freq) {
          // Rcout << "  *** since new freq is 0, cleaning up ..." << std::endl;
          digram_table.erase(ole_left_digram);
          new_digrams.erase(ole_left_digram);
        }

        // and place the new digram entry
        std::string new_left_digram = prev_not_null->payload->to_string() + " " + r->get_rule_string();
        // Rcout << "  *** the new left digram shall be : " << new_left_digram << std::endl;
        // see the new freq..
        // save the digram occurrence
        if (digram_table.find(new_left_digram) == digram_table.end()){
          std::vector<int> occurrences;
          occurrences.push_back(prev_not_null->payload->str_index);
          digram_table.insert(std::make_pair(new_left_digram, occurrences));
        }else{
          digram_table[new_left_digram].push_back(prev_not_null->payload->str_index);
        }

        new_digrams.emplace(new_left_digram);
      }

      // walk over the R0
      // Rcout << "\nthe R0: ";
      // ptr = r0[0];
      // do {
      //   Rcout << ptr->payload->payload << " ";
      //   ptr = ptr->next;
      // } while(ptr != nullptr);
      // Rcout << std::endl;
      // Rcout << "\nthe digrams table\n=================" << std::endl;
      // for(std::unordered_map<std::string, std::vector<int>>::iterator it = digram_table.begin();
      //     it != digram_table.end(); ++it) {
      //   Rcout << it->first << " [";
      //   for (auto i = it->second.begin(); i != it->second.end(); ++i)
      //     Rcout << *i << ", ";
      //   Rcout << "]" << std::endl;
      // }
      // all digrams are pushed to the queue, see those
      // Rcout << "\nthe digrams queue\n=================" << std::endl;
      // Rcout << digram_queue.to_string() << std::endl;

      // ### now need to fix the OLE RIGHT digram
      // ###
      if(occ < (int) r0.size() - 2 && nullptr != next_not_null){

        // Rcout << " *** fixing old right digram: ";

        std::string ole_right_digram = next_sym->payload->to_string() + " "
        + next_not_null->payload->to_string();
        // Rcout << ole_right_digram << std::endl;

        // Rcout << "  **++** after second: " << next_not_null->payload->to_string() ;
        // Rcout << std::endl;

        std::unordered_map<std::string, std::vector<int>>::iterator it =
          digram_table.find(ole_right_digram);

        int new_freq = it->second.size() - 1;
        // Rcout << "  *** old occurrences: ";
        // for (auto i = it->second.begin(); i != it->second.end(); ++i)
        //   Rcout << *i << ' ';
        // Rcout << std::endl;

        // clean up the specific index in the occurrences array
        // Rcout << " **++* erasing  " << next_sym->payload->to_string() << " @" <<
        //   next_sym->payload->str_index << std::endl;
        it->second.erase(std::remove(it->second.begin(), it->second.end(),
                      next_sym->payload->str_index), it->second.end());
        // Rcout << "  *** new occurrences: ";
        // for (auto i = it->second.begin(); i != it->second.end(); ++i)
        //   Rcout << *i << ' ';
        // Rcout << std::endl;

        // take a look if the digram is actually the one we work with...
        if (0 == ole_right_digram.compare(entry->digram)) {
        // Rcout << "  ***** the old digram is like the new one, cleaning up ..." << std::endl;
        // Rcout << "  *** old ext. loop: ";
        // for (auto i = occurrences.begin(); i != occurrences.end(); ++i) Rcout << *i << ' ';
        // Rcout << std::endl;
          occurrences.erase(std::remove(occurrences.begin(), occurrences.end(),
                       next_sym->payload->str_index), occurrences.end());
        // Rcout << "  *** new ext. loop: ";
        // for (auto i = occurrences.begin(); i != occurrences.end(); ++i) Rcout << *i << ' ';
        // Rcout << std::endl;
        }
        digram_queue.update_digram_frequency(&ole_right_digram, new_freq);

        // if it was the last entry...
        if (0 == new_freq) {
          // Rcout << "  *** since new freq is 0, cleaning up ..." << std::endl;
          digram_table.erase(ole_right_digram);
          new_digrams.erase(ole_right_digram);
        }

        // and place the new digram entry
        std::string new_right_digram = r->get_rule_string() + " " + next_not_null->payload->to_string();
        // Rcout << "  *** the new right digram shall be : " << new_right_digram << std::endl;
        // see the new freq..
        // save the digram occurrence
        if (digram_table.find(new_right_digram) == digram_table.end()){
          std::vector<int> occurrences;
          occurrences.push_back(curr_sym->payload->str_index);
          digram_table.insert(std::make_pair(new_right_digram, occurrences));
        }else{
          digram_table[new_right_digram].push_back(curr_sym->payload->str_index);
        }

        new_digrams.emplace(new_right_digram);

      }

    }
    // clean up the digram we have worked with ...
    //
    digram_table.erase(entry->digram);

    // update the priority queue with new digrams ...
    //
    // Rcout << "  *** new digrams size " << new_digrams.size() << " : ";
    // for(std::string s : new_digrams)
    //   Rcout << s << ' ';
    // Rcout << std::endl;

    for (std::string st : new_digrams) {
      // Rcout << "  *** checking on new digram " << st  << std::endl;
      // if (digram_table.find(st) == digram_table.end()){
      //   Rcout << "  ***XXX new digram " << st  << " not in da table " << std::endl;
      // }
      if(digram_table[st].size() > 1){
        // Rcout << "  *** digrams table has an entry for " << st  << std::endl;
        if(digram_queue.contains_digram(&st)){
          // Rcout << "      gotta update the queue entry ... " << std::endl;
          digram_queue.update_digram_frequency(&st, digram_table[st].size());
        } else {
          // Rcout << "      enqueueing a digram ... " << st << ":" << digram_table[st].size() << std::endl;
          repair_digram* digram = new repair_digram( st, digram_table[st].size() );
          digram_queue.enqueue(digram);
        }
      }
    }

    // walk over the R0
//     Rcout << "\n*** XXXXXX end of the loop ****\nR0: ";
//     repair_symbol_record* ptr = r0[0];
//     do {
//       Rcout << ptr->payload->payload << " ";
//       ptr = ptr->next;
//     } while(ptr != nullptr);
//     Rcout << std::endl;
//     Rcout << "\nthe digrams table\n=================" << std::endl;
//     for(std::unordered_map<std::string, std::vector<int>>::iterator it = digram_table.begin();
//         it != digram_table.end(); ++it) {
//       Rcout << it->first << " [";
//       for (auto i = it->second.begin(); i != it->second.end(); ++i)
//         Rcout << *i << ", ";
//       Rcout << "]" << std::endl;
//     }
    // all digrams are pushed to the queue, see those
//     Rcout << "\nthe digrams queue\n=================" << std::endl;
//     Rcout << digram_queue.to_string() << std::endl;

    entry = digram_queue.dequeue();
    i++;

  }

  // make up the R0
  std::stringstream ss;
  // repair_symbol_record*
  repair_symbol_record* ptr = r0[0];
  while(ptr != nullptr) {
    ss << ptr->payload->payload;
    ptr = ptr->next;
    if(ptr != nullptr){  ss << " "; }
  };

//   Rcout << "\n\nthe R0\n=================\n";
//   Rcout << ss.str() << std::endl;
//
//   Rcout << "\nthe digrams table\n=================" << std::endl;
//   for(std::unordered_map<std::string, std::vector<int>>::iterator it = digram_table.begin();
//       it != digram_table.end(); ++it) {
//     Rcout << it->first << " [";
//     for (auto i = it->second.begin(); i != it->second.end(); ++i)
//       Rcout << *i << ", ";
//     Rcout << "]" << std::endl;
//   }

  // all digrams are pushed to the queue, see those
  // Rcout << "\nthe digrams queue\n=================" << std::endl;
  // Rcout << digram_queue.to_string() << std::endl;

//   Rcout << "\nthe Grammar\n=================" << std::endl;
//   for(std::map<int, repair_rule*>::iterator it = grammar.begin();
//       it != grammar.end(); ++it) {
//     Rcout << it->second->get_rule_string() << " : "
//           << it->second->get_rule_string() << " : "
//           << it->second->expanded_rule_string << " [";
//     for (auto i = it->second->occurrences.begin(); i != it->second->occurrences.end(); ++i)
//       Rcout << *i << ", ";
//     Rcout << "]" << std::endl;
//   }

  // make results
  std::unordered_map<int, rule_record*> res;

  // the R0
  rule_record* rec0 = new rule_record();
  rec0->rule_id = 0;
  rec0->rule_string = ss.str();
  rec0->expanded_rule_string = s;
  rec0->rule_occurrences.push_back(-1);
  rec0->rule_intervals.push_back(std::make_pair(0, str_length));
  res.emplace(std::make_pair(0, rec0));

  // the rest of the grammar
  for(std::map<int, repair_rule*>::iterator it = grammar.begin();
      it != grammar.end(); ++it) {
    rule_record* rr = new rule_record();
    rr->rule_id = it->first;
    rr->rule_string = it->second->get_rule_string();
    rr->expanded_rule_string = it->second->expanded_rule_string;
    rr->rule_occurrences = it->second->occurrences;
    for(std::vector<int>::iterator oi = rr->rule_occurrences.begin();
        oi != rr->rule_occurrences.end(); ++oi) {
      rr->rule_intervals.push_back(std::make_pair(*oi,
          *oi + _count_spaces(&rr->expanded_rule_string)));
    }
    res.emplace(std::make_pair(it->first, rr));
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
  std::unordered_map<int, rule_record*> rules = _str_to_repair_grammar(s);

  // make results
  Rcpp::List res(rules.size());

  for(unsigned i = 0; i < rules.size(); ++i) {

    std::unordered_map<int, rule_record*>::iterator it = rules.find(i);

    std::stringstream ss;
    ss << it->second->rule_id;
    Rcpp::CharacterVector rule_name = "R" + ss.str();

    Rcpp::CharacterVector rule_string = it->second->rule_string;
    Rcpp::CharacterVector expanded_rule_string = it->second->expanded_rule_string;
    Rcpp::NumericVector rule_interval_starts = Rcpp::wrap(it->second->rule_occurrences);
    Rcpp::NumericVector rule_interval_ends(rule_interval_starts.length());
    int spaces_count = _count_spaces(&it->second->expanded_rule_string);
    // Rcout << "spaces: " <<spaces_count <<"\n";
    for(int j=0; j<rule_interval_starts.length(); ++j) {
     rule_interval_ends[j] = rule_interval_starts[j] + spaces_count;
    }

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
