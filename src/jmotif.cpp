#include <Rcpp.h>
#include <ctime>
//#include <numeric>
//#include <math>
using namespace Rcpp;
// Enable C++11 via this plugin (Rcpp 0.10.3 or later)
// [[Rcpp::plugins("cpp11")]]

const char LETTERS[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                        'q', 'r', 's', 't', 'u',  'v', 'w', 'x',
                        'y', 'z'};

//' Z-normalizes a time series by subtracting the mean value and dividing by the standard deviation value.
//'
//' @param ts a time series to process.
//' @param threshold a z-normalization threshold value, if the input time series' standard deviation is
//' found less than this value, the procedure is not applied, so the noise would not get overamplified.
//' @useDynLib jmotif
//' @export
//' @references Dina Goldin and Paris Kanellakis,
//' On similarity queries for time-series data: Constraint specification and implementation.
//' In Principles and Practice of Constraint Programming (CP 1995), pages 137-153. (1995)
//' @examples
//' x = seq(0, pi*4, 0.02)
//' y = sin(x) * 5 + rnorm(length(x))
//' plot(x, y, type="l", col="blue")
//' lines(x, znorm(y, 0.01), type="l", col="red")
// [[Rcpp::export]]
NumericVector znorm(NumericVector ts, double threshold = 0.01) {
  double ts_sd = sd(ts);
   if (ts_sd < threshold){
    return clone(ts);
  }
  return (ts - mean(ts)) / ts_sd;
}

//' Computes the column means for a matrix.
//'
//' @param m a matrix to process.
//' @useDynLib jmotif
//' @export
//' @examples
//' x = matrix(rnorm(100), ncol=10)
//' col_means(x)
// [[Rcpp::export]]
NumericVector col_means(NumericMatrix m) {
  NumericVector res(m.ncol());
  for (int j = 0; j < m.ncol(); j++) {
    res[j] = mean(m(_,j));
  }
  return res;
}

//' Computes a Piecewise Aggregate Approximation (PAA) for a time series.
//'
//' @param ts a timeseries to compute the PAA for.
//' @param paa_num the desired PAA size.
//' @useDynLib jmotif
//' @export
//' @references Keogh, E., Chakrabarti, K., Pazzani, M., Mehrotra, S.,
//' Dimensionality reduction for fast similarity search in large time series databases.
//' Knowledge and information Systems, 3(3), 263-286. (2001)
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
    // if the number of points in a segment is even
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
      // if the number of points in a segment is odd
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

//' Get the ASCII letter by an index.
//'
//' @param idx the index.
//' @useDynLib jmotif
//' @export
//' @examples
//' # letter 'b'
//' idx_to_letter(2)
// [[Rcpp::export]]
char idx_to_letter(int idx) {
  return LETTERS[idx-1];
}

//' Get the index for an ASCII letter.
//'
//' @param letter the letter.
//' @useDynLib jmotif
//' @export
//' @examples
//' # letter 'b' translates to 2
//' letter_to_idx('b')
// [[Rcpp::export]]
int letter_to_idx(char letter) {
  return letter - 96;
}

//' Get an ASCII indexes sequence for a given character array.
//'
//' @param str the character array.
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
  NumericVector cuts = alphabet_to_cuts(a_size);
  int len = ts.length();
  std::string res(len, ' ');
  for (int i=0; i<len; i++) {
    NumericVector dd = cuts[cuts <= ts[i]];
    res[i] = idx_to_letter(dd.length());
  }
  return wrap(res);
}

//' Extracts a subseries.
//'
//' @param ts the input timeseries (0-based, left inclusive).
//' @param start the interval start.
//' @param end the interval end.
//' @useDynLib jmotif
//' @export
//' @examples
//' y = c(-1, -2, -1, 0, 2, 1, 1, 0)
//' subseries(y, 0, 3)
// [[Rcpp::export]]
NumericVector subseries(NumericVector ts, int start, int end) {
  if(start<0 || end>ts.length()){
    stop("provided start and stop indexes are invalid.");
  }
  NumericVector res(end-start);
  for (int i=start; i<end; i++) {
    res[i-start] = ts[i];
  }
  return res;
}

//' Compares two strings using natural letter ordering.
//'
//' @param a the string a.
//' @param b the string b.
//' @useDynLib jmotif
//' @export
//' @examples
//' is_equal_str("aaa", "bbb")
//' is_equal_str("ccc", "ccc")
// [[Rcpp::export]]
bool is_equal_str(CharacterVector a, CharacterVector b) {
  std::string ca = Rcpp::as<std::string>(a);
  std::string cb = Rcpp::as<std::string>(b);
  // Rcout << ca << " and " << cb << "\n";
  return (ca == cb);
}

