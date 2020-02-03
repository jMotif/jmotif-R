#include <jmotif.h>
using namespace Rcpp ;

// enqueue a new element, if older element's version exists, throws an error
//
repair_digram* repair_priority_queue::enqueue(repair_digram* digram) {

  // check if the the key is already in the table
  if (nodes.find(digram->digram) != nodes.end()) {
    throw std::range_error("Inadmissible value, key exists...");
    return nullptr;
  }
  // Rcout << "  ++ enqueueing " << digram ->digram << " : " << digram ->freq << std::endl;

  // create a new node
  repair_pqueue_node* nn = new repair_pqueue_node(digram);

  // make it the head if the queue is empty
  if (nullptr == queue_head) {
      queue_head = nn;
      // Rcout << "queue was empty, the new node is the head: " << queue_head->payload->digram
      //       << " : " << queue_head->payload ->freq << std::endl;
  }

  // if new node has the frequency value greater than the current head node, shall be the new head
  else if (nn->payload->freq >= queue_head->payload->freq) {
    queue_head->prev = nn;
    nn->next = queue_head;
    queue_head = nn;
    // Rcout << "  ++ the nn freq is greater than or equal to head, re-headed the queue: "
    //       << queue_head->payload->digram << " : " << queue_head->payload ->freq << std::endl;
  }

  // in all other cases find a good place in the existing queue, starting from the head
  else {
    repair_pqueue_node* curr_node = queue_head;

    // it goes to scroll down and look for a place where the curent NN frequency is above the
    // pointer's value
    while (nullptr != curr_node->next) {

      if (nn->payload->freq >= curr_node->payload->freq) {
        repair_pqueue_node* prev_node = curr_node->prev; // at least second el here, since not head
        // Rcout << "  ++ found a place for nn before: "
        //       << curr_node->payload->digram << " : " << curr_node->payload->freq
        //       << ", after: " << prev_node->payload->digram << " : "
        //       << prev_node->payload->freq << std::endl;
        prev_node->next = nn;
        nn->prev = prev_node;
        curr_node->prev = nn;
        nn->next = curr_node;
        break; // the element has been placed -- break out of the loop
      }

      curr_node = curr_node->next;
    }

    // check if the above loop was broken by condition, not by the node placement
    if (nullptr == curr_node->next) {
      // so, current_node points on the tail... lets see

      if (nn->payload->freq >= curr_node->payload->freq) {
        // insert just before... sorta repeating the code above, just wont loose the tail
        repair_pqueue_node* prev_node = curr_node->prev;
        // Rcout << "  ++ found a place for nn before the tail: "
        //       << curr_node->payload->digram << " : " << curr_node->payload->freq
        //       << ", after: " << prev_node->payload->digram << " : "
        //       << prev_node->payload->freq << std::endl;
        prev_node->next = nn;
        nn->prev = prev_node;
        curr_node->prev = nn;
        nn->next = curr_node;
      }

      else {
        // nn shall be the new tail
        // Rcout << "  ++ found a place for nn in the tail after: "
        //       << curr_node->payload->digram << " : " << curr_node->payload->freq << std::endl;
        nn->prev = curr_node;
        curr_node->next = nn;
      }

    }

  }

  // also save the element in the index store
  std::string key = digram->digram;
  nodes.emplace(key, nn);

  // consistency_check();

  return nn->payload;
}

//
// removes a specific digram off the queue
//
void repair_priority_queue::remove_node(repair_pqueue_node* node){
  // Rcout << "  ++ removing the node " << node->payload->digram << std::endl;

  // clean-up the table first
  nodes.erase(node->payload->digram);

  // the head case
  if(nullptr == node->prev){
    // Rcout << "  ++ it was a head...";
    // case if the queue size > 1
    if(nullptr != node->next){
      queue_head = node->next;
      queue_head->prev = nullptr;
      // Rcout << "  ++ moved the head to " << queue_head->payload->digram << "\n";
    }
    // case when there is just a single element
    else {
      queue_head = nullptr;
    }
  }

  // the tail case
  else if (nullptr == node->next){
    // Rcout << "  ++ it was a tail...";
    // obviously it is not the head too...
    node->prev->next = nullptr;
  }

  // all other cases, assumption is that prev and next are not nulls,
  // since we checked for these cases just above
  else{
    // Rcout << "  ++ it was a middle node...";
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }

  // consistency_check();
}

