#include <jmotif.h>
using namespace Rcpp ;

repair_digram* repair_priority_queue::enqueue(repair_digram* digram) {

  if (nodes.find(digram->digram) != nodes.end()) {
    throw std::range_error("Inadmissible value, key exists...");
    return nullptr;
  }

  // Rcout << "enqueueing " << digram ->digram << " : " << digram ->freq << std::endl;
  // create a new node
  repair_pqueue_node* nn = new repair_pqueue_node(digram);

  // place it into the queue if it's empty
  if (nullptr == queue_head) {
      queue_head = nn;
//       Rcout << "queue was empty, the new node is the head: " << queue_head->payload->digram
//             << " : " << queue_head->payload ->freq << std::endl;
  }

  // if new node has lesser or equal than head frequency... this going to be the new head
  else if (nn->payload->freq >= queue_head->payload->freq) {
    queue_head->prev = nn;
    nn->next = queue_head;
    queue_head = nn;
//     Rcout << "the nn freq is greater than or equal to head, re-headed the queue: "
//     << queue_head->payload->digram << " : " << queue_head->payload ->freq << std::endl;
  }

  // in all other cases find a good place in the existing queue, starting from the head
  else {
    repair_pqueue_node* curr_node = queue_head;
    while (nullptr != curr_node->next) {
      //
      // if current node has lesser or equal frequency... it should be after nn
      //
      // remember we are pointing on the second node inside this loop or onto second to tail
      // node
      //
      if (nn->payload->freq >= curr_node->payload->freq) {
//           Rcout << "found a place for nn before: "
//                 << curr_node->payload->digram << " : " << curr_node->payload->freq << std::endl;
          repair_pqueue_node* prev_node = curr_node->prev;
//           Rcout << " after : "
//                 << prev_node->payload->digram << " : " << prev_node->payload->freq << std::endl;
          prev_node->next = nn;
          nn->prev = prev_node;
          curr_node->prev = nn;
          nn->next = curr_node;
          break; // the element has been placed
        }
        curr_node = curr_node->next;
      }
      // check if loop was broken by condition, not by placement
      if (nullptr == curr_node->next) {
        // so, current_node points on the tail...
        if (nn->payload->freq >= curr_node->payload->freq) {
          // insert just before...
          repair_pqueue_node* prev_node = curr_node->prev;
//           Rcout << "found a place for nn before the tail: "
//                 << curr_node->payload->digram << " : " << curr_node->payload->freq << std::endl;
          prev_node->next = nn;
          nn->prev = prev_node;
          curr_node->prev = nn;
          nn->next = curr_node;
        }
        else {
          // or make a new tail
//           Rcout << "found a place for nn -- new tail after: "
//                 << curr_node->payload->digram << " : " << curr_node->payload->freq << std::endl;
          nn->prev = curr_node;
          curr_node->next = nn;
        }
      }

  }
  // also save the element in the index store
  std::string key = nn->payload->digram;
  nodes.emplace(key, nn);
  return nn->payload;
}

void repair_priority_queue::remove_node(repair_pqueue_node* node){
  // the head case
  //
  if(nullptr == node->prev){
    if(nullptr != node->next){
      queue_head = node->next;
      queue_head->prev = nullptr;
    } else {
      queue_head = nullptr;
    }
  }
  // the tail case
  //
  else if (nullptr == node->next){
    if(nullptr != node->prev){
      node->prev->next = nullptr;
    }
  }
  // all other cases
  //
  else{
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }
};