//' Compares two strings using mindist.
//'
//' @param a the string a.
//' @param b the string b.
//' @useDynLib jmotif
//' @export
//' @examples
//' is_equal_str("aaa", "bbb") # true
//' is_equal_str("aaa", "ccc") # false
// [[Rcpp::export]]
bool is_equal_mindist(CharacterVector a, CharacterVector b) {
  std::string ca = Rcpp::as<std::string>(a);
  std::string cb = Rcpp::as<std::string>(b);
  if(ca.length() != cb.length()){
    return false;
  }else{
    for(int i=0; i<ca.length(); i++){
      // Rcout << ca[i] << " " << cb[i] << " " << " " << abs(ca[i] - cb[i]) << "\n";
      if( abs(ca[i] - cb[i]) > 1 ){
        return false;
      }
    }
  }
  return true;
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

  for(int i=0; i<curr_str.length(); i++){
    idx2word.insert(std::make_pair(i,curr_str.substr(i,1)));
  }

  return idx2word;
}

//' Converts a single time series into a bag of words.
//'
//' @param ts the timeseries.
//' @param w_size the sliding window size.
//' @param paa_size the PAA size.
//' @param a_size the alphabet size.
//' @param nr_strategy the NR strategy.
//' @param n_threshold the normalization threshold.
//' @useDynLib jmotif
//' @export
//' @references Senin Pavel and Malinchik Sergey,
//' SAX-VSM: Interpretable Time Series Classification Using SAX and Vector Space Model.
//' Data Mining (ICDM), 2013 IEEE 13th International Conference on, pp.1175,1180, 7-10 Dec. 2013.
//' @references Salton, G., Wong, A., Yang., C.,
//' A vector space model for automatic indexing. Commun. ACM 18, 11, 613-620, 1975.
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
      series_to_string(subSection, a_size));

    // Rcout << curr_str << "\n";

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

//' Converts a set of time-series into a single bag of words.
//'
//' @param data the timeseries data, row-wise.
//' @param w_size the sliding window size.
//' @param paa_size the PAA size.
//' @param a_size the alphabet size.
//' @param nr_strategy the NR strategy.
//' @param n_threshold the normalization threshold.
//' @useDynLib jmotif
//' @export
//' @references Senin Pavel and Malinchik Sergey,
//' SAX-VSM: Interpretable Time Series Classification Using SAX and Vector Space Model.
//' Data Mining (ICDM), 2013 IEEE 13th International Conference on, pp.1175,1180, 7-10 Dec. 2013.
//' @references Salton, G., Wong, A., Yang., C.,
//' A vector space model for automatic indexing. Commun. ACM 18, 11, 613-620, 1975.
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
      std::string curr_str = Rcpp::as<std::string>(series_to_string(subSection, a_size));

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

