#include <jmotif.h>

std::string repair_rule::get_rule_string() {
  return "R" + std::to_string(id);
}
