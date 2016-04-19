#include <jmotif.h>
using namespace Rcpp ;

// constructor
//
repair_symbol::repair_symbol(const std::string str, int index ) {
  payload = str;
  str_index = index;
}
