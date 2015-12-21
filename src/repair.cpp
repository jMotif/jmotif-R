#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>

// Tokens are used in the R0
//
class Token {
public:
  int str_idx;
  std::string payload;
  Token(){
    str_idx = -1;
  };
  Token(std::string s, int idx){
    payload = s;
    str_idx = idx;
  };
};
  std::ostream& operator<<(std::ostream &strm, const Token &t) {
    return strm << "T(" << t.payload << "@" << t.str_idx << ")";
};

// Rules build up the rules table, i.e. the grammar
//
class Rule {
public:
  int id;
  std::string rule_string;
  std::string expanded_rule_string;
  Rule(){
    id = -1;
    rule_string = "\0";
    expanded_rule_string = "\0";
  };
  Rule(int r_id, std::string rule_str, std::string expanded_rule_str){
    id = r_id;
    rule_string = rule_str;
    expanded_rule_string = expanded_rule_str;
  };
  std::string ruleString(){
    std::stringstream ss;
    ss << id;
    return "R" + ss.str();
  };
};
std::ostream& operator<<(std::ostream &strm, const Rule &d) {
    return strm << "R" << d.id << "\t" << d.rule_string << "\t" << d.expanded_rule_string;
};

// this is the digram priority queue sorter
//
struct sort_pred {
  bool operator()(const std::pair<std::string,int> &left,
                const std::pair<std::string,int> &right) {
    return left.second < right.second;
  }
};

