#include <pthread.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

pthread_mutex_t mutex;

int GLOB = 0;

void *f(void* c) {
  int p = (int) c;
  pthread_mutex_lock(&mutex);
  GLOB = p;
  pthread_mutex_unlock(&mutex);
  return 1;
}

void* g()
{
  pthread_t threads[6];
  int i;
  for(i = 0; i < 6; ++i)
    {
      pthread_create(&threads[i], NULL, f, (void*)i);
    }
  for(i = 0; i < 6; ++i) 
    {
      pthread_join(threads[i], NULL);
    }
}

int main()
{
  pthread_t a;
  pthread_create(&a, NULL, g, NULL);

  GLOB = 0;

  pthread_join(a, NULL);

  printf("GLOB = %d \n", GLOB);

  return 0;
}

