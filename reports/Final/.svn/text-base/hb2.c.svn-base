// --scheduler-preemption-bound=0
//KLEE: done: completed paths = 3
//KLEE: done: num paths considering hb graph cache = 1

#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int klee_fork(int reason);

//pthread_mutex_t mutex;

void* slave(void* args)
{
  
}

void* slave2(void* args)
{
}

int main(int argc, char** argv)
{
  pthread_t handle;
  pthread_t handle2;
  
  pthread_create(&handle, NULL, slave, NULL);
  pthread_create(&handle2, NULL, slave2, NULL);
  
  pthread_join(handle, NULL);
  pthread_join(handle2, NULL);
  
  return 0;
}


