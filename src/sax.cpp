#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
#include <memory>
#include <iostream>
#include <string>
#include <cstdio>
#include <unordered_map>
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
  case 3: {return NumericVector::create(R_NegInf, -0.4307273, 0.4307273);}
  case 4: {return NumericVector::create(R_NegInf, -0.6744898, 0, 0.6744898);}
  case 5: {return NumericVector::create(R_NegInf, -0.841621233572914, -0.2533471031358,
        0.2533471031358, 0.841621233572914);}
  case 6: {return NumericVector::create(R_NegInf, -0.967421566101701, -0.430727299295457, 0,
        0.430727299295457, 0.967421566101701);}
  case 7: {return NumericVector::create(R_NegInf, -1.06757052387814, -0.565948821932863,
        -0.180012369792705, 0.180012369792705, 0.565948821932863, 1.06757052387814);}
  case 8: {return NumericVector::create(R_NegInf, -1.15034938037601, -0.674489750196082, -0.318639363964375,
        0, 0.318639363964375, 0.674489750196082, 1.15034938037601);}
  case 9: {return NumericVector::create(R_NegInf, -1.22064034884735, -0.764709673786387, -0.430727299295457,
        -0.139710298881862, 0.139710298881862, 0.430727299295457, 0.764709673786387,
        1.22064034884735);}
  case 10: {return NumericVector::create(R_NegInf, -1.2815515655446, -0.841621233572914, -0.524400512708041,
         -0.2533471031358, 0, 0.2533471031358, 0.524400512708041, 0.841621233572914, 1.2815515655446);}
  case 11: {return NumericVector::create(R_NegInf, -1.33517773611894, -0.908457868537385,
         -0.604585346583237, -0.348755695517045, -0.114185294321428, 0.114185294321428,
         0.348755695517045, 0.604585346583237, 0.908457868537385, 1.33517773611894);}
  case 12: {return NumericVector::create(R_NegInf, -1.38299412710064, -0.967421566101701,
         -0.674489750196082, -0.430727299295457, -0.210428394247925, 0, 0.210428394247925,
         0.430727299295457, 0.674489750196082, 0.967421566101701, 1.38299412710064);}
  case 13: {return NumericVector::create(R_NegInf, -1.42607687227285, -1.0200762327862, -0.736315917376129,
         -0.502402223373355, -0.293381232121193, -0.0965586152896391, 0.0965586152896394,
         0.293381232121194, 0.502402223373355, 0.73631591737613, 1.0200762327862, 1.42607687227285);}
  case 14: {return NumericVector::create(R_NegInf, -1.46523379268552, -1.06757052387814, -0.791638607743375,
         -0.565948821932863, -0.36610635680057, -0.180012369792705, 0, 0.180012369792705,
         0.36610635680057, 0.565948821932863, 0.791638607743375, 1.06757052387814, 1.46523379268552);}
  case 15: {return NumericVector::create(R_NegInf, -1.50108594604402, -1.11077161663679, -0.841621233572914,
         -0.622925723210088, -0.430727299295457, -0.2533471031358, -0.0836517339071291,
         0.0836517339071291, 0.2533471031358, 0.430727299295457, 0.622925723210088, 0.841621233572914,
         1.11077161663679, 1.50108594604402);}
  case 16: {return NumericVector::create(R_NegInf, -1.53412054435255, -1.15034938037601, -0.887146559018876,
         -0.674489750196082, -0.488776411114669, -0.318639363964375, -0.157310684610171, 0,
         0.157310684610171, 0.318639363964375, 0.488776411114669, 0.674489750196082, 0.887146559018876,
         1.15034938037601, 1.53412054435255);}
  case 17: {return NumericVector::create(R_NegInf, -1.5647264713618, -1.18683143275582, -0.928899491647271,
         -0.721522283982343, -0.541395085129088, -0.377391943828554, -0.223007830940367,
         -0.0737912738082727, 0.0737912738082727, 0.223007830940367, 0.377391943828554,
         0.541395085129088, 0.721522283982343, 0.928899491647271, 1.18683143275582, 1.5647264713618);}
  case 18: {return NumericVector::create(R_NegInf, -1.59321881802305, -1.22064034884735, -0.967421566101701,
         -0.764709673786387, -0.589455797849779, -0.430727299295457, -0.282216147062508,
         -0.139710298881862, 0, 0.139710298881862, 0.282216147062508, 0.430727299295457,
         0.589455797849779, 0.764709673786387, 0.967421566101701, 1.22064034884735, 1.59321881802305);}
  case 19: {return NumericVector::create(R_NegInf, -1.61985625863827, -1.25211952026522, -1.00314796766253,
         -0.8045963803603, -0.633640000779701, -0.47950565333095, -0.336038140371823,
         -0.199201324789267, -0.0660118123758407, 0.0660118123758406, 0.199201324789267,
         0.336038140371823, 0.47950565333095, 0.633640000779701, 0.8045963803603, 1.00314796766253,
         1.25211952026522, 1.61985625863827);}
  case 20: {return NumericVector::create(R_NegInf, -1.64485362695147, -1.2815515655446, -1.03643338949379,
         -0.841621233572914, -0.674489750196082, -0.524400512708041, -0.385320466407568,
         -0.2533471031358, -0.125661346855074, 0, 0.125661346855074, 0.2533471031358,
         0.385320466407568, 0.524400512708041, 0.674489750196082, 0.841621233572914, 1.03643338949379,
         1.2815515655446, 1.64485362695147);}
  default: { stop("'a_size' is invalid"); return NumericVector::create(0.0); }
  }
  return NumericVector::create(R_NegInf, R_PosInf);
}