//
// updates the digram's frequency and moves it around the queue to the proper place
//
repair_digram* repair_priority_queue::update_digram_frequency(
    std::string *digram_string, int new_value){

  // if element doesnt exist
  if (nodes.find(*digram_string) == nodes.end()) {
//      Rcout << "  ++ attempted to correct the digram frequency for " << *digram_string <<
//        " but no node found..." << std::endl;
    return nullptr;
  }

  // get a pointer on that node
  repair_pqueue_node* altered_node = nodes.find(*digram_string)->second;
//   Rcout << "  ++ correcting the digram frequency for " << altered_node->payload->digram
//         << " from " << altered_node->payload->freq << " to " << new_value << std::endl;

  // the trivial case
  if (new_value == altered_node->payload->freq) {
    // Rcout << "  ++ new value is same as the old, exiting... " << std::endl;
    return altered_node->payload;
  }

  // simply evict the node if the new freq is too low -- this is a default behavior
  if (2 > new_value) {
    // Rcout << "  ++ new value is less than 2, cleaning up the entry... " << std::endl;
    remove_node(altered_node);
    return nullptr;
  }

  // update the frequency
  int oldFreq = altered_node->payload->freq; // remember the old one
  altered_node->payload->freq = new_value;

  // if the list is just too short
  if (1 == nodes.size()) {
    // Rcout << "  ++ queue is a single element, just updating..." << std::endl;
    return altered_node->payload;
  }
  // all other cases will have the tail and the head...

  // if we have to move the element up in the list
  if (new_value > oldFreq) {

    // check the head status
    if (nullptr == altered_node->prev) {
      // no change needed
      // consistency_check();
      return altered_node->payload;
    }

    // in another case we need to traverse up and insert the altered node up there
    repair_pqueue_node* current_node = altered_node->prev;
    if (altered_node->payload->freq <= current_node->payload->freq) { // no need to go up
      return altered_node->payload;
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
    nodes.emplace(altered_node->payload->digram, altered_node);
  }

  // we need to move the element down in the list
  else {

    // what if this is a tail already?
    if (nullptr == altered_node->next) {
      // consistency_check();
      return altered_node->payload;
    }

    // going down..
    repair_pqueue_node* current_node = altered_node->next;
    if (altered_node->payload->freq >= current_node->payload->freq) { // no need to go down
      return altered_node->payload;
    }

    remove_node(altered_node);
    altered_node->next = nullptr;
    altered_node->prev = nullptr;

    int ctr = 0;
    while (nullptr != current_node->next &&
           current_node->payload->freq > altered_node->payload->freq) {
      current_node = current_node->next;
      ctr++;
    }
    // Rcout << " CTR: " << ctr;

    if (nullptr == current_node->next) { // we hit the tail

      if (altered_node->payload->freq > current_node->payload->freq) {
        // place before tail
        if (queue_head == current_node) { // this may happen...
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
      // or make a new tale
      else {
        current_node->next = altered_node;
        altered_node->prev = current_node;
      }

    // we found another place which doesn't deal with the tail
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
    nodes.emplace(altered_node->payload->digram, altered_node);
  }

  // Rcout << consistency_check();

  // Rcout << to_string() << "\n";

  return altered_node->payload;

}


//
// gets the top digram out of the queue
//
repair_digram* repair_priority_queue::dequeue() {
  if(nullptr != queue_head){
    // Rcout << "  ++ dequeueing the node " << queue_head->payload->digram << std::endl;
    repair_pqueue_node* res = queue_head; // make sure we returning the head
    queue_head = queue_head->next; // make head to point onto the next node
    if(nullptr != queue_head){
      queue_head->prev = nullptr; // make the head's prev pointer to be null
    }
    nodes.erase(res->payload->digram); // cleanup the accounkeeping datastructure
    // consistency_check();
    return res->payload;
  }
  return nullptr;
}

//
// translates the queue into an array of digram tokens
//
std::vector<repair_digram> repair_priority_queue::to_array() {
  std::vector<repair_digram> res;
  repair_pqueue_node* cp = queue_head;
  while(NULL != cp->next){
    res.push_back(*cp->payload);
  }
  return res;
}

//
// printh the whole queue as a string
//
std::string repair_priority_queue::to_string() {
  std::stringstream res;
  repair_pqueue_node* ptr = queue_head;
  while(nullptr != ptr) {
    res << ptr->payload->digram << " : " << ptr->payload->freq << std::endl;
    ptr = ptr->next;
  }
  return res.str();
 }

//
// check for a digram presence in the queue
//
bool repair_priority_queue::contains_digram(std::string *digram_string) {
  if (nodes.find(*digram_string) == nodes.end()) {
    return false;
  }
  return true;
}

//
// check for a digram presence in the queue
//
bool repair_priority_queue::consistency_check() {
  Rcout << "   + consistency_check on ...";
  repair_pqueue_node* ptr = queue_head;
  while(nullptr != ptr) {
    repair_pqueue_node* p = ptr -> prev;
    repair_pqueue_node* n = ptr ->next;
    // head case & tail case check
    if(nullptr == p && nullptr == n ){
      if(!(queue_head == ptr)){
        Rcout << " !!! consistency_check FAILED ..." << std::endl;
        return false;
      }
    }
    if(nullptr == p && nullptr != n && queue_head != ptr){
      Rcout << " !!! consistency_check off FAILED..." << std::endl;
      return false;
    }
    ptr = ptr->next;
  }
  Rcout << "  ->  consistency_check off ..." << std::endl;
  return true;
}
