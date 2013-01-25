#include <pthread.h>
#include <stdio.h>

int i = 0;

void* inc() {
  i++;
}

void* lock() {
  pthread_mutex_lock(&imut);
  if(i == 2)
    pthread_mutex_unlock(&imut);
  else
    return;
}

int main()
{
  pthread_t a,b,c,d,e,f;
  pthread_create(&a, NULL, inc, NULL);
  pthread_create(&b, NULL, inc, NULL);
  pthread_create(&c, NULL, inc, NULL);
  pthread_create(&d, NULL, lock, NULL);
  pthread_create(&e, NULL, lock, NULL);
  pthread_create(&f, NULL, inc, NULL);


  pthreead_join(&a, NULL);
  pthreead_join(&b, NULL);
  pthreead_join(&c, NULL);
  pthreead_join(&d, NULL);
  pthreead_join(&e, NULL);
  pthreead_join(&f, NULL);
  
  return 0;
}
