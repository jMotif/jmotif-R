#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
//

//' Translates an alphabet size into the array of corresponding SAX cut-lines built using the Normal distribution.
//'
//' @param a_size the alphabet size, a value between 2 and 20 (inclusive).
//' @useDynLib jmotif
//' @export
//' @references Lonardi, S., Lin, J., Keogh, E., Patel, P.,
//' Finding motifs in time series.
//' In Proc. of the 2nd Workshop on Temporal Data Mining (pp. 53-68). (2002)
//' @examples
//' alphabet_to_cuts(5)
// [[Rcpp::export]]
NumericVector alphabet_to_cuts(int a_size) {
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
  default: { stop("'a_size' is invalid"); return NumericVector::create(0.0); }
  }
}

std::vector<double> _alphabet_to_cuts(int a_size) {
  switch(a_size){
  case 2: {return std::vector<double> {std::numeric_limits<double>::lowest(),  0.00};}
  case 3: {return std::vector<double> {std::numeric_limits<double>::lowest(), -0.43,  0.43};}
  case 4: {return std::vector<double> {std::numeric_limits<double>::lowest(), -0.67,  0.00,  0.67};}
  case 5: {return std::vector<double> {std::numeric_limits<double>::lowest(), -0.84, -0.25,  0.25,  0.84};}
  case 6: {return std::vector<double> {std::numeric_limits<double>::lowest(), -0.97, -0.43,  0.00,  0.43,  0.97};}
  case 7: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.07, -0.57, -0.18,  0.18,  0.57,  1.07};}
  case 8: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.15, -0.67, -0.32,  0.00,  0.32,  0.67,  1.15};}
  case 9: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.22, -0.76, -0.43, -0.14,  0.14,  0.43,  0.76,  1.22};}
  case 10: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.28, -0.84, -0.52, -0.25,  0.00,  0.25,  0.52,  0.84,  1.28};}
  case 11: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.34, -0.91, -0.60, -0.35, -0.11,  0.11,  0.35,  0.60,  0.91, 1.34};}
  case 12: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.38, -0.97, -0.67, -0.43, -0.21,  0.00,  0.21,  0.43,  0.67, 0.97, 1.38};}
  case 13: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.43, -1.02, -0.74, -0.50, -0.29, -0.10,  0.10,  0.29,  0.50, 0.74, 1.02, 1.43};}
  case 14: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.47, -1.07, -0.79, -0.57, -0.37, -0.18,  0.00,  0.18,  0.37, 0.57, 0.79, 1.07, 1.47};}
  case 15: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.50, -1.11, -0.84, -0.62, -0.43, -0.25, -0.08,  0.08,  0.25, 0.43, 0.62, 0.84, 1.11, 1.5};}
  case 16: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.53, -1.15, -0.89, -0.67, -0.49, -0.32, -0.16,  0.00,  0.16, 0.32, 0.49, 0.67, 0.89, 1.15, 1.53};}
  case 17: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.56, -1.19, -0.93, -0.72, -0.54, -0.38, -0.22, -0.07,  0.07, 0.22, 0.38, 0.54, 0.72, 0.93, 1.19, 1.56};}
  case 18: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.59, -1.22, -0.97, -0.76, -0.59, -0.43, -0.28, -0.14,  0.00, 0.14, 0.28, 0.43, 0.59, 0.76, 0.97, 1.22, 1.59};}
  case 19: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.62, -1.25, -1.00, -0.80, -0.63, -0.48, -0.34, -0.20, -0.07, 0.07, 0.20, 0.34, 0.48, 0.63, 0.80, 1.00, 1.25, 1.62};}
  case 20: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.64, -1.28, -1.04, -0.84, -0.67, -0.52, -0.39, -0.25, -0.13, 0.00, 0.13, 0.25, 0.39, 0.52, 0.67, 0.84, 1.04, 1.28, 1.64};}
  default: { stop("'a_size' is invalid"); std::vector<double> {0.0}; }
  }
}

//' Transforms a time series into the char array using SAX and the normal alphabet.
//'
//' @param ts the timeseries.
//' @param a_size the alphabet size.
//' @useDynLib jmotif
//' @export
//' @references Lonardi, S., Lin, J., Keogh, E., Patel, P.,
//' Finding motifs in time series.
//' In Proc. of the 2nd Workshop on Temporal Data Mining (pp. 53-68). (2002)
//' @examples
//' y = c(-1, -2, -1, 0, 2, 1, 1, 0)
//' y_paa3 = paa(y, 3)
//' series_to_chars(y_paa3, 3)
// [[Rcpp::export]]
CharacterVector series_to_chars(NumericVector ts, int a_size) {
  std::vector<double> cuts = _alphabet_to_cuts(a_size);
  int len = ts.length();
  std::vector<char> res(len);
  for (int i=0; i<len; i++) {
    int ctr = 0;
    for(int j=0; j<cuts.size(); j++){
      if(cuts[j]<ts[i]){
        ctr++;
      } else {
        break;
      }
    }
    res[i] = idx_to_letter(ctr);
  }
  return Rcpp::wrap(res);
}

//' Transforms a time series into the string.
//'
//' @param ts the timeseries.
//' @param a_size the alphabet size.
//' @useDynLib jmotif
//' @export
//' @references Lonardi, S., Lin, J., Keogh, E., Patel, P.,
//' Finding motifs in time series.
//' In Proc. of the 2nd Workshop on Temporal Data Mining (pp. 53-68). (2002)
//' @examples
//' y = c(-1, -2, -1, 0, 2, 1, 1, 0)
//' y_paa3 = paa(y, 3)
//' series_to_string(y_paa3, 3)
// [[Rcpp::export]]
CharacterVector series_to_string(NumericVector ts, int a_size) {
  std::vector<double> cuts = _alphabet_to_cuts(a_size);
  int len = ts.length();
  std::string res(len, ' ');
  for (int i=0; i<len; i++) {
    int ctr = 0;
    for(int j=0; j<cuts.size(); j++){
      if(cuts[j]<ts[i]){
        ctr++;
      } else {
        break;
      }
    }
    res[i] = idx_to_letter(ctr);
  }
  return Rcpp::wrap(res);
}

