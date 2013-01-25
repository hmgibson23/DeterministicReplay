#include <pthread.h>


//global schedule
int schedule[];


pthread_t* intercept_pthread_create(pthread_t p, void* f, void* arg)
{
  return *pthread_create(p, NULL, p, arg); 
}
  
