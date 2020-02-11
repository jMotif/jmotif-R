#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
//

//' Get the ASCII letter by an index.
//'
//' @param idx the index.
//' @useDynLib jmotif
//' @export
//' @examples
//' # letter 'b'
//' idx_to_letter(2)
// [[Rcpp::export]]
char idx_to_letter(int idx) {
  return LETTERS[idx-1];
}

//' Get the index for an ASCII letter.
//'
//' @param letter the letter.
//' @useDynLib jmotif
//' @export
//' @examples
//' # letter 'b' translates to 2
//' letter_to_idx('b')
// [[Rcpp::export]]
int letter_to_idx(char letter) {
  return letter - 96;
}

//' Get an ASCII indexes sequence for a given character array.
//'
//' @param str the character array.
//' @useDynLib jmotif
//' @export
//' @examples
//' letters_to_idx(c('a','b','c','a'))
// [[Rcpp::export]]
IntegerVector letters_to_idx(CharacterVector str) {
  IntegerVector res(str.length());
  for(int i=0; i<str.length(); i++){
    res[i] = letter_to_idx((str[i])[0]);
  }
  return res;
}

//' Compares two strings using natural letter ordering.
//'
//' @param a the string a.
//' @param b the string b.
//' @useDynLib jmotif
//' @export
//' @examples
//' is_equal_str("aaa", "bbb")
//' is_equal_str("ccc", "ccc")
// [[Rcpp::export]]
bool is_equal_str(CharacterVector a, CharacterVector b) {
  std::string ca = Rcpp::as<std::string>(a);
  std::string cb = Rcpp::as<std::string>(b);
  // Rcout << ca << " and " << cb << "\n";
  return (ca == cb);
}

//' Compares two strings using mindist.
//'
//' @param a the string a.
//' @param b the string b.
//' @useDynLib jmotif
//' @export
//' @examples
//' is_equal_str("aaa", "bbb") # true
//' is_equal_str("aaa", "ccc") # false
// [[Rcpp::export]]
bool is_equal_mindist(CharacterVector a, CharacterVector b) {
  std::string ca = Rcpp::as<std::string>(a);
  std::string cb = Rcpp::as<std::string>(b);
  if(ca.length() != cb.length()){
    return false;
  }else{
    for(unsigned i=0; i<ca.length(); i++){
      if( abs(ca[i] - cb[i]) > 1 ){
        return false;
      }
    }
  }
  return true;
}

/* bool _is_equal_mindist(std::string a, std::string b) {
  if(a.length() != b.length()){
    return false;
  }else{
    for(unsigned i=0; i<a.length(); i++){
      if( abs(a[i] - b[i]) > 1 ){
        return false;
      }
    }
  }
  return true;
}*/

int _count_spaces(std::string *s) {
  int count = 0;
  for (unsigned i = 0; i < s->size(); i++)
    if (s->at(i) == ' ') count++;
  return count;
}
