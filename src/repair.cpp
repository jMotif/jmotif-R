#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
//

class Token {
  public:
    int str_index;
    std::string payload;
};

class Digram: public Token {
public:
  Token* first;
  Token* second;
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
};

Grammar str_to_repair_grammar(CharacterVector str){

}