//' Computes a TF-IDF weight vectors for a set of word bags.
//'
//' @param data the list containing the input word bags.
//' @useDynLib jmotif
//' @export
//' @references Senin Pavel and Malinchik Sergey,
//' SAX-VSM: Interpretable Time Series Classification Using SAX and Vector Space Model.
//' Data Mining (ICDM), 2013 IEEE 13th International Conference on, pp.1175,1180, 7-10 Dec. 2013.
//' @references Salton, G., Wong, A., Yang., C.,
//' A vector space model for automatic indexing. Commun. ACM 18, 11, 613-620, 1975.
//' @examples
//' bag1 = data.frame(
//'    "words" = c("this", "is", "a", "sample"),
//'    "counts" = c(1, 1, 2, 1),
//'    stringsAsFactors = FALSE
//'    )
//' bag2 = data.frame(
//'    "words" = c("this", "is", "another", "example"),
//'    "counts" = c(1, 1, 2, 3),
//'    stringsAsFactors = FALSE
//'    )
//' ll = list("bag1" = bag1, "bag2" = bag2)
//' tfidf = bags_to_tfidf(ll)
// [[Rcpp::export]]
Rcpp::DataFrame bags_to_tfidf(Rcpp::List data) {

  // the classes labels
  std::vector<std::string> class_names =
           Rcpp::as< std::vector<std::string> > (data.names());

  // iterate over the list elements building the global word entry count matrix
  //
  std::map<std::string, std::vector<int> > counts;
  for(int i = 0; i< class_names.size(); i++) {

    // get a current class' wordbag
    std::string current_class_name = class_names[i];
    Rcpp::DataFrame df = (Rcpp::DataFrame) data[current_class_name];

    // its words and counts
    std::vector<std::string> bag_words = Rcpp::as< std::vector<std::string> > (df["words"]);
    std::vector<int> bag_counts = Rcpp::as< std::vector<int> > (df["counts"]);

    // iterate over words
    for(int j=0; j<bag_words.size(); j++) {

      // cirrent word and its count
      std::string curr_word = bag_words[j];
      int curr_count = bag_counts[j];

      // try to find the word in the res map
      std::map<std::string,std::vector<int> >::iterator entry_it = counts.find(curr_word);
      if (entry_it == counts.end()){
        // if not found, create one
        std::vector<int> empty_counts(class_names.size());
        empty_counts[i] = curr_count;
        char * new_key = new char [curr_word.size()+1];
        std::copy(curr_word.begin(), curr_word.end(), new_key);
        new_key[curr_word.size()] = '\0';
        counts.insert(std::make_pair(new_key, empty_counts));
      } else {
        // if it was found, update the count array
        std::vector<int> entry = entry_it->second;
        entry[i] = entry[i] + curr_count;
        entry_it->second = entry;
      }

    }
  }

  // count the amount on non-zero entries in this table
  int non_zero_entries = 0;
  for(std::map<std::string, std::vector<int> >::iterator it = counts.begin();
                                                        it != counts.end(); ++it) {
    std::string e_key = it->first;
    std::vector<int> e_counts = it->second;
    int docs_with_word = 0;
    for(int k=0; k<e_counts.size(); k++){
      if(e_counts[k] > 0){
        docs_with_word++;
      }
    }
    if(docs_with_word == e_counts.size()){
      continue;
    }else{
      non_zero_entries++;
    }
  }

  // RESULT: the words in tfidf table
  //
  std::vector<std::string> res_words(non_zero_entries);

  // RESULT: the tfidf coefficients data structure
  //
  std::map<std::string, NumericVector > tfidf;
  for(int k=0;k<class_names.size();k++){
    char * class_name_copy = new char [class_names[k].size()+1];
    std::copy(class_names[k].begin(), class_names[k].end(), class_name_copy);
    class_name_copy[class_names[k].size()] = '\0';
    NumericVector new_values(non_zero_entries);
    tfidf.insert(std::make_pair(class_name_copy, new_values));
  }

  int counter = 0;
  for(std::map<std::string, std::vector<int> >::iterator it = counts.begin();
                                                        it != counts.end(); ++it) {
    // get the word and counts
    std::string e_key = it->first;
    std::vector<int> e_counts = it->second;

    // count docs which contain that word
    int docs_with_word = 0;
    for(int k=0; k<e_counts.size(); k++){
      if(e_counts[k] > 0){
        docs_with_word++;
      }
    }
    if(docs_with_word == e_counts.size()){
      continue;
    }

    // copy the word for the new key
    char * word = new char [e_key.size()+1];
    std::copy(e_key.begin(), e_key.end(), word);
    word[e_key.size()] = '\0';
    res_words[counter] = word;

    // compute the tfidf for each of the elements
    for(int k=0; k<e_counts.size(); k++){
      if(e_counts[k] != 0){
        double tf = log(1.0 + (double) e_counts[k]);
        double idf = log( (double) e_counts.size() / (double) docs_with_word);
        NumericVector vector = tfidf[class_names[k]];
        vector[counter] = tf * idf;
      }
    }

    counter++;
  }

  // build a data structure to return
  //
  Rcpp::List pre_res(class_names.size() + 1);

  // firs, all the words
  pre_res[0] = res_words;

  // second, all the counts
  for(int k=0;k<class_names.size();k++){
    pre_res[1+k] = tfidf[class_names[k]];
  }

  // attach names to the resulting data structure
  CharacterVector df_names(class_names.size() + 1);
  df_names[0] = "words";
  for(int k=0;k<class_names.size();k++){
    char * class_name_copy = new char [class_names[k].size()+1];
    std::copy(class_names[k].begin(), class_names[k].end(), class_name_copy);
    class_name_copy[class_names[k].size()] = '\0';
    df_names[k+1] = class_name_copy;
  }

  pre_res.names() = df_names;

  // and return the results
  DataFrame df = Rcpp::DataFrame::create(pre_res, Rcpp::Named("stringsAsFactors")=false);

  return df;
}

