#include <RcppArmadillo.h>
using namespace Rcpp ;
//
#include <jmotif.h>
//

VisitRegistry::VisitRegistry( int capacity ) {
  registry = new bool[capacity];
  for( int i = 0; i < capacity; i++ ) {
    registry[i] = false;
  }
  unvisited_count = capacity;
  size = capacity;
  // std::srand(std::time(0)); // use current time as seed for random generator
}

VisitRegistry::~VisitRegistry() {
  delete[] registry;
}

int VisitRegistry::getNextUnvisited(){
  if(0 == unvisited_count){
    return -1;
  } else {
    int random_index = -1;
    do{
      // random_index = std::rand() % size;
      random_index = armaRand() % size;
    } while ( registry[random_index] );
    return random_index;
  }
}

void VisitRegistry::markVisited(int idx){
  if(registry[idx]){
    return;
  }else{
    unvisited_count = unvisited_count - 1;
    registry[idx] = true;
  }
}

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

bool VisitRegistry::isVisited(int idx){
  return(registry[idx]);
}
