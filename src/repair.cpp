#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
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
//
// class Digram: public Token {
// public:
//   Token* first;
//   Token* second;
//   Digram(){
//   };
//   Digram(Token* p, Token* n){
//     first = p;
//     second = n;
//   };
//   std::string getPayload(){
//     return first->payload + " " + second->payload;
//   }
// };
// std::ostream& operator<<(std::ostream &strm, const Digram &d) {
//   return strm << "D(" << d.first->payload << " "
//               << d.second->payload << ")";
// };

class Rule {
public:
  int id;
  std::string rule_string;
  std::string expanded_rule_string;
  Rule(int r_id, std::string rule_str, std::string expanded_rule_str){
    id = r_id;
    rule_string = rule_str;
    expanded_rule_string = expanded_rule_str;
  };
  std::string ruleString(){
    std::stringstream ss;
    ss << id;
    return "R" + ss.str();
  }
};

class Grammar {
public:
  std::map<int, Rule> rules;
  Grammar(){
    std::map<int, Rule> rules;
  }
};

struct sort_pred {
  bool operator()(const std::pair<std::string,int> &left,
                const std::pair<std::string,int> &right) {
    return left.second < right.second;
  }
};
//  str_to_repair_grammar("abc abc cba cba bac xxx abc abc cba cba bac")

