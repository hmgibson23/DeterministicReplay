// --scheduler-preemption-bound=-1
//KLEE: done: completed paths = 12
//KLEE: done: num paths considering hb graph cache = 4

// 1 non-det choice in each thread.

#include <pthread.h>
#include <assert.h>
#include <stdio.h>

void klee_make_symbolic(void *addr, size_t nbytes, const char *name);

int klee_fork(int reason);

//pthread_mutex_t mutex;

void* slave(void* args)
{
  volatile int b;
  klee_make_symbolic((void*)&b, sizeof(b), 0);
  
  if(b == 0)
  {
    b = 1;
  }
  else
  {
    b = 2;
  }
}

int main(int argc, char** argv)
{
  pthread_t handle;
  
  pthread_create(&handle, NULL, slave, NULL);
  
  volatile int a;
  klee_make_symbolic((void*)&a, sizeof(a), 0);
  
  if(a == 0)
  {
    a = 1;
  }
  else
  {
    a = 2;
  }
  
  
  
  pthread_join(handle, NULL);
  
  return 0;
}


