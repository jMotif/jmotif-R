#ifndef JMOTIF_h
#define JMOTIF_h
//
#include <RcppArmadillo.h>
using namespace Rcpp ;
//
//define letters array
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
#endif
