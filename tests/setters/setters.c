//

// The first line must be a comment and is arguments for klee.
// Further comments are commands to execute afterwards, which must have exit status 0.
// Then there must be an empty line.


#include <pthread.h>
#include <assert.h>
#include <stdio.h>

#define NUM_THREADS1  (2)
#define NUM_THREADS2  (1)

// main thread creates t1.
// t1 creates NUM_THREADS1 setters.
// main thread also creates NUM_THREADS2 setters.
// All setters set int global to a different value.
// We print the value of global at the end.

pthread_mutex_t mutex;
volatile int g=0;

void* setter(void* args)
{
  pthread_mutex_lock(&mutex);
  
  g = *(int*)args;
  pthread_mutex_unlock(&mutex);
  return 0;
}

void* t1(void* args)
{
  pthread_t otherHandles[NUM_THREADS1];
  int threadArgs[NUM_THREADS1];
  size_t i;
  
  for(i=0; i < NUM_THREADS1; ++i)
  {
    threadArgs[i] = i+1;
    pthread_create(&otherHandles[i], NULL, setter, &threadArgs[i]);
  }
  
  for(i=0; i < NUM_THREADS1; ++i)
  {
    pthread_join(otherHandles[i], NULL);
  }
  
  return 0;
}


int main(int argc, char** argv)
{
  pthread_t handle;
  pthread_t otherHandles[NUM_THREADS2];
  int threadArgs[NUM_THREADS2];
  size_t i;
  pthread_mutex_init(&mutex, NULL);
  
  pthread_create(&handle, NULL, t1, NULL);
  for(i=0; i < NUM_THREADS2; i++)
  {
    threadArgs[i] = NUM_THREADS1+i+1;
    pthread_create(&otherHandles[i], NULL, setter, (void*)&threadArgs[i] );
  }
  
  for(i=0; i < NUM_THREADS2; ++i)
  {
    pthread_join(otherHandles[i], NULL);
  }
  
  pthread_join(handle, NULL);
  
  assert(g != 0 && "g is 0 after joining!");
  
  printf("On this schedule, g ends up being %d\n", g);
  
  return 0;
}