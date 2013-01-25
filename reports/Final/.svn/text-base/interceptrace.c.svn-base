#include <pthread.h>
#include <stdio.h>

int GLOB = 0;
void* fn(void*);

pthread_mutex_t mut;

int main()
{
  pthread_t a,b,c,d;
  pthread_create(&a, NULL, fn, (void*)1);
  pthread_create(&b, NULL, fn, (void*)2);
  pthread_create(&c, NULL, fn, (void*)3);
  pthread_create(&d, NULL, fn, (void*)4);


  pthread_join(a, NULL);
  pthread_join(b, NULL);
  pthread_join(c, NULL);
  pthread_join(d, NULL);


  fprintf(stderr, "GLOB: %d\n", GLOB);
  return 0;
}


/*
 *Make thread lock a mutex, open a file, write to the file and then change the
 *the global variable GLOB
 */
void* fn(void* a)
{
  int i = (int) a;
  pthread_mutex_lock(&mut);
  GLOB = i;
  FILE *f = fopen("something.txt", "r");
  fputc(i, f);
  fclose(f);
  FILE* g = fopen("another.txt", "r");
  int j;
  for(j = 0; j < 10000; ++j)
    {
      fclose(g);
      g = fopen("another.txt", "r");
      fputc(j, g);
    }
  fclose(g);
  pthread_mutex_unlock(&mut);
  return 0;
}
