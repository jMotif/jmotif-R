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
    payload = "\0";
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

class Digram: public Token {
public:
  Token* first;
  Token* second;
  Digram(){
  };
  Digram(Token* p, Token* n){
    first = p;
    second = n;
  };
};
std::ostream& operator<<(std::ostream &strm, const Digram &d) {
  return strm << "D(" << d.first->payload << " "
              << d.second->payload << ")";
};

class Rule {
public:
  int id;
  std::string rule_string;
  std::string expanded_rule_string;
};

class Grammar {
public:
  std::map<int, Rule> rules;
  Grammar(){
    std::map<int, Rule> rules;
  }
};

//str_to_repair_grammar("abc abc cba cba bac xxx abc abc cba cba bac")
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
CharacterVector str_to_repair_grammar(CharacterVector str){
  Rcout << "input string \'" << str << "\'\n making a digram table\n";

  // define the objects we ar work with
  std::string s = Rcpp::as<std::string>(str);
  std::string delimiter = " ";

  // digrams array shall be built first it is digram -> indexes
  std::map<std::string, std::vector<size_t> > digrams;

  // working vars
  Digram c_digram;
  std::string old_token;
  std::string token;
  size_t token_counter = 0;

  size_t pos = 0; // the tokenizer var
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    std::cout << token << std::endl;

    if (!old_token.empty()) {
      Token p(old_token, token_counter - 1);
      Token n(token, token_counter);
      Digram d(&p, &n);
      std::cout << d << std::endl;
    }
    s.erase(0, pos + delimiter.length());
    old_token = token;
    token_counter++;
  }
  std::cout << s << std::endl;

  return str;
}
