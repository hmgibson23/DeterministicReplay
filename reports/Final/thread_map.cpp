#include <map>
#include <iostream>
#include <pthread.h>
#include <assert.h>

#include "thread_map.h"

pthread_mutex_t put_mutex;

std::map<pthread_t, int>* idMap = NULL;
pthread_mutex_t print_mutex;


extern "C"
int get_instrumented_id(pthread_t id) {
  assert (idMap->find(id) != idMap->end());
  int instrumented_id = (*idMap)[id];
  return instrumented_id;
}


extern "C"
void put_instrumented_id(pthread_t id, int instrumented_id)
{
  if(!idMap)
    {
      idMap = new std::map<pthread_t, int>;
    }
  pthread_mutex_lock(&put_mutex);
  (*idMap)[id] = instrumented_id;
  pthread_mutex_unlock(&put_mutex);
}


extern "C"
void print_mappings()
{

  if(!idMap)
    {
      return;
    }
  else {
    pthread_mutex_lock(&print_mutex);
    std::cout << "Map contents:" << std::endl;
    for(std::map<pthread_t, int>::const_iterator iter = idMap->begin();
	iter != idMap->end(); ++iter)
      {
	std::cout << std::hex <<iter->first << '\t' << std::hex 
		<< iter->second << std::endl;
      }
    pthread_mutex_unlock(&print_mutex);
  }
}