std::vector<double> _alphabet_to_cuts(int a_size) {
  switch(a_size){
  case 2: {return std::vector<double> {std::numeric_limits<double>::lowest(),  0.00};}
  case 3: {return std::vector<double> {std::numeric_limits<double>::lowest(), -0.4307273, 0.4307273};}
  case 4: {return std::vector<double> {std::numeric_limits<double>::lowest(), -0.6744898, 0, 0.6744898};}
  case 5: {return std::vector<double> {std::numeric_limits<double>::lowest(), -0.841621233572914,
                                       -0.2533471031358, 0.2533471031358, 0.841621233572914};}
  case 6: {return std::vector<double> {std::numeric_limits<double>::lowest(), -0.967421566101701,
                                       -0.430727299295457, 0, 0.430727299295457, 0.967421566101701};}
  case 7: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.06757052387814,
       -0.565948821932863, -0.180012369792705, 0.180012369792705, 0.565948821932863, 1.06757052387814};}
  case 8: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.15034938037601,
                                       -0.674489750196082, -0.318639363964375,
       0, 0.318639363964375, 0.674489750196082, 1.15034938037601};}
  case 9: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.22064034884735,
                                       -0.764709673786387, -0.430727299295457,
       -0.139710298881862, 0.139710298881862, 0.430727299295457, 0.764709673786387,
       1.22064034884735};}
  case 10: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.2815515655446,
                                        -0.841621233572914, -0.524400512708041,
        -0.2533471031358, 0, 0.2533471031358, 0.524400512708041, 0.841621233572914, 1.2815515655446};}
  case 11: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.33517773611894,
                                        -0.908457868537385,
        -0.604585346583237, -0.348755695517045, -0.114185294321428, 0.114185294321428,
        0.348755695517045, 0.604585346583237, 0.908457868537385, 1.33517773611894};}
  case 12: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.38299412710064,
                                        -0.967421566101701,
        -0.674489750196082, -0.430727299295457, -0.210428394247925, 0, 0.210428394247925,
        0.430727299295457, 0.674489750196082, 0.967421566101701, 1.38299412710064};}
  case 13: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.42607687227285,
                                        -1.0200762327862, -0.736315917376129,
        -0.502402223373355, -0.293381232121193, -0.0965586152896391, 0.0965586152896394,
        0.293381232121194, 0.502402223373355, 0.73631591737613, 1.0200762327862, 1.42607687227285};}
  case 14: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.46523379268552,
                                        -1.06757052387814, -0.791638607743375,
        -0.565948821932863, -0.36610635680057, -0.180012369792705, 0, 0.180012369792705,
        0.36610635680057, 0.565948821932863, 0.791638607743375, 1.06757052387814, 1.46523379268552};}
  case 15: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.50108594604402,
                                        -1.11077161663679, -0.841621233572914,
        -0.622925723210088, -0.430727299295457, -0.2533471031358, -0.0836517339071291,
        0.0836517339071291, 0.2533471031358, 0.430727299295457, 0.622925723210088, 0.841621233572914,
        1.11077161663679, 1.50108594604402};}
  case 16: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.53412054435255,
                                        -1.15034938037601, -0.887146559018876,
        -0.674489750196082, -0.488776411114669, -0.318639363964375, -0.157310684610171, 0,
        0.157310684610171, 0.318639363964375, 0.488776411114669, 0.674489750196082, 0.887146559018876,
        1.15034938037601, 1.53412054435255};}
  case 17: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.5647264713618,
                                        -1.18683143275582, -0.928899491647271,
        -0.721522283982343, -0.541395085129088, -0.377391943828554, -0.223007830940367,
        -0.0737912738082727, 0.0737912738082727, 0.223007830940367, 0.377391943828554,
        0.541395085129088, 0.721522283982343, 0.928899491647271, 1.18683143275582, 1.5647264713618};}
  case 18: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.59321881802305,
                                        -1.22064034884735, -0.967421566101701,
        -0.764709673786387, -0.589455797849779, -0.430727299295457, -0.282216147062508,
        -0.139710298881862, 0, 0.139710298881862, 0.282216147062508, 0.430727299295457,
        0.589455797849779, 0.764709673786387, 0.967421566101701, 1.22064034884735, 1.59321881802305};}
  case 19: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.61985625863827,
                                        -1.25211952026522, -1.00314796766253,
        -0.8045963803603, -0.633640000779701, -0.47950565333095, -0.336038140371823,
        -0.199201324789267, -0.0660118123758407, 0.0660118123758406, 0.199201324789267,
        0.336038140371823, 0.47950565333095, 0.633640000779701, 0.8045963803603, 1.00314796766253,
        1.25211952026522, 1.61985625863827};}
  case 20: {return std::vector<double> {std::numeric_limits<double>::lowest(), -1.64485362695147,
                                        -1.2815515655446, -1.03643338949379,
        -0.841621233572914, -0.674489750196082, -0.524400512708041, -0.385320466407568,
        -0.2533471031358, -0.125661346855074, 0, 0.125661346855074, 0.2533471031358,
        0.385320466407568, 0.524400512708041, 0.674489750196082, 0.841621233572914, 1.03643338949379,
        1.2815515655446, 1.64485362695147};}
  default: { stop("'a_size' is invalid"); std::vector<double> {0.0}; }
  }
  return std::vector<double> {std::numeric_limits<double>::lowest(),  std::numeric_limits<double>::max()};
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
    for(unsigned j=0; j<cuts.size(); j++){
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
    for(unsigned j=0; j<cuts.size(); j++){
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
    for(unsigned j=0; j<cuts.size(); j++){
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

std::unordered_map<int, std::string> _sax_via_window(
    std::vector<double> ts, int w_size, int paa_size, int a_size,
    std::string nr_strategy, double n_threshold) {

  std::unordered_map<int, std::string> idx2word;
  bool strategy_exact = is_equal_str("exact", nr_strategy);
  bool strategy_mindist = is_equal_str("mindist", nr_strategy);

  std::string old_str;

  //Rcout << "series length " << ts.size() << ", window " << w_size << std::endl;

  for (unsigned i = 0; i <= ts.size() - w_size; i++) {

    // check if NA is encountered
    int idx = i + w_size - 1;
    if(R_IsNA(ts[idx])) {
      size_t size = std::snprintf(nullptr, 0, "encountered an Na and stopped processing at %i", i + w_size - 1) + 1;
      std::unique_ptr<char[]> buf( new char[ size ] );
      std::snprintf( buf.get(), size, "encountered an Na and stopped processing at %i", i + w_size - 1 );
      Rcpp::warning(
        std::string( buf.get(), buf.get() + size - 1 )
      );
      break;
    }

    // subseries extraction
    // std::vector<double>::const_iterator first = ts.begin() + i;
    // std::vector<double>::const_iterator last = ts.begin() + i + w_size;
    // std::vector<double> subSection(first, last);
    std::vector<double> subSection = _subseries(&ts, i, i + w_size);

    subSection = _znorm(subSection, n_threshold);

    subSection = _paa(subSection, paa_size);

    std::string curr_str = _series_to_string(subSection, a_size);

    if (!(old_str.empty())) {
      if (strategy_exact && old_str==curr_str) {
        continue;
      }
      else if (strategy_mindist && is_equal_mindist(old_str, curr_str) ) {
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
std::map<int, std::string> sax_via_window(
    NumericVector ts, int w_size, int paa_size, int a_size,
    CharacterVector nr_strategy, double n_threshold) {

  try {
    if(ts.length() < w_size) {
      throw std::range_error("sliding window is longer than timeseries");
    }
  } catch(std::exception &ex) {
    forward_exception_to_r(ex);
  }

  std::vector<double> series = Rcpp::as< std::vector<double> > (ts);
  bool strategy_exact = is_equal_str("exact", nr_strategy);
  bool strategy_mindist = is_equal_str("mindist", nr_strategy);

  std::map<int, std::string> idx2word;

  // std::ofstream bw("test_outR.txt");

  std::string old_str("");

  for (unsigned i = 0; i <= series.size() - w_size; i++) {

    // check if NA is encountered
    int idx = i + w_size - 1;
    if(R_IsNA(ts[idx])) {
      size_t size = std::snprintf(nullptr, 0, "encountered an Na and stopped processing at %i", i + w_size - 1) + 1;
      std::unique_ptr<char[]> buf( new char[ size ] );
      std::snprintf( buf.get(), size, "encountered an Na and stopped processing at %i", i + w_size - 1 );
      Rcpp::warning(
        std::string( buf.get(), buf.get() + size - 1 )
      );
      break;
    }

    NumericVector subSection = subseries(ts, i, i + w_size);
    std::vector<double>::const_iterator first = series.begin() + i;
    std::vector<double>::const_iterator last = series.begin() + i + w_size;
    std::vector<double> sub_section(first, last);

    // bw << i << "\t[" ;
    // for (auto i = subSection.begin(); i != subSection.end(); ++i)
    //   bw << *i << ',';
    // bw << "]\t" ;

    sub_section = _znorm(sub_section, n_threshold);

    sub_section = _paa(sub_section, paa_size);
    // bw << "[" ;
    // for (auto i = subSection.begin(); i != subSection.end(); ++i)
    //   bw << *i << ',';
    // bw << "]\t" ;

    std::string curr_str = _series_to_string(sub_section, a_size);
    // bw << curr_str << "\t" ;

    if (!(0 == old_str.length())) {
      if ( strategy_exact
             && old_str == curr_str ) {
        // std::cout << std::endl;
        // bw << "skipped\n" ;
        continue;
      }
      else if (strategy_mindist
                 && is_equal_mindist(old_str, curr_str) ) {
        // std::cout << std::endl;
        continue;
      }
    }

    idx2word.insert(std::make_pair(i, curr_str));

    old_str = curr_str;

    // std::cout << " " << curr_str << std::endl;
    // bw << "kept\n" ;
  }
  // bw.close();
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
