#include <Rcpp.h>
using namespace Rcpp;

const char LETTERS[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                        'q', 'r', 's', 't', 'u',  'v', 'w', 'x',
                        'y', 'z'};

//' z-Normalize a time series
//'
//' @param x A time series.
//' @param threshold A z-normalization threshold.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
NumericVector znorm_cpp(NumericVector x, double threshold = 0.01) {

  double x_sd = sd(x);

   if (x_sd < threshold){
    return clone(x);
  }

  return (x - mean(x)) / x_sd;

}


//' Reshape matrix
//'
//' @param a A matrix to reshape.
//' @param n new row size.
//' @param m new column size.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
NumericMatrix reshape_cpp(NumericMatrix a, int n, int m) {

  int ce = 0;
  int n_rows = a.nrow();

  NumericMatrix res(n, m);

  for (int j = 0; j < m; j++) {
    for (int i = 0; i < n; i++) {
      res(i,j) = a(ce % n_rows, ce / n_rows);
      ce++;
    }
  }

  return res;

}

//' Computes column means
//'
//' @param a A matrix to use.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
NumericVector col_means_cpp(NumericMatrix a) {
  NumericVector res(a.ncol());
  for (int j = 0; j < a.ncol(); j++) {
    res[j] = mean(a(_,j));
  }
  return res;
}

//' Compute PAA
//'
//' @param ts A timeseries to convert into PAA.
//' @param paa_num the desired PAA size.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
NumericVector paa_cpp(NumericVector ts, int paa_num) {

  // fix the length
  int len = ts.length();

  // check for the trivial case
  if (len == paa_num) {
    return clone(ts);
  }
  else {
      NumericVector res(paa_num);
      for (int i = 0; i < len * paa_num; i++) {
        int idx = i / len; // the spot
        int pos = i / paa_num; // the col spot
        res[idx] = res[idx] + ts[pos];
      }
      for (int i = 0; i < paa_num; i++) {
        res[i] = res[i] / (double) len;
      }
      return res;
  }

}

//' Get a letter by index
//'
//' @param idx The index.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
char idx2letter_cpp(int idx) {
  return LETTERS[idx-1];
}

//' Get an index for a letter
//'
//' @param letter The letter.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
int letter2idx_cpp(char letter) {
  return letter - 96;
}

//' Get an index sequence by string
//'
//' @param str The char array.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
IntegerVector letters2idx_cpp(CharacterVector str) {
  IntegerVector res(str.length());
  for(int i=0; i<str.length(); i++){
    res[i] = letter2idx_cpp((str[i])[0]);
  }
  return res;
}

//' Translates an alphabet size into the array of corresponding
//' SAX cut-lines assuming the Normal distribution
//'
//' @param a_size the alphabet size, a value between 2 and 20 (inclusive).
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
NumericVector alphabet2cuts_cpp(int a_size) {
  switch(a_size){
    case 2: {return NumericVector::create(R_NegInf,  0.00);}
    case 3: {return NumericVector::create(R_NegInf, -0.43,  0.43);}
    case 4: {return NumericVector::create(R_NegInf, -0.67,  0.00,  0.67);}
    case 5: {return NumericVector::create(R_NegInf, -0.84, -0.25,  0.25,  0.84);}
    case 6: {return NumericVector::create(R_NegInf, -0.97, -0.43,  0.00,  0.43,  0.97);}
    case 7: {return NumericVector::create(R_NegInf, -1.07, -0.57, -0.18,  0.18,  0.57,  1.07);}
    case 8: {return NumericVector::create(R_NegInf, -1.15, -0.67, -0.32,  0.00,  0.32,  0.67,  1.15);}
    case 9: {return NumericVector::create(R_NegInf, -1.22, -0.76, -0.43, -0.14,  0.14,  0.43,  0.76,  1.22);}
    case 10: {return NumericVector::create(R_NegInf, -1.28, -0.84, -0.52, -0.25,  0.00,  0.25,  0.52,  0.84,  1.28);}
    case 11: {return NumericVector::create(R_NegInf, -1.34, -0.91, -0.60, -0.35, -0.11,  0.11,  0.35,  0.60,  0.91, 1.34);}
    case 12: {return NumericVector::create(R_NegInf, -1.38, -0.97, -0.67, -0.43, -0.21,  0.00,  0.21,  0.43,  0.67, 0.97, 1.38);}
    case 13: {return NumericVector::create(R_NegInf, -1.43, -1.02, -0.74, -0.50, -0.29, -0.10,  0.10,  0.29,  0.50, 0.74, 1.02, 1.43);}
    case 14: {return NumericVector::create(R_NegInf, -1.47, -1.07, -0.79, -0.57, -0.37, -0.18,  0.00,  0.18,  0.37, 0.57, 0.79, 1.07, 1.47);}
    case 15: {return NumericVector::create(R_NegInf, -1.50, -1.11, -0.84, -0.62, -0.43, -0.25, -0.08,  0.08,  0.25, 0.43, 0.62, 0.84, 1.11, 1.5);}
    case 16: {return NumericVector::create(R_NegInf, -1.53, -1.15, -0.89, -0.67, -0.49, -0.32, -0.16,  0.00,  0.16, 0.32, 0.49, 0.67, 0.89, 1.15, 1.53);}
    case 17: {return NumericVector::create(R_NegInf, -1.56, -1.19, -0.93, -0.72, -0.54, -0.38, -0.22, -0.07,  0.07, 0.22, 0.38, 0.54, 0.72, 0.93, 1.19, 1.56);}
    case 18: {return NumericVector::create(R_NegInf, -1.59, -1.22, -0.97, -0.76, -0.59, -0.43, -0.28, -0.14,  0.00, 0.14, 0.28, 0.43, 0.59, 0.76, 0.97, 1.22, 1.59);}
    case 19: {return NumericVector::create(R_NegInf, -1.62, -1.25, -1.00, -0.80, -0.63, -0.48, -0.34, -0.20, -0.07, 0.07, 0.20, 0.34, 0.48, 0.63, 0.80, 1.00, 1.25, 1.62);}
    case 20: {return NumericVector::create(R_NegInf, -1.64, -1.28, -1.04, -0.84, -0.67, -0.52, -0.39, -0.25, -0.13, 0.00, 0.13, 0.25, 0.39, 0.52, 0.67, 0.84, 1.04, 1.28, 1.64);}
    default: {  stop("'a_size' is invalid"); }
  }
  return NumericVector::create(0.0);
}

//' Transforms a time series into a char array
//'
//' @param ts the timeseries
//' @param a_size the alphabet size
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
CharacterVector ts2chars_cpp(NumericVector ts, int a_size) {
  NumericVector cuts = alphabet2cuts_cpp(a_size);
  int len = ts.length();
  CharacterVector res(len);
    for (int i=0; i<len; i++) {
      NumericVector dd = cuts[cuts <= ts[i]];
      char b[] = {idx2letter_cpp(dd.length()), '\0'};
      res[i] = b;
    }
  return res;
}

//' Transforms a time series into a char array
//'
//' @param ts the timeseries
//' @param a_size the alphabet size
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
CharacterVector ts2string_cpp(NumericVector ts, int a_size) {
  NumericVector cuts = alphabet2cuts_cpp(a_size);
  int len = ts.length();
  std::string res(len, ' ');
  for (int i=0; i<len; i++) {
    NumericVector dd = cuts[cuts <= ts[i]];
    res[i] = idx2letter_cpp(dd.length());
  }
  return res;
}
