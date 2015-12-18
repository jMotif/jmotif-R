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

  std::string s = Rcpp::as<std::string>(str);

  std::string delimiter = " ";
  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
   token = s.substr(0, pos);
   std::cout << token << std::endl;
   s.erase(0, pos + delimiter.length());
 }
 std::cout << s << std::endl;

  return "test\n";
}
