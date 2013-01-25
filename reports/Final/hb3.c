// --scheduler-preemption-bound=1
//KLEE: done: completed paths = 4
//KLEE: done: num paths considering hb graph cache = 2

#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int klee_fork(int reason);

pthread_mutex_t m;
int g;
void* slave(void* args)
{
  pthread_mutex_lock(&m);
  pthread_mutex_unlock(&m);
}

void* slave2(void* args)
{
}

int main(int argc, char** argv)
{
  pthread_t handle;
  pthread_t handle2;
  pthread_mutex_init(&m, NULL);
  
  pthread_create(&handle, NULL, slave, NULL);
  //pthread_create(&handle2, NULL, slave2, NULL);
  pthread_mutex_lock(&m);
  pthread_mutex_unlock(&m);
  
  pthread_join(handle, NULL);
  //pthread_join(handle2, NULL);
  
  pthread_mutex_destroy(&m);
  return 0;
}


