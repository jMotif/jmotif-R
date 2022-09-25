#include <jmotif.h>

// constructor
//
VisitRegistry::VisitRegistry( int capacity ) {

  indexes = std::vector<int>(capacity);
  registry = std::vector<bool>(capacity);

  for( int i = 0; i < capacity; i++ ) {
    indexes[i] = i;
  }

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(std::begin(indexes), std::end(indexes), g);

  unvisited_count = capacity;
  size = capacity;

}

// destructor
//
VisitRegistry::~VisitRegistry() {
  registry = std::vector<bool>();
  indexes = std::vector<int>();
}

// gets next unvisited position... following the HOTSAX heuristics this need to be a random
// position, so we can abandon the search earlier, if possible
//
int VisitRegistry::getNextUnvisited(){
  int random_index = -1;
    do {
      if(indexes.size()<=0){
        return -1;
      }
      random_index = indexes.back();
      indexes.pop_back();
    } while ( registry[random_index] );
    return random_index;
}

// marks a position visited, takes care about the counter
//
void VisitRegistry::markVisited(int idx){
  if(idx>=0 && idx<registry.size()){
    if(registry[idx]){
      return;
    }else{
      unvisited_count = unvisited_count - 1;
      registry[idx] = true;
    }
  }
}

// marks an interval as visited
//
void VisitRegistry::markVisited(int start, int end){
  for(int i=start; i<end; i++){
    if(i>=0 && i<registry.size()){
      if(registry[i]){
        continue;
      }else{
        unvisited_count = unvisited_count - 1;
        registry[i] = true;
      }
    }
  }
}

// check if the position has been marked as visited
//
bool VisitRegistry::isVisited(int idx){
  if(idx>=0 && idx<registry.size()){
    return(registry[idx]);
  }
  return false;
}
