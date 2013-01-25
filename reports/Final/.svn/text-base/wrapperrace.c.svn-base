#include "wrapper.h"

/*global schedule*/
int schedule[] = {
  #include "schedule#3.h"
};

pthread_mutex_t mut;
int GLOB = 0;
void* fn(void*);
int main()
{

  pthread_t a = intercept_pthread_create((void*)fn, (void*)1);  
  pthread_t b = intercept_pthread_create((void*)fn, (void*)2);  
  pthread_t c = intercept_pthread_create((void*)fn, (void*)3);
  pthread_t d = intercept_pthread_create((void*)fn, (void*)4);
  intercept_pthread_join(a, NULL);
  intercept_pthread_join(b, NULL);
  intercept_pthread_join(c, NULL); 
  intercept_pthread_join(d, NULL);
  fprintf(stderr, "GLOB: %d\n", GLOB);
  return 0;
}
void* fn(void* a)
{
  int i = (int) a;
  intercept_mutex_lock(&mut);
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
  intercept_mutex_unlock(&mut);
  return 0;
}
