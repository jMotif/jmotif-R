#include <jmotif.h>
using namespace Rcpp ;

// constructor
//
repair_symbol_record::repair_symbol_record( repair_symbol* symbol ) {
  payload = symbol;
  next = nullptr;
  prev = nullptr;
}

