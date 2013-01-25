#include "../wrapper.h"
int schedule[] = {
  #include "../schedules/racesched/schedule#3.h"
};

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
  GLOB = i;
}
