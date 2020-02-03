#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
//

//' Finds the Euclidean distance between points.
//'
//' @param seq1 the array 1.
//' @param seq2 the array 2.
//' stops and the NAN is returned.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
double euclidean_dist(NumericVector seq1, NumericVector seq2) {
  if(seq1.length() == seq2.length()){
    double res = 0.0;
    for(int i=0; i<seq1.length(); i++){
      res = res + (seq1[i]-seq2[i])*(seq1[i]-seq2[i]);
    }
    return sqrt(res);
  } else {
    stop("arrays length are not equal");
    return std::numeric_limits<double>::quiet_NaN();
  }
}

// this a faster version for the internal use
//
double _euclidean_dist(std::vector<double>* seq1, std::vector<double>* seq2) {
  double res = 0;
  for(unsigned i=0; i<seq1->size(); i++){
    res = res + (seq1->at(i) - seq2->at(i)) * (seq1->at(i) - seq2->at(i));
  }
  return sqrt(res);
}

//' Finds the Euclidean distance between points, if distance is above the threshold, abandons the computation
//' and returns NAN.
//'
//' @param seq1 the array 1.
//' @param seq2 the array 2.
//' @param upper_limit the max value after reaching which the distance computation
//' stops and the NAN is returned.
//' @useDynLib jmotif
//' @export
// [[Rcpp::export]]
double early_abandoned_dist(NumericVector seq1, NumericVector seq2, double upper_limit) {
  if(seq1.length() == seq2.length()){

    double limit = upper_limit;
    if(limit != std::numeric_limits<double>::max()){
      limit = upper_limit * upper_limit;
    }
    double res = 0.0;
    for(int i=0; i<seq1.length(); i++){
      res = res + (seq1[i]-seq2[i])*(seq1[i]-seq2[i]);
      //Rcout << "res: " <<res <<" limit: " << limit <<"\n";
      if(res > limit){
        return std::numeric_limits<double>::quiet_NaN();
      }
    }
    return sqrt(res);
  } else {
    stop("arrays length are not equal");
    return std::numeric_limits<double>::quiet_NaN();
  }
}
