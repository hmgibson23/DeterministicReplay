#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include "build_sched.h"

//Builds the schedule the tool followed.

std::vector<int>* followedSched = NULL;

extern "C"
void add_to_followed(int num) {
    
    if (!followedSched) {
      followedSched = new std::vector<int>; 
    }
    
    followedSched->push_back(num);
}

extern "C"
void print_followed() {
    std::cout << "The tool followed the following schedule in this execution run" << std::endl;
    
    std::cout << "[";
    for(std::vector<int>::const_iterator iter = followedSched->begin();
	iter != followedSched->end(); ++iter)
      {
	std::cout << *iter << " ";
      }
    std::cout << "]";
    std::cout << std::endl;
}

extern "C"
bool schedules_matched(int* sched, int length) {
  std::vector<int> isched(sched, sched+length);
  if(isched == *followedSched) {
    std::cout << "The tool faithfully followed the schedule!" << std::endl;
    return 1;
  } else {
    std::cout << "The tool failed to follow the schedule!" << std::endl;
    return 0;
  }
 
}

extern "C"
void write_to_file() {
  
  std::cout << "The schedule that led to the deadlock has been written to a file " 
	    << "schedules/deadlock.schedule." << std::endl;
  
  std::ofstream output_file("schedules/deadlock.schedule");
  for(std::vector<int>::const_iterator i = followedSched->begin(); 
      i != followedSched->end(); ++i) {
    output_file << *i << ',';
  }
}
