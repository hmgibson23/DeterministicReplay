#include <pthread.h>
#include <stdio.h>
#include <assert.h>

int NUM_THREADS = 5;
int GLOB = 0;

pthread_mutex_t mutex;

void* g(void* a) {
  int p = (int) a;
  pthread_mutex_lock(&mutex);
  GLOB = p;
  pthread_mutex_unlock(&mutex);
}

void *f() {
  pthread_t threads[NUM_THREADS];
  int i, j;
  int rc;
  for(i = 0; i < NUM_THREADS; i++)
    {
      rc = pthread_create(&threads[i], NULL, g, (void*)i);
    }
  for(j = 0; i< NUM_THREADS;++j)
    {
      pthread_join(threads[j], NULL);
    }

  fprintf(stderr, "GLOB = %d\n", GLOB);
}


int main() {

  pthread_t a, b;

  pthread_create(&a, NULL, f, NULL);

  pthread_join(a, NULL);

	assert(0);

  return 0;
}

