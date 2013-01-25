#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


int GLOB = 0;
int g = 0;
pthread_cond_t cond;
pthread_mutex_t mut, g_mut;

void* f(void* c) {

  int i = (int) c;

  pthread_mutex_lock(&mut);
  while(g != 3)
    pthread_cond_wait(&cond, &mut);
  GLOB = i;
  pthread_mutex_unlock(&mut);
  return 0;
}

void* p()
{
  pthread_mutex_lock(&g_mut);
  g++;
  pthread_mutex_unlock(&g_mut);
  pthread_cond_broadcast(&cond);
}


int main()
{
  pthread_t a, b, c, d;
  pthread_create(&a, NULL, f, (void*)1);
  pthread_create(&b, NULL, f, (void*)2);
  pthread_create(&c, NULL, p, (void*)3);
  pthread_create(&d, NULL, p, (void*)3);


  g++;

  pthread_join(a,NULL);
  pthread_join(b,NULL);
  pthread_join(c,NULL);
  pthread_join(d,NULL);


  printf("\n\nGLOB = %d\n\n", GLOB);

  assert(false);

  return 0;
}