//' Runs the repair.
//'
//' @param str the input string.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
CharacterVector str_to_repair_grammar(CharacterVector str){
  Rcout << "input string \'" << str << "\'\n making a digram table...\n";

  // define the objects we ar work with
  std::string s = Rcpp::as<std::string>(str);
  std::string delimiter = " ";
  s.append(delimiter);

  // digrams array shall be built first it is digram -> indexes
  std::vector<Token> R0; // this is the R0 tokens sequence
  std::map<int, Rule> rules; // the grammar' rule dictionary
  rules.insert(std::make_pair(0, Rule(0, "\0", "\0")));
  std::map<std::string, int> digram_table; // digram counts
  std::vector< std::pair<std::string, int> > digrams_vec; // digram - count pairs

  // working vars
  std::string old_token;
  std::string token;
  int token_counter = 0;

  int pos = 0; // the tokenizer var
  while ((pos = s.find(delimiter)) != std::string::npos) {

    token = s.substr(0, pos);
    Rcout << "current token: " << token << std::endl;
    R0.push_back( Token(token, pos) );

    if (!old_token.empty()) {
      std::string d_str = old_token + " " + token;
      Rcout << "new digram: " << d_str << std::endl;
      if (digram_table.find(d_str) == digram_table.end()){
        digram_table.insert(std::make_pair(d_str, 1));
      }else{
        digram_table[d_str]++;
      }
    }

    s.erase(0, pos + delimiter.length());
    old_token = token;
    token_counter++;
  }

  // all digrams are accounted for, print the state and make a frequencies vector
  Rcout << "\n\nthe digrams table\n=================" << std::endl;
  for(std::map<std::string, int>::iterator it = digram_table.begin();
      it != digram_table.end(); ++it) {
    Rcout << it->first << " " << it->second << std::endl;
    digrams_vec.push_back(std::make_pair(it->first, it->second) ); // frequencies vec new element
  }

  // sort the frequencies vector
  std::sort(digrams_vec.begin(), digrams_vec.end(), sort_pred());

  Rcout << "\n\nsorted vec:\n=================" << std::endl;
  for (std::vector<std::pair<std::string,int> >::iterator it = digrams_vec.begin();
       it != digrams_vec.end(); ++it) Rcout << it->first << " " << it->second << std::endl;
  Rcout << '\n';

  Rcout << "\n\nstarting Re-Pair on the string:\n=================" << std::endl;
  Rcout << "R0 -> ";
  for (std::vector<Token>::iterator it = R0.begin();
       it != R0.end(); ++it) Rcout << it->payload << " ";
  Rcout << std::endl;

  std::pair<std::string,int> top_digram = digrams_vec.back();
  while(top_digram.second > 1){

    Rcout << "working with " << top_digram.first << " of freq " << top_digram.second << std::endl;

    // remove this digram
    std::string digram_str = top_digram.first;
    int digram_freq = top_digram.second;
    int space_idx = digram_str.find_first_of(" ", 0);
    std::string p = digram_str.substr(0, space_idx);
    std::string n = digram_str.substr(space_idx+1, digram_str.size());
    digrams_vec.pop_back(); // **************** removes the digram token from the table *********

    // create the new rule
    int rule_id = rules.size();
    Rule r(rule_id, digram_str, "\0");
    rules.insert(std::make_pair(r.id, r));

    // do a pass over the R0 substituting the digram string with R#
    int start = 0;
    int end = R0.size() - 1;
    int cp = start + 1;
    while(cp <= end){

      if(R0[cp-1].payload == p && R0[cp].payload == n) { // if the digram has been found

        Rcout << "hit @ " << cp-1 << "\n";

        // update the first digram's token with the rule [cp-1]
        R0[cp-1] = Token(r.ruleString(), cp-1);
        // and remove its second token
        R0.erase(R0.begin() + cp); // ******* removes the token at [cp] *******
        // update limits
        end = end - 1;

        // update digrams count table
        Rcout << "  * decr. count of " << digram_str << ": " << digram_table[digram_str] << " to ";
        digram_table[digram_str]--;
        Rcout << digram_table[digram_str] << std::endl;

        // if not at the very beginning
        if(cp-1 > 0) {

          // update the NEW left digram
          std::string left_str = R0[cp-2].payload + " " + R0[cp-1].payload;
          if (digram_table.find(left_str) == digram_table.end()){
            digram_table.insert(std::make_pair(left_str, 1));
            Rcout << "  added a digram: " << left_str << ":1" << std::endl;
          }else{
            Rcout << "  incr. count for a digram " << left_str << " from " << digram_table[left_str];
            digram_table[left_str]++;
            Rcout << " to " << digram_table[left_str] << std::endl;
          }

          // update the OLD left digram
          std::string old_left =  R0[cp-2].payload + " " + p;
          Rcout << "  decr. count for a digram " << old_left << " from " << digram_table[old_left];
          digram_table[old_left]--;
          Rcout << " to " << digram_table[old_left] << std::endl;
          if(0 == digram_table[old_left]){
            Rcout << "  removing digram: " << old_left << std::endl;
            digram_table.erase(old_left);
          }

        }

        // if not at the very end
        if(cp <= end) {

          // update the NEW right digram
          std::string right_str = R0[cp-1].payload + " " + R0[cp].payload;
          if (digram_table.find(right_str) == digram_table.end()){
            digram_table.insert(std::make_pair(right_str, 1));
            Rcout << "  added a digram: " << right_str << ":1" << std::endl;
          }else{
            Rcout << "  incr. count for a digram " << right_str << " from " << digram_table[right_str];
            digram_table[right_str]++;
            Rcout << " to " << digram_table[right_str] << std::endl;
          }

          // update the OLD right digram
          std::string old_right =  n + " " + R0[cp].payload;
          Rcout << "  decr. count for a digram " << old_right << " from " << digram_table[old_right];
          digram_table[old_right]--;
          Rcout << " to " << digram_table[old_right] << std::endl;
          if(0 == digram_table[old_right]){
            Rcout << "  removing digram: " << old_right << std::endl;
            digram_table.erase(old_right);
          }

        }

        // need to reshuffle frequencies
        Rcout << "\n\nvec before corrections:\n=================" << std::endl;
        for (std::vector<std::pair<std::string,int> >::iterator it = digrams_vec.begin();
             it != digrams_vec.end(); ++it)
          Rcout << it->first << " " << it->second << std::endl;
        Rcout << '\n';

//         std::vector<int> elements_to_delete;
//         for (int i = 0; i< digrams_vec.size(); ++i){
//           std::map<std::string, int>::iterator im =  digram_table.find(digrams_vec[i].first);
//           if(digram_table.end() == im){
//             Rcout << "  a digram " << digrams_vec[i].first <<
//               " is in the vec but not map, cleaning elm at " << i << std::endl;
//             elements_to_delete.push_back(i);
//           } else {
//             Rcout << "  updating vector counts for a digram " << digrams_vec[i].first <<
//               " from " << digrams_vec[i].second << " to " << im->second << std::endl;
//             digrams_vec[i].second = im->second;
//           }
//         }
//
//         Rcout << "\n\nvec after corrections:\n=================" << std::endl;
//         for (std::vector<std::pair<std::string,int> >::iterator it = digrams_vec.begin();
//              it != digrams_vec.end(); ++it)
//           Rcout << it->first << " " << it->second << std::endl;
//         Rcout << '\n';
//
//         for (int i=elements_to_delete.size()-1; i>=0; --i){
//           Rcout << "   deleting vector element at " << elements_to_delete[i] << ", i.e. " <<
//             digrams_vec[elements_to_delete[i]].first << ":" << digrams_vec[elements_to_delete[i]].second <<
//               std::endl;
//           digrams_vec.erase(digrams_vec.begin() + elements_to_delete[i]);
//         }

        // all digrams are accounted for, print the state
        digrams_vec.clear();
        Rcout << "  vec size:" << digrams_vec.size() << std::endl;
        digrams_vec.reserve(digram_table.size());
        Rcout << "  vec size:" << digrams_vec.size() << std::endl;
        for(std::map<std::string, int>::iterator it = digram_table.begin();
            it != digram_table.end(); ++it) {
          digrams_vec.push_back(std::make_pair(it->first, it->second) );
        }

        Rcout << "\n\nvec after corrections:\n=================" << std::endl;
        for (std::vector<std::pair<std::string,int> >::iterator it = digrams_vec.begin();
             it != digrams_vec.end(); ++it)
          Rcout << it->first << " " << it->second << std::endl;
        Rcout << '\n';

        Rcout << "  *** working digram " << digram_str << ":" << digram_table[digram_str] << std::endl;

        // if this was the last instance
        if(digram_table[digram_str]==0){
          Rcout << "  breaking the digram substitute loop and erasing dd" << std::endl;
          std::map<std::string, int>::iterator im =  digram_table.find(digram_str);
          digram_table.erase(digram_str);

          digrams_vec.clear();
          digrams_vec.reserve(digram_table.size());
          for(std::map<std::string, int>::iterator it = digram_table.begin();
              it != digram_table.end(); ++it) {
            digrams_vec.push_back(std::make_pair(it->first, it->second) );
          }

          cp = end + 1;
        }

      } else {
        cp++;
      }

    } // while cp <= end

    Rcout << "\n\nthe digrams table\n=================" << std::endl;
    for(std::map<std::string, int>::iterator it = digram_table.begin();
        it != digram_table.end(); ++it) {
      Rcout << it->first << " " << it->second << std::endl;
    }

    Rcout << "\n\nsorted vec:\n=================" << std::endl;
    for (std::vector<std::pair<std::string,int> >::iterator it = digrams_vec.begin();
         it != digrams_vec.end(); ++it)
      Rcout << it->first << " " << it->second << std::endl;
    Rcout << '\n';

    Rcout << "R0 -> ";
    for (std::vector<Token>::iterator it = R0.begin();
         it != R0.end(); ++it)
      Rcout << it->payload << " ";
    Rcout << std::endl;

    // select the new digram to work with
    std::sort(digrams_vec.begin(), digrams_vec.end(), sort_pred());
    top_digram = digrams_vec.back();

  }

  std::string new_r0 = "";
  for (std::vector<Token>::iterator it = R0.begin();
       it != R0.end(); ++it) new_r0.append(it->payload + " ");
  new_r0.erase(new_r0.length()-1, new_r0.length());

  rules[0].rule_string = new_r0;
  Rcout << "\n\nthe rules table\n=================" << std::endl;
  for(std::map<int, Rule>::iterator it = rules.begin();
      it != rules.end(); ++it) {
    Rcout << it->second << std::endl;
  }

  return new_r0;
}

// library(jmotif); str_to_repair_grammar("abc abc cba cba bac xxx abc abc cba cba bac")
