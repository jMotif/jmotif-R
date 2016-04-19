#include <jmotif.h>
#include <Rcpp.h>
using namespace Rcpp;

std::string repair_rule::get_rule_string() {
  return "R" + std::to_string(id);
}
