#include <RcppArmadillo.h>
using namespace Rcpp;
//
#include <jmotif.h>
//

// constructor
//
VisitRegistry::VisitRegistry( int capacity ) {
  registry = new bool[capacity];
  for( int i = 0; i < capacity; i++ ) {
    registry[i] = false;
  }
  unvisited_count = capacity;
  size = capacity;
}

// destructor
//
VisitRegistry::~VisitRegistry() {
  delete[] registry;
}

// gets next unvisited position... following the HOTSAX heuristics this need to be a random
// position, so we can abandon the search earlier, if possible
//
int VisitRegistry::getNextUnvisited(){
  if(0 == unvisited_count){
    return -1;
  } else {
    int random_index = -1;
    do{ // iterate over random indexes until we hit an unvisited position
      random_index = armaRand() % size;
    } while ( registry[random_index] );
    return random_index;
  }
}

// marks a position visited, takes care about the counter
//
void VisitRegistry::markVisited(int idx){
  if(registry[idx]){
    return;
  }else{
    unvisited_count = unvisited_count - 1;
    registry[idx] = true;
  }
}

// marks an interval as visited
//
void VisitRegistry::markVisited(int start, int end){
  for(int i=start; i<end; i++){
    if(registry[i]){
      continue;
    }else{
      unvisited_count = unvisited_count - 1;
      registry[i] = true;
    }
  }
}

// check if the position has been marked as visited
//
bool VisitRegistry::isVisited(int idx){
  return(registry[idx]);
}
