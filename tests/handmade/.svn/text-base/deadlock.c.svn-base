//

// The first line must be a comment and is arguments for klee.
// Further comments are commands to execute afterwards, which must have exit status 0.
// Then there must be an empty line.


#include <pthread.h>
#include <assert.h>
#include <stdio.h>

#define NUM_THREADS1 (2)
#define NUM_THREADS2 (1)

pthread_mutex_t mutexA;
pthread_mutex_t mutexB;

void* locker1(void* args)
{
  pthread_mutex_lock(&mutexA);
  pthread_mutex_lock(&mutexB);
  pthread_mutex_unlock(&mutexB);
  pthread_mutex_unlock(&mutexA);
  return 0;
}

void* locker2(void* args)
{
  pthread_mutex_lock(&mutexB);
  pthread_mutex_lock(&mutexA);
  pthread_mutex_unlock(&mutexA);
  pthread_mutex_unlock(&mutexB);
  return 0;
}

void* t1(void* args)
{
  pthread_t otherHandles[NUM_THREADS1];
  size_t i;
  
  for(i=0; i < NUM_THREADS1; ++i)
  {
    pthread_create(&otherHandles[i], NULL, locker2, NULL);
  }
  
  for(i=0; i < NUM_THREADS1; ++i)
  {
    pthread_join(otherHandles[i], NULL);
  }
  
  return 0;
}

int main(int argc, char** argv)
{
  pthread_mutex_init(&mutexA, NULL);
  pthread_mutex_init(&mutexB, NULL);
  
  pthread_t handle;
  
  pthread_create(&handle, NULL, t1, NULL);
  
  pthread_t handles[NUM_THREADS2];
  size_t i;
  for(i=0; i < NUM_THREADS2; ++i)
  {
    pthread_create(&handles[i], NULL, locker1, NULL);
  }
  
  for(i=0; i < NUM_THREADS2; ++i)
  {
    pthread_join(handles[i], NULL);
  }
  
  pthread_join(handle, NULL);
  
  return 0;
}