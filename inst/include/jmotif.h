#ifndef JMOTIF_h
#define JMOTIF_h
//
#include <RcppArmadillo.h>
using namespace Rcpp ;
// Enable C++11 via this plugin (Rcpp 0.10.3 or later)
// [[Rcpp::plugins("cpp11")]]

//
// Define the letters array
//
const char LETTERS[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                        'q', 'r', 's', 't', 'u',  'v', 'w', 'x', 'y', 'z'};

//
// SAX stack
//
NumericVector znorm(NumericVector ts, double threshold);
//
NumericVector paa(NumericVector ts, int paa_num);
//
NumericVector alphabet_to_cuts(int a_size);
//
CharacterVector series_to_chars(NumericVector ts, int a_size);
//
CharacterVector series_to_string(NumericVector ts, int a_size);
//
std::map<int, CharacterVector> sax_via_window(NumericVector ts, int w_size, int paa_size, int a_size,
    CharacterVector nr_strategy, double n_threshold);
//
std::map<int, CharacterVector> sax_by_chunking(NumericVector ts, int paa_size,
                                               int a_size, double n_threshold);

//
// Strings operations
//
char idx_to_letter(int idx);
//
int letter_to_idx(char letter);
//
IntegerVector letters_to_idx(CharacterVector str);
//
bool is_equal_str(CharacterVector a, CharacterVector b);
//
bool is_equal_mindist(CharacterVector a, CharacterVector b);

//
// Distance
//
double euclidean_dist(NumericVector seq1, NumericVector seq2);
//
double early_abandoned_dist(NumericVector seq1, NumericVector seq2, double upper_limit);

//
// SAX-VSM
//
Rcpp::DataFrame series_to_wordbag(NumericVector ts, int w_size, int paa_size, int a_size,
    CharacterVector nr_strategy, double n_threshold);
//
Rcpp::DataFrame manyseries_to_wordbag(NumericMatrix data, int w_size, int paa_size, int a_size,
    CharacterVector nr_strategy, double n_threshold);
//
Rcpp::DataFrame bags_to_tfidf(Rcpp::List data);
//
Rcpp::DataFrame cosine_sim(Rcpp::List data);

//
// Discord
//
// Defines the discord record which consists of its index and the distance to NN
struct discord_record {
  int index;
  double nn_distance;
};
//
int armaRand();
//
class VisitRegistry {
public:
  int size;
  bool* registry;
  int unvisited_count;
  VisitRegistry( int capacity );
  int getNextUnvisited();
  void markVisited(int idx);
  void markVisited(int start, int end);
  bool isVisited(int idx);
  ~VisitRegistry();
};
//
Rcpp::DataFrame find_discords_brute_force(NumericVector ts, int w_size, int discords_num);
//

//
// HOT-SAX
//
Rcpp::DataFrame find_discords_hot_sax(NumericVector ts, int w_size, int paa_size,
                                      int a_size, double n_threshold, int discords_num);

//
// Utilities
//
NumericVector col_means(NumericMatrix m);
//
NumericVector subseries(NumericVector ts, int start, int end);

Rcpp::List str_to_repair_grammar(CharacterVector str);

// internal high performance computing
bool _is_equal_mindist(std::string a, std::string b);
std::vector<double> _znorm(std::vector<double> ts, double threshold);
std::vector<double> _paa(std::vector<double> ts, int paa_num);
std::string _series_to_string(std::vector<double> ts, int a_size);
std::map<int, std::string> _sax_via_window(
    std::vector<double> ts, int w_size, int paa_size, int a_size,
    std::string nr_strategy, double n_threshold);
Rcpp::DataFrame series_to_wordbag2(
    NumericVector ts, int w_size, int paa_size, int a_size,
    CharacterVector nr_strategy, double n_threshold);

#endif
