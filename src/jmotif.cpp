#include <Rcpp.h>
//#include <numeric>
//#include <math>
using namespace Rcpp;
// Enable C++11 via this plugin (Rcpp 0.10.3 or later)
// [[Rcpp::plugins("cpp11")]]

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
//' @examples
//' x = seq(0, pi*4, 0.02)
//' y = sin(x) * 5 + rnorm(length(x))
//' plot(x, y, type="l", col="blue")
//' lines(x, znorm(y, 0.01), type="l", col="red")
// [[Rcpp::export]]
NumericVector znorm(NumericVector x, double threshold = 0.01) {

  double x_sd = sd(x);

   if (x_sd < threshold){
    return clone(x);
  }

  return (x - mean(x)) / x_sd;

}

//' Computes column means
//'
//' @param a A matrix to use.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
NumericVector col_means(NumericMatrix a) {
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
//' @examples
//' x = c(-1, -2, -1, 0, 2, 1, 1, 0)
//' plot(x, type = "l", main = "8-points time series and it PAA transform into three points")
//' points(x,pch = 16, lwd = 5)
//' # segments
//' abline(v = c(1, 1+7/3, 1+7/3 * 2, 8), lty = 3, lwd = 2)
// [[Rcpp::export]]
NumericVector paa(NumericVector ts, int paa_num) {

  // fix the length
  int len = ts.length();

  // check for the trivial case
  if (len == paa_num) {
    return clone(ts);
  }
  else {
    if (len % paa_num == 0) {
      NumericVector res(paa_num);
      int inc = len / paa_num;
      for (int i = 0; i < len; i++) {
        int idx = i / inc; // the spot
        res[idx] += ts[i];
      }
      for (int i = 0; i < paa_num; i++) {
        res[i] = res[i] / (double) (inc);
      }
      return res;
    }else{
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

}

//' Get a letter by index
//'
//' @param idx The index.
//' @useDynLib jmotif
//' @export
//' @examples
//' # letter 'b'
//' idx_to_letter(2)
// [[Rcpp::export]]
char idx_to_letter(int idx) {
  return LETTERS[idx-1];
}

//' Get an index for a letter
//'
//' @param letter The letter.
//' @useDynLib jmotif
//' @export
//' @examples
//' # letter 'b' translates to 2
//' letter_to_idx('b')
// [[Rcpp::export]]
int letter_to_idx(char letter) {
  return letter - 96;
}

//' Get an index sequence by string
//'
//' @param str The char array.
//' @useDynLib jmotif
//' @export
//' @examples
//' letters_to_idx(c('a','b','c','a'))
// [[Rcpp::export]]
IntegerVector letters_to_idx(CharacterVector str) {
  IntegerVector res(str.length());
  for(int i=0; i<str.length(); i++){
    res[i] = letter_to_idx((str[i])[0]);
  }
  return res;
}

//' Translates an alphabet size into the array of corresponding SAX cut-lines using the Normal distribution
//'
//' @param a_size the alphabet size, a value between 2 and 20 (inclusive).
//' @useDynLib jmotif
//' @export
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
CharacterVector ts_2_chars(NumericVector ts, int a_size) {
  NumericVector cuts = alphabet_to_cuts(a_size);
  int len = ts.length();
  CharacterVector res(len);
    for (int i=0; i<len; i++) {
      NumericVector dd = cuts[cuts <= ts[i]];
      char b[] = {idx_to_letter(dd.length()), '\0'};
      res[i] = b;
    }
  return res;
}

//' Transforms a time series into a string
//'
//' @param ts the timeseries
//' @param a_size the alphabet size
//' @useDynLib jmotif
//' @export
//' @examples
//' y = c(-1, -2, -1, 0, 2, 1, 1, 0)
//' y_paa3 = paa(y, 3)
//' ts_to_string(y_paa3, 3)
// [[Rcpp::export]]
CharacterVector ts_to_string(NumericVector ts, int a_size) {
  NumericVector cuts = alphabet_to_cuts(a_size);
  int len = ts.length();
  std::string res(len, ' ');
  for (int i=0; i<len; i++) {
    NumericVector dd = cuts[cuts <= ts[i]];
    res[i] = idx_to_letter(dd.length());
  }
  return wrap(res);
}

//' Extracting subseries
//'
//' @param x the timeseries (0-based)
//' @param start the interval start
//' @param end the interval end
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
NumericVector subseries(NumericVector x, int start, int end) {
  NumericVector res(end-start);
  for (int i=start; i<end; i++) {
    res[i-start] = x[i];
  }
  return res;
}

//' Comparing strings
//'
//' @param a the string a
//' @param b the string b
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
bool is_equal_str(CharacterVector a, CharacterVector b) {
  std::string ca = Rcpp::as<std::string>(a);
  std::string cb = Rcpp::as<std::string>(b);
  // Rcout << ca << " and " << cb << "\n";
  return (ca == cb);
}

//' SAXifying a timeseries
//'
//' @param ts the timeseries
//' @param w_size the sliding window size
//' @param paa_size the PAA size
//' @param a_size the alphabet size
//' @param nr_strategy the NR strategy
//' @param n_threshold the normalization threshold
//' @useDynLib jmotif
//' @export
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

  typedef std::map<int, CharacterVector> idx2wordMap;
  idx2wordMap idx2word;

  CharacterVector old_str("");

  for (int i = 0; i < ts.length() - w_size; i++) {

    NumericVector subSection = subseries(ts, i, i + w_size);

    subSection = znorm(subSection, n_threshold);

    subSection = paa(subSection, paa_size);

    CharacterVector curr_str = ts_to_string(subSection, a_size);

    // Rcout << curr_str << "\n";

    if (!(0 == old_str.length())) {

      if ( is_equal_str("exact", nr_strategy)
            && is_equal_str(old_str, curr_str) ) {
        continue;
      }
      else if (is_equal_str("mindist", nr_strategy)
                 && is_equal_str(old_str, curr_str) ) {
        continue;
      }

    }

    idx2word.insert(std::make_pair(i,curr_str));

    old_str = curr_str;

  }

  return idx2word;
}

//' SAXifying a timeseries
//'
//' @param ts the timeseries
//' @param paa_size the PAA size
//' @param a_size the alphabet size
//' @param n_threshold the normalization threshold
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
std::map<int, CharacterVector> sax_by_chunking(
    NumericVector ts, int paa_size, int a_size, double n_threshold) {

  // Rcout << "ts of length " << ts.length();
  // Rcout << ", paa " << paa_size;
  // Rcout << ", a " << a_size;
  // Rcout << ", n_th " << n_threshold << "\n";

  typedef std::map<int, CharacterVector> idx2wordMap;
  idx2wordMap idx2word;

  NumericVector vec = znorm(ts, n_threshold);

  vec = paa(vec, paa_size);

  std::string curr_str = Rcpp::as<std::string>(ts_to_string(vec, a_size));
  // Rcout << curr_str << "\n";

  for(int i=0; i<curr_str.length(); i++){
    idx2word.insert(std::make_pair(i,curr_str.substr(i,1)));
  }

  return idx2word;
}

//' SAXifying a timeseries
//'
//' @param ts the timeseries
//' @param w_size the sliding window size
//' @param paa_size the PAA size
//' @param a_size the alphabet size
//' @param nr_strategy the NR strategy
//' @param n_threshold the normalization threshold
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
Rcpp::DataFrame series_to_wordbag(
  NumericVector ts, int w_size, int paa_size, int a_size,
  CharacterVector nr_strategy, double n_threshold) {

  std::map<std::string, int> word_bag;

  std::string old_str;

  for (int i = 0; i < ts.length() - w_size; i++) {

    NumericVector subSection = subseries(ts, i, i + w_size);

    subSection = znorm(subSection, n_threshold);

    subSection = paa(subSection, paa_size);

    std::string curr_str = Rcpp::as<std::string>(
      ts_to_string(subSection, a_size));

    // Rcout << curr_str << "\n";

    if (!(0 == old_str.length())) {
      if ( is_equal_str("exact", nr_strategy)
            && is_equal_str(old_str, curr_str) ) {
        continue;
      }
      else if (is_equal_str("mindist", nr_strategy)
                 && is_equal_str(old_str, curr_str) ) {
        continue;
      }
    }

    // Rcout << "processing " << curr_str << "\n";

    if (word_bag.find(curr_str) == word_bag.end()){
      // Rcout << "word " << curr_str << " not found \n";
      word_bag.insert(std::make_pair(curr_str, 1));
    }else{
      // Rcout << "word " << curr_str << " found: " << word_bag[curr_str] << "\n";
      word_bag[curr_str] += 1;
    }

    old_str = curr_str;

  }

  std::vector<std::string> k;
  std::vector<int> v;
  for(std::map<std::string, int>::iterator it = word_bag.begin();
      it != word_bag.end(); ++it) {
    k.push_back(it->first);
    v.push_back(it->second);
  }

  return Rcpp::DataFrame::create( Named("words")= k,
                                  Named("counts") = v,
                                  Named("stringsAsFactors") = false);

}

//' SAXifying a bunch of timeseries into a wod bag
//'
//' @param data the timeseries data, row-wise
//' @param w_size the sliding window size
//' @param paa_size the PAA size
//' @param a_size the alphabet size
//' @param nr_strategy the NR strategy
//' @param n_threshold the normalization threshold
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
Rcpp::DataFrame manyseries_to_wordbag(
    NumericMatrix data, int w_size, int paa_size, int a_size,
    CharacterVector nr_strategy, double n_threshold) {

  std::map<std::string, int> word_bag; // the result

  for(int s=0; s<data.nrow(); s++){

    NumericVector ts = data.row(s);

    std::string old_str;

    for (int i = 0; i < ts.length() - w_size; i++) {

      NumericVector subSection = subseries(ts, i, i + w_size);
      subSection = znorm(subSection, n_threshold);
      subSection = paa(subSection, paa_size);
      std::string curr_str = Rcpp::as<std::string>(ts_to_string(subSection, a_size));

      if (!(0 == old_str.length())) {
        if ( is_equal_str("exact", nr_strategy)
            && is_equal_str(old_str, curr_str) ) {
          continue;
        }
        else if (is_equal_str("mindist", nr_strategy)
            && is_equal_str(old_str, curr_str) ) {
          continue;
        }
      }

      if (word_bag.find(curr_str) == word_bag.end()){
        word_bag.insert(std::make_pair(curr_str, 1));
      }else{
        word_bag[curr_str] += 1;
      }

      old_str = curr_str;

    }
  }

  std::vector<std::string> k;
  std::vector<int> v;
  for(std::map<std::string, int>::iterator it = word_bag.begin();
      it != word_bag.end(); ++it) {
    k.push_back(it->first);
    v.push_back(it->second);
  }

  return Rcpp::DataFrame::create( Named("words")= k,
                                  Named("counts") = v,
                                  Named("stringsAsFactors") = false);

}

//' Computing the TFIDF matrix for a list of word bags
//'
//' @param data the word bags list
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
Rcpp::DataFrame bags_to_tfidf(Rcpp::List data) {

  // get the class labels that suppose to be the list elements' names
  //
  std::vector<std::string> names = Rcpp::as< std::vector<std::string> > (data.names());
  for(std::vector<std::string>::iterator it = names.begin(); it != names.end(); ++it) {
    Rcout << *it << "\n";
  }
  int entry_array_size = names.size();

  // iterate over the list elements building the count matrix
  //
  std::map<std::string, int[]> counts;
  for(int i = 0; i< names.size(); i++) {
    std::string class_name = names[i];

    Rcpp::DataFrame df = (Rcpp::DataFrame) data[class_name];

    std::vector<std::string> words = Rcpp::as< std::vector<std::string> > (dd["words"]);
    for(std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it) {
      Rcout << *it << "\n";
    }
  }

  return Rcpp::DataFrame::create( Named("words")= 1,
                                  Named("counts") = 2,
                                  Named("stringsAsFactors") = 3);
}
