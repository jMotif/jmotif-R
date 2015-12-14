#include <jmotif.h>
//
#include <limits>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
//
// [[Rcpp::depends(RcppArmadillo)]]
#include <RcppArmadillo.h>
//
// Enable C++11 via this plugin (Rcpp 0.10.3 or later)
// [[Rcpp::plugins("cpp11")]]
using namespace Rcpp;
//

// flipping the map
//
//template<typename A, typename B>
//std::pair<B,A> flip_pair(const std::pair<A,B> &p)
//{
//  return std::pair<B,A>(p.second, p.first);
//}

//template<typename A, typename B>
//std::multimap<B,A> flip_map(const std::map<A,B> &src)
//{
//  std::multimap<B,A> dst;
//  std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
//                 flip_pair<A,B>);
//  return dst;
//}
