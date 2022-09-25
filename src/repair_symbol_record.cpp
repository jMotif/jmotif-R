#include <jmotif.h>

// constructor
//
repair_symbol_record::repair_symbol_record( repair_symbol* symbol ) {
  payload = symbol;
  next = nullptr;
  prev = nullptr;
}

