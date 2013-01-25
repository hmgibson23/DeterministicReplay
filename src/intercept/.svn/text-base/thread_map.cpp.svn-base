#include <map>
#include <iostream>
#include "thread_map.h"

#include <assert.h>


std::map<pthread_t, int>* idMap = NULL;



extern "C"
int get_instrumented_id(pthread_t id) {
  return (*idMap)[id];
}


extern "C"
void put_instrumented_id(pthread_t id, int instrumented_id)
{
 
  if(!idMap)
    {
      idMap = new std::map<pthread_t, int>;
    } 
  (*idMap)[id] = instrumented_id;

}


extern "C"
void print_mappings()
{

  if(!idMap)
    {
      return;
    }
  else {
    std::cout << "Map contents:" << std::endl;
    for(std::map<pthread_t, int>::const_iterator iter = idMap->begin();
	iter != idMap->end(); ++iter)
      {
	std::cout << std::hex <<iter->first << '\t' << std::hex << iter->second << std::endl;
      }
  }
}
