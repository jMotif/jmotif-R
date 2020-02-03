#include <jmotif.h>
using namespace Rcpp ;

// constructor
//
repair_digram::repair_digram( const std::string digram_dtring, int frequency ) {
 digram = digram_dtring;
 freq = frequency;
}
