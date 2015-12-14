#ifndef JMOTIF_JMOTIF_H
#define JMOTIF_JMOTIF_H

#include <RcppArmadillo.h>
using namespace Rcpp ;

//define letters array
//
const char LETTERS[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                        'q', 'r', 's', 't', 'u',  'v', 'w', 'x',
                        'y', 'z'};

NumericVector znorm(NumericVector ts, double threshold = 0.01);

#endif
