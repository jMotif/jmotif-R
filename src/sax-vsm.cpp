#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
//

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

  std::unordered_map<int, std::string> sax_map = _sax_via_window(
    Rcpp::as<std::vector<double>>(ts), w_size, paa_size, a_size,
    Rcpp::as<std::string>(nr_strategy), n_threshold);

  // Rcout << "sax done" << "\n";

  std::map<std::string, int> word_bag;
  for(std::unordered_map<int, std::string>::iterator it = sax_map.begin();
      it != sax_map.end(); ++it) {
    if (word_bag.find(it->second) == word_bag.end()){
      word_bag.insert(std::make_pair(it->second, 1));
    }else{
      word_bag[it->second] += 1;
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

    std::unordered_map<int, std::string> sax_map = _sax_via_window(
      Rcpp::as<std::vector<double>>(ts), w_size, paa_size, a_size,
      Rcpp::as<std::string>(nr_strategy), n_threshold);

    for(std::unordered_map<int, std::string>::iterator it = sax_map.begin();
        it != sax_map.end(); ++it) {
      if (word_bag.find(it->second) == word_bag.end()){
        word_bag.insert(std::make_pair(it->second, 1));
      }else{
        word_bag[it->second] += 1;
      }
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
  for(unsigned i = 0; i < class_names.size(); i++) {

    // get a current class' wordbag
    std::string current_class_name = class_names[i];
    Rcpp::DataFrame df = (Rcpp::DataFrame) data[current_class_name];

    // its words and counts
    std::vector<std::string> bag_words = Rcpp::as< std::vector<std::string> > (df["words"]);
    std::vector<int> bag_counts = Rcpp::as< std::vector<int> > (df["counts"]);

    // iterate over words
    for(unsigned j=0; j<bag_words.size(); j++) {

      // cirrent word and its count
      std::string curr_word = bag_words[j];
      int curr_count = bag_counts[j];

      // try to find the word in the res map
      std::map<std::string,std::vector<int> >::iterator entry_it = counts.find(curr_word);
      if (entry_it == counts.end()){
        // if not found, create one
        std::vector<int> empty_counts(class_names.size());
        empty_counts[i] = curr_count;
        std::string new_key = curr_word;
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
    unsigned docs_with_word = 0;
    for(unsigned k=0; k<e_counts.size(); k++){
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
  for(unsigned k=0;k<class_names.size();k++){
    std::string class_name_copy = class_names[k];
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
    unsigned docs_with_word = 0;
    for(unsigned k=0; k<e_counts.size(); k++){
      if(e_counts[k] > 0){
        docs_with_word++;
      }
    }
    if(docs_with_word == e_counts.size()){
      continue;
    }

    // copy the word for the new key
    std::string word = e_key;
    res_words[counter] = word;

    // compute the tfidf for each of the elements
    for(unsigned k=0; k<e_counts.size(); k++){
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
  for(unsigned k=0;k<class_names.size();k++){
    pre_res[1+k] = tfidf[class_names[k]];
  }

  // attach names to the resulting data structure
  CharacterVector df_names(class_names.size() + 1);
  df_names[0] = "words";
  for(unsigned k=0;k<class_names.size();k++){
    std::string class_name_copy = class_names[k];
    df_names[k+1] = class_name_copy;
  }

  pre_res.names() = df_names;

  tfidf.clear();

  // and return the results
  DataFrame df = Rcpp::DataFrame::create(pre_res, Rcpp::Named("stringsAsFactors")=false);

  return df;
}

//' Computes the cosine distance value between a bag of words and a set of TF-IDF weight vectors.
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
  for(unsigned i=0;i<bag_words.size();i++){

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
  for(unsigned i=0; i<tfidf_words.size(); i++){

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
  for(unsigned i=1; i<tfidf_classes.size(); i++){

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