std::string _series_to_string(std::vector<double> ts, int a_size) {
  std::vector<double> cuts = _alphabet_to_cuts(a_size);
  int len = ts.size();
  std::string res(len, ' ');
  for (int i=0; i<len; i++) {
    int ctr = 0;
    for(int j=0; j<cuts.size(); j++){
      if(cuts[j]<ts[i]){
        ctr++;
      } else {
        break;
      }
    }
    res[i] = idx_to_letter(ctr);
  }
  return res;
}

std::map<int, std::string> _sax_via_window(
    std::vector<double> ts, int w_size, int paa_size, int a_size,
    std::string nr_strategy, double n_threshold) {

  std::map<int, std::string> idx2word;

  std::string old_str;

  for (int i = 0; i <= ts.size() - w_size; i++) {

    // subseries extraction
    std::vector<double>::const_iterator first = ts.begin() + i;
    std::vector<double>::const_iterator last = ts.begin() + i + w_size;
    std::vector<double> subSection(first, last);

//     for (auto i = subSection.begin(); i != subSection.end(); ++i)
//       Rcout << *i << ' ';
//     Rcout << "\n";

    subSection = _znorm(subSection, n_threshold);

//     for (auto i = subSection.begin(); i != subSection.end(); ++i)
//       Rcout << *i << ' ';
//     Rcout << "\n";

    subSection = _paa(subSection, paa_size);

    std::string curr_str = _series_to_string(subSection, a_size);

    if (!(old_str.empty())) {
      if ("exact"==nr_strategy && old_str==curr_str) {
        continue;
      }
      else if ("mindist"==nr_strategy && is_equal_mindist(old_str, curr_str) ) {
        continue;
      }
    }

    idx2word.insert(std::make_pair(i,curr_str));

    old_str = curr_str;
  }


  return idx2word;
}


//' Discretizes a time series with SAX via sliding window.
//'
//' @param ts the input timeseries.
//' @param w_size the sliding window size.
//' @param paa_size the PAA size.
//' @param a_size the alphabet size.
//' @param nr_strategy the Numerosity Reduction strategy, acceptable values are "exact" and "mindist" --
//' any other value triggers no numerosity reduction.
//' @param n_threshold the normalization threshold.
//' @useDynLib jmotif
//' @export
//' @references Lonardi, S., Lin, J., Keogh, E., Patel, P.,
//' Finding motifs in time series.
//' In Proc. of the 2nd Workshop on Temporal Data Mining (pp. 53-68). (2002)
// [[Rcpp::export]]
std::map<int, CharacterVector> sax_via_window(
    NumericVector ts, int w_size, int paa_size, int a_size,
    CharacterVector nr_strategy, double n_threshold) {

  // Rcout << "ts of length " << ts.length();
  // Rcout << ", win " << w_size;
  // Rcout << ", paa " << paa_size;
  // Rcout << ", a " << a_size;
  // Rcout << ", nr_strategy '" << nr_strategy << "'";
  // Rcout << ", n_th " << n_threshold << "\n";

  std::map<int, CharacterVector> idx2word;

  CharacterVector old_str("");

  for (int i = 0; i <= ts.length() - w_size; i++) {

    NumericVector subSection = subseries(ts, i, i + w_size);

    subSection = znorm(subSection, n_threshold);

    subSection = paa(subSection, paa_size);

    CharacterVector curr_str = series_to_string(subSection, a_size);

    if (!(0 == old_str.length())) {
      if ( is_equal_str("exact", nr_strategy)
             && is_equal_str(old_str, curr_str) ) {
        continue;
      }
      else if (is_equal_str("mindist", nr_strategy)
                 && is_equal_mindist(old_str, curr_str) ) {
        continue;
      }
    }

    idx2word.insert(std::make_pair(i,curr_str));

    old_str = curr_str;
  }
  return idx2word;
}

//' Discretize a time series with SAX using chunking (no sliding window).
//'
//' @param ts the input time series.
//' @param paa_size the PAA size.
//' @param a_size the alphabet size.
//' @param n_threshold the normalization threshold.
//' @useDynLib jmotif
//' @export
//' @references Lonardi, S., Lin, J., Keogh, E., Patel, P.,
//' Finding motifs in time series.
//' In Proc. of the 2nd Workshop on Temporal Data Mining (pp. 53-68). (2002)
// [[Rcpp::export]]
std::map<int, CharacterVector> sax_by_chunking(
    NumericVector ts, int paa_size, int a_size, double n_threshold) {

  // Rcout << "ts of length " << ts.length();
  // Rcout << ", paa " << paa_size;
  // Rcout << ", a " << a_size;
  // Rcout << ", n_th " << n_threshold << "\n";

  std::map<int, CharacterVector> idx2word;

  NumericVector vec = znorm(ts, n_threshold);

  vec = paa(vec, paa_size);

  std::string curr_str = Rcpp::as<std::string>(series_to_string(vec, a_size));
  // Rcout << curr_str << "\n";

  for(unsigned i=0; i<curr_str.length(); i++){
    idx2word.insert(std::make_pair(i,curr_str.substr(i,1)));
  }

  return idx2word;
}
