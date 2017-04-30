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
std::map<int, std::string> sax_via_window(NumericVector ts, int w_size, int paa_size, int a_size,
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
Rcpp::DataFrame find_discords_hotsax(NumericVector ts, int w_size, int paa_size,
                                      int a_size, double n_threshold, int discords_num);

//
// REPAIR
//

// the basic work string token
//
class repair_symbol {
public:
  std::string payload;
  int str_index;
  repair_symbol() { // a generic constructor
    str_index = -1;
  };
  repair_symbol(const std::string str, int index); // a more advance constructor
  virtual bool is_guard(){ // if this is a guard? no... overriden in Guard
    return false;
  }
  std::string to_string(){ // get the payload
    return std::string(payload);
  }
};

// the symbol (token) wrapper for the string data structure0
//
class repair_symbol_record {
public:
  repair_symbol* payload;
  repair_symbol_record* prev;
  repair_symbol_record* next;
  repair_symbol_record( repair_symbol* symbol );
};

// the grammar rule
//
class repair_rule {
public:
  int id;
  int rule_use;
  repair_symbol* first;
  repair_symbol* second;
  std::string expanded_rule_string;
  std::vector<int> occurrences;
  repair_rule(){
    id = -1; rule_use = 0; first = nullptr; second = nullptr;
  };
  std::string get_rule_string();
};

// the guard symbol: a special symbol that holds the rule
//
class repair_guard: public repair_symbol {
public:
  repair_rule* r;
  repair_guard();
  repair_guard(repair_rule* rule, int idx){
    r = rule;
    payload = r->get_rule_string();
    str_index = idx;
  }
  bool is_guard(){
    return true;
  }
  std::string get_expanded_string(){
    return r->expanded_rule_string;
  }
};

//
// the priority queue elements...
//
class repair_digram {
public:
  std::string digram;
  int freq;
  repair_digram(const std::string str, int index);
};

//
// the priority queue (a dobly-linked list) node
//
class repair_pqueue_node {
public:
  repair_pqueue_node* prev;
  repair_pqueue_node* next;
  repair_digram* payload;
  repair_pqueue_node() {
    payload = nullptr;
    prev = nullptr;
    next = nullptr;
  }
  repair_pqueue_node(repair_digram* d) {
    payload = d;
    prev = nullptr;
    next = nullptr;
  }
};

//
// the priority queue taking care about repair digrams ordering
//
class repair_priority_queue {
public:
  repair_pqueue_node* queue_head; // queue head pointer
  std::unordered_map<std::string, repair_pqueue_node*> nodes; // the fastmap <digram> -> <node>
  repair_priority_queue() {
    queue_head = nullptr;
    std::unordered_map<std::string, repair_pqueue_node*> nodes;
  }
  repair_digram* enqueue(repair_digram* digram);
  repair_digram* dequeue();
  repair_digram* peek();
  repair_digram* get(std::string *digram_string);
  repair_digram* update_digram_frequency(std::string *digram_string, int new_value);
  bool contains_digram(std::string *digram_string);
  std::vector<repair_digram> to_array();
  void remove_node(repair_pqueue_node* node);
  std::string to_string();
  bool consistency_check();
};

//
// RRA
//
class rule_record {
public:
  int rule_id;
  std::string rule_string;
  std::string expanded_rule_string;
  std::vector<int> rule_occurrences;
  std::vector<std::pair<int, int>> rule_intervals;
  // int rule_use;
  rule_record() {
    rule_id = -1;
    // rule_use = 0;
    std::vector<std::pair<int, int>> rule_intervals;
  }
};

struct rra_discord_record {
  int rule;
  int start;
  int end;
  double nn_distance;
};

std::unordered_map<int, rule_record*> _str_to_repair_grammar(std::string s);
Rcpp::List str_to_repair_grammar(CharacterVector str);

Rcpp::DataFrame find_discords_rra(NumericVector series, int w_size, int paa_size,
  int a_size, CharacterVector nr_strategy, double n_threshold = 0.01, int discords_num = 3);

//
// Utilities
//
NumericVector col_means(NumericMatrix m);
//
NumericVector subseries(NumericVector ts, int start, int end);
//
int armaRand();

// internal high performance computing
//
std::vector<double> _alphabet_to_cuts(int a_size);
int _count_spaces(std::string *s);
double _mean(std::vector<double> *ts, int *start, int *end);
std::vector<double> _znorm(std::vector<double> ts, double threshold);
std::vector<double> _paa(std::vector<double> ts, int paa_num); // deprecated
std::vector<double> _paa2(std::vector<double> ts, int paa_num);
double _euclidean_dist(std::vector<double>* seq1, std::vector<double>* seq2);
std::vector<double> _subseries(std::vector<double>* ts, int start, int end);
//
std::string _series_to_string(std::vector<double> ts, int a_size);
bool _is_equal_mindist(std::string a, std::string b);
//
std::unordered_map<int, std::string> _sax_via_window(
    std::vector<double> ts, int w_size, int paa_size, int a_size,
    std::string nr_strategy, double n_threshold);

#endif