//' Runs the repair.
//'
//' @param str the input string.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
CharacterVector str_to_repair_grammar(CharacterVector str){
  Rcout << "input string \'" << str << "\'\n making a digram table\n";

  // define the objects we ar work with
  std::string s = Rcpp::as<std::string>(str);
  std::string delimiter = " ";

  // digrams array shall be built first it is digram -> indexes
  std::vector< Token > R0; // this is the R0 tokens sequence
  std::map<int, Rule > rules; // the grammar' rule dictionary
  std::map<std::string, int> digrams_count; // digram counts
  std::vector< std::pair<std::string, int> > digrams_vec; // digram - count pairs

  // working vars
  std::string old_token;
  std::string token;
  int token_counter = 0;

  int pos = 0; // the tokenizer var
  while ((pos = s.find(delimiter)) != std::string::npos) {

    token = s.substr(0, pos);
    Rcout << "current token: " << token << std::endl;
    R0.push_back( Token(token , pos) );

    if (!old_token.empty()) {
      Token p(old_token, token_counter - 1);
      Token n(token, token_counter);
      std::string d_str = p.payload + " " + n.payload;
      Rcout << "new digram: " << d_str << std::endl;
      if (digrams_count.find(d_str) == digrams_count.end()){
        digrams_count.insert(std::make_pair(d_str, 1));
      }else{
        ++digrams_count[d_str];
      }

    }
    s.erase(0, pos + delimiter.length());
    old_token = token;
    token_counter++;
  }

  // all digrams are accounted for, print the state
  Rcout << "\n\nthe digrams table\n=================\n" << std::endl;
  for(std::map<std::string, int>::iterator it = digrams_count.begin();
      it != digrams_count.end(); ++it) {
    Rcout << it->first << " " << it->second << std::endl;
    digrams_vec.push_back(std::make_pair(it->first, it->second) );
  }

  std::sort(digrams_vec.begin(), digrams_vec.end(), sort_pred());

  Rcout << "\n\nsorted digrams:\n=================\n" << std::endl;
  for (std::vector<std::pair<std::string,int> >::iterator it = digrams_vec.begin();
       it != digrams_vec.end(); ++it)
    Rcout << it->first << " " << it->second << std::endl;
  Rcout << '\n';

  Rcout << "\n\nstarting Re-Pair onthe string:\n=================\n" << std::endl;
  Rcout << "R0 -> ";
  for (std::vector<Token>::iterator it = R0.begin();
       it != R0.end(); ++it) Rcout << it->payload << " ";
  Rcout << std::endl;

  std::pair<std::string,int> top_digram = digrams_vec.back();
  while(top_digram.second > 1){

    Rcout << "working with " << top_digram.first << " of freq " << top_digram.second << std::endl;

    // remove this digram
    std::string digram_str = top_digram.first;
    int space_idx = digram_str.find_first_of(" ", 0);
    std::string p = digram_str.substr(0, space_idx);
    std::string n = digram_str.substr(space_idx+1, digram_str.size());
    int digram_freq = top_digram.second;
    digrams_vec.pop_back(); // **************** removes the token from table *********

    // create the new rule
    int rule_id = rules.size();
    Rule r(rule_id, digram_str, "\0");

    // do a pass over the R0
    int start = 0;
    int end = R0.size();
    int cp = start + 1;
    while(cp <= end){

      if(R0[cp-1].payload == p && R0[cp].payload == n) {

        Rcout << "hit @ " << cp-1 << "\n";

        // update the first digram's token with the rule [cp-1]
        R0[cp-1] = Token(r.ruleString(), cp-1);
        // and remove its second token
        R0.erase(R0.begin() + cp); // ******* removes the token at [cp] *******

        // update limits
        end = end - 1;

        // update digrams vec
        Rcout << "  decr. freq of " << digram_str << " : " << digrams_count[digram_str] << " to ";
        digrams_count[digram_str]--;
        Rcout << digrams_count[digram_str] << std::endl;

        // add or update other elements in digram vector
        if(cp-1 > 0) {
          std::string left_str = R0[cp-2].payload + " " + R0[cp-1].payload;
          if (digrams_count.find(left_str) == digrams_count.end()){
            digrams_count.insert(std::make_pair(left_str, 1));
            Rcout << "  added a digram to table:" << left_str << "1" << std::endl;
          }else{
            digrams_count[left_str]++;
            Rcout << "  incr. count for a digram in table:" << left_str << digrams_count[left_str] << std::endl;
          }
        }
        if(cp <= end) {
          std::string right_str = R0[cp-1].payload + " " + R0[cp].payload;
          if (digrams_count.find(right_str) == digrams_count.end()){
            digrams_count.insert(std::make_pair(right_str, 1));
            Rcout << "  added a digram:" << right_str << "1" << std::endl;
          }else{
            digrams_count[right_str]++;
            Rcout << "  incr. count for a digram:" << right_str << digrams_count[right_str] << std::endl;
          }
        }

        // remove or update other digrams
        std::string old_left =  R0[cp-2].payload + " " + p;
        digrams_count[old_left]--;
        Rcout << "  decr count for a digram:" << old_left << ", " << digrams_count[old_left] << std::endl;
        if(0 == digrams_count[old_left]){
          digrams_count.erase(old_left);
          Rcout << "    removed a digram:" << old_left << std::endl;
        }

        std::string old_right =  n + " " + R0[cp].payload;
        digrams_count[old_right]--;
        Rcout << "  decr count for a digram:" << old_right << ", " << digrams_count[old_right] << std::endl;
        if(0 == digrams_count[old_right]){
          digrams_count.erase(old_right);
          Rcout << "    removed a digram:" << old_right << std::endl;
        }

        // need to reshuffle frequencies
        std::vector<int> elements_to_delete;
        for (int i = 0; i< digrams_vec.size(); ++i){
          std::map<std::string, int>::iterator im =  digrams_count.find(digrams_vec[i].first);
          if(digrams_count.end() == im){
            Rcout << "  a digram " << digrams_vec[i].first << " is in the vec but not map, cleaning el at " << i << std::endl;
            elements_to_delete.push_back(i);
          } else {
            Rcout << "  updating vector counts for a digram " << digrams_vec[i].first << std::endl;
            digrams_vec[i].second = im->second;
          }
        }

        for (int i=elements_to_delete.size()-1; i>=0; --i){
          Rcout << "   deleting vector element at " << elements_to_delete[i] << "\n";
          digrams_vec.erase(digrams_vec.begin() + elements_to_delete[i]);
        }

        // if this was the last instance
        if(digrams_count[digram_str]==0){
          std::map<std::string, int>::iterator im =  digrams_count.find(digram_str);
          digrams_count.erase(digram_str);
          break;
        }

      } else {
        cp++;
      }

    } // while cp <= end

    // finalize
    std::sort(digrams_vec.begin(), digrams_vec.end(), sort_pred());
    top_digram = digrams_vec.back();

    Rcout << "\n\nthe digrams table\n=================\n" << std::endl;
    for(std::map<std::string, int>::iterator it = digrams_count.begin();
        it != digrams_count.end(); ++it) {
      Rcout << it->first << " " << it->second << std::endl;
      digrams_vec.push_back(std::make_pair(it->first, it->second) );
    }

    std::sort(digrams_vec.begin(), digrams_vec.end(), sort_pred());

    Rcout << "\n\nsorted digrams:\n=================\n" << std::endl;
    for (std::vector<std::pair<std::string,int> >::iterator it = digrams_vec.begin();
         it != digrams_vec.end(); ++it)
      Rcout << it->first << " " << it->second << std::endl;
    Rcout << '\n';

    Rcout << "R0 -> ";
    for (std::vector<Token>::iterator it = R0.begin();
         it != R0.end(); ++it)
      Rcout << it->payload << " ";
    Rcout << std::endl;

    return str;

  }

  return str;
}

// str_to_repair_grammar("abc abc cba cba bac xxx abc abc cba cba bac")
