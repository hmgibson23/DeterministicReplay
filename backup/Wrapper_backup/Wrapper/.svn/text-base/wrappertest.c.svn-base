#include "wrapper.h"


void *f(void* id) 
{
  int i  = (int*) id;
  sleep(1);
  printf("Thread %d running!\n", i);
}

int main()
{


  print_schedule();
  pthread_t a = intercept_pthread_create((void*)f, (void*)1);
  pthread_t b = intercept_pthread_create((void*)f, (void*)2);
  pthread_t c = intercept_pthread_create((void*)f, (void*)3);
  pthread_t d = intercept_pthread_create((void*)f, (void*)4);

  /*sleep(4);*/

  intercept_pthread_join(a,1);
  intercept_pthread_join(b,2);
  intercept_pthread_join(c,3);
  intercept_pthread_join(d,4);


  printf("Step: %d \n", step);

  return 0;

}