//' Computes a cosine similarity values between a bag of words and a set of
//' TF-IDF weight vectors.
//'
//' @param data the list containing a word-bag and the TF-IDF object.
//' @useDynLib jmotif
//' @export
//' @references Senin Pavel and Malinchik Sergey,
//' SAX-VSM: Interpretable Time Series Classification Using SAX and Vector Space Model.
//' Data Mining (ICDM), 2013 IEEE 13th International Conference on, pp.1175,1180, 7-10 Dec. 2013.
//' @references Salton, G., Wong, A., Yang., C.,
//' A vector space model for automatic indexing. Commun. ACM 18, 11, 613-620, 1975.
// [[Rcpp::export]]
Rcpp::DataFrame cosine_sim(Rcpp::List data) {

  DataFrame bag_df = (Rcpp::DataFrame) data["bag"];

  std::vector<std::string> bag_words = Rcpp::as< std::vector<std::string> > (bag_df["words"]);
  std::vector<int> bag_counts = Rcpp::as< std::vector<int> > (bag_df["counts"]);
  // .. Rcout << bag_words.size() << ": " << bag_counts.size() << "\n";

  std::map< std::string, int > bag;
   for(int i=0;i<bag_words.size();i++){

     // Rcout << i << ": " << bag_words[i] << "\n";

     std::string e_key = bag_words[i];
     char * new_key = new char [e_key.size()+1];
     std::copy(e_key.begin(), e_key.end(), new_key);
     new_key[e_key.size()] = '\0';

     // Rcout << new_key << " : " <<  bag_counts[i] <<"\n";

     bag.insert( std::make_pair(new_key, bag_counts[i]) );
   }

  DataFrame tfidf = (Rcpp::DataFrame) data["tfidf"];
  std::vector<std::string> tfidf_words = Rcpp::as< std::vector<std::string> > (tfidf["words"]);
  std::vector<std::string> tfidf_classes = Rcpp::as< std::vector<std::string> > (tfidf.names());

//   Rcout << "bag of " << bag.size() << " words, ";
//   Rcout << "and TFIDF matrix of " << tfidf_words.size() << " words, ";
//   Rcout << "and " << (tfidf_classes.size()-1) << " classes\n";

  // align the wordbag vector to tfidf one
  NumericVector wordbag_counts(tfidf_words.size());
  for(int i=0; i<tfidf_words.size(); i++){

    std::string curr_word = tfidf_words[i];

    std::map<std::string, int >::iterator entry_it = bag.find(curr_word);
    if (entry_it == bag.end()){
      wordbag_counts[i] = 0.0;
    } else {
      wordbag_counts[i] = (double) entry_it -> second;
    }

  }

  // computing cosines
  NumericVector cosines(tfidf_classes.size()-1);
  std::vector<std::string> class_names(tfidf_classes.size()-1);
  for(int i=1; i<tfidf_classes.size(); i++){

    //class_names[i] = tfidf_classes[i];
    std::string e_key = tfidf_classes[i];
    char * new_key = new char [e_key.size()+1];
    std::copy(e_key.begin(), e_key.end(), new_key);
    new_key[e_key.size()] = '\0';
    class_names[i-1] = new_key;
    // Rcout << new_key << "\n";

    NumericVector tfidf_values = tfidf[new_key];
    // std::vector<double> tfidf_values = Rcpp::as< std::vector<double> > (tfidf[new_key]);
    // Rcout << tfidf_values.size() << "\n";
    double norm_a_squared = 0;
    double norm_b_squared = 0;
    double sum_a_dot_b = 0;
    for(int j=0;j<wordbag_counts.size();j++){
      double a = (double) wordbag_counts[j];
      double b = tfidf_values[j];
      sum_a_dot_b = sum_a_dot_b + a * b;
      norm_a_squared = norm_a_squared + a * a;
      norm_b_squared = norm_b_squared + b * b;
    }
    cosines[i-1] = sum_a_dot_b / ( sqrt(norm_a_squared) * sqrt(norm_b_squared) );
  }

  // make results
  return Rcpp::DataFrame::create(
    Named("classes") = class_names,
    Named("cosines") = cosines,
    Named("stringsAsFactors") = false
    );

}

class VisitRegistry {
public:
  bool* registry;
  int unvisited_count;

  VisitRegistry( int capacity ) {
    registry = new bool[capacity];
    for( int i = 0; i < capacity; i++ ) {
      registry[i] = false;
    }
    unvisited_count = capacity;
    std::srand(std::time(0)); // use current time as seed for random generator
  }

  ~VisitRegistry() {
    delete[] registry;
  }

  int getNextUnvisited(){
    if(0 == unvisited_count){
      return -1;
    } else {
      int random_index = -1;
      do{
        random_index = std::rand();
      } while ( !(registry[random_index]) );
      return random_index;
    }
  }

  void markVisited(int start, int end){
    for(int i=start; i<end; i++){
      if(registry[i]){
        continue;
      }else{
        --unvisited_count;
        registry[i] = true;
      }
    }
  }
};

//' Finds a discord using brute force algorithm.
//'
//' @param ts the input timeseries.
//' @param w_size the sliding window size.
//' @param discords_num the number of discords to report.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
std::map<int, double> get_discords_brute_force(
    NumericVector ts, int w_size, int discords_num) {

  std::map<int, double> res;

  VisitRegistry registry(ts.length());



  return res;
}