repair_digram* repair_priority_queue::update_digram_frequency(
    std::string *digram_string, int new_value){

  // if element doesnt exist
  if (nodes.find(*digram_string) == nodes.end()) {
    return nullptr;
  }

  // get a pointer on that node
  repair_pqueue_node* altered_node = nodes.find(*digram_string)->second;
//   Rcout << " +++ correcting the digram frequency for " << altered_node->payload->digram <<
//     " from " << altered_node->payload->freq << " to " << new_value << std::endl;

  // the trivial case
  if (new_value == altered_node->payload->freq) {
    // Rcout << " +++ new value is same as the old, exiting... " << std::endl;
    return altered_node->payload;
  }

  // simply evict the node if the freq is too low
  if (2 > new_value) {
    // Rcout << " +++ new value is less than 2, cleaning up the entry... " << std::endl;
    remove_node(altered_node);
    nodes.erase(altered_node->payload->digram);
    return nullptr;
  }

  // update the frequency
  int oldFreq = altered_node->payload->freq;
  altered_node->payload->freq = new_value;

  // if the list is just too damn short
  if (1 == nodes.size()) {
    // Rcout << " +++ queue is a single element, just updating..." << std::endl;
    return altered_node->payload;
  }

  // if we have to push the element up in the list
  if (new_value > oldFreq) {

    // going up here
    repair_pqueue_node* current_node = altered_node->prev;
    if (nullptr == altered_node->prev) {
      current_node = altered_node->next;
    }

    remove_node(altered_node);
    altered_node->next = nullptr;
    altered_node->prev = nullptr;

    while ((nullptr != current_node) &&
           (current_node->payload->freq < altered_node->payload->freq)) {
      current_node = current_node->prev;
    }

    // we hit the head, oops... make it the new head
    if (nullptr == current_node) {
      altered_node->next = queue_head;
      queue_head->prev = altered_node;
      queue_head = altered_node;
    }
    else {
      if (nullptr == current_node->next) {
        current_node->next = altered_node;
        altered_node->prev = current_node;
      }
      else {
        current_node->next->prev = altered_node;
        altered_node->next = current_node->next;
        current_node->next = altered_node;
        altered_node->prev = current_node;
      }
    }
  }
  else {

    // what if this is a tail already?
    if (nullptr == altered_node->next) {
      return altered_node->payload;
    }

    // going down..
    repair_pqueue_node* current_node = altered_node->next;
    remove_node(altered_node);
    altered_node->next = nullptr;
    altered_node->prev = nullptr;

    while (nullptr != current_node->next &&
           current_node->payload->freq > altered_node->payload->freq) {
      current_node = current_node->next;
    }

    if (nullptr == current_node->next) { // we hit the tail
      if (altered_node->payload->freq > current_node->payload->freq) {
        // place before tail
        if (queue_head == current_node) {
          altered_node->next = current_node;
          current_node->prev = altered_node;
          this->queue_head = altered_node;
        }
        else {
          altered_node->next = current_node;
          altered_node->prev = current_node->prev;
          current_node->prev->next = altered_node;
          current_node->prev = altered_node;
        }
      }
      else {
        current_node->next = altered_node;
        altered_node->prev = current_node;
      }
    }
    else { // place element just before of cp
      altered_node->next = current_node;
      altered_node->prev = current_node->prev;
      if (nullptr == current_node->prev) {
        // i.e. we are in da queue_head...
        this->queue_head = altered_node;
      }
      else {
        current_node->prev->next = altered_node;
        current_node->prev = altered_node;
      }
    }
  }

  return altered_node->payload;

}

repair_digram* repair_priority_queue::dequeue() {
  if(nullptr != queue_head){
    nodes.erase(queue_head->payload->digram);
    repair_pqueue_node* res = queue_head;
    queue_head = queue_head->next;
    if(nullptr != queue_head){
      queue_head->prev = nullptr;
    }
    return res->payload;
  }
  return nullptr;
}

std::vector<repair_digram> repair_priority_queue::to_array() {
  std::vector<repair_digram> res;
  repair_pqueue_node* cp = queue_head;
  while(NULL != cp->next){
    res.push_back(*cp->payload);
  }
  return res;
}

std::string repair_priority_queue::to_string() {
  std::stringstream res;
  repair_pqueue_node* ptr = queue_head;
  while(nullptr != ptr) {
    res << ptr->payload->digram << " : " << ptr->payload->freq << std::endl;
    ptr = ptr->next;
  }
  std::string str = res.str();
  return str;
 }

bool repair_priority_queue::contains_digram(std::string *digram_string) {
  if (nodes.find(*digram_string) == nodes.end()) {
    return false;
  }
  return true;
}

