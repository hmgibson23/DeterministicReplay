#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#include "scheduler.h"

typedef struct {
  void (*function)(void*);
  int thread_id;
  void* other_args;
  pthread_t tid;
} thread;



/*current step and id*/
int step = -1;
int current_id = 1;


/*mutex for step*/
pthread_mutex_t step_mutex;

/*condition variable for scheduling*/
pthread_cond_t condition;

/*function prototypes*/
void* start_new_thread(void *);
pthread_t intercept_pthread_create(void (*fn)(void*), void* o_args);
void intercept_pthread_join(pthread_t, int);
void intercept_mutex_lock(pthread_t, pthread_mutex_t*, int);
void wait(thread*);
void step_and_notify();





/*intercept pthread_create*/
pthread_t intercept_pthread_create(void (*fn)(void*), void* o_args)
{   

  /*not sure I should actually step_and_notify() in this function*/
  step_and_notify(); 
  

  fprintf(stderr,"Creating thread %d...step is %d\n", current_id, step);
  int g = schedule_size();
  /*fprintf(stderr, "Schedule is %d long...\n", g);*/
  int max = schedule_size();
  /*fprintf(stderr, "max step is: %d ..\n", max);*/
  /*set up the thread parameters*/
  pthread_t a;
  thread *args = malloc(sizeof(thread));
  args->function = fn;
  args->thread_id = current_id;    
  args->other_args = o_args;
  args->tid = a;

  wait(args);
  
  current_id++;
    
  pthread_create(&a, NULL, start_new_thread, (void*) args); 

 

  return args;   
}

/*start new thread*/
void* start_new_thread(void* arg)
{
  thread* args = (thread*) arg;
  
  wait(args);

  /*fprintf(stderr, "Calling thread function...\n");*/
  
  /*fprintf(stderr, "About to run thread %d with step: %d and sched: %d,\n", 
    args->thread_id,step,schedule[step]);*/
  args->function((void*)args->other_args);


  step_and_notify();

}


/*intercept pthread join*/
void intercept_pthread_join(pthread_t t, int thread_id)
{
  fprintf(stderr, "Waiting for thread...%d\n", thread_id);
  wait(thread_id);
  pthread_join(t, NULL);
  //step_and_notify();
}



/*step and notify */
void step_and_notify()
{
  pthread_mutex_lock(&step_mutex);
  step++;
  /*printf("Step: %d\n", step);*/
  pthread_cond_broadcast(&condition);
  pthread_mutex_unlock(&step_mutex);
}


/*wait function*/
/*
There needs to be some sort of mechanism to handle the event
that when threads are being created it is not their turn to be created.
Since if that thread id is not on the schedule then the thread will
not be created and the program will hand indefinitely.
 */
void wait(thread* t)
{
  if(!thread_in_schedule(t->thread_id))
    {
      fprintf(stderr,"Thread not in schedule with id .. %d!\n", t->thread_id);
      /*kill the calling thread to prevent it from running any longer*/
      pthread_kill(t->tid, NULL);
      return;
    }


  pthread_mutex_lock(&step_mutex);
  while(t->thread_id != schedule[step] && step < schedule_size()-1)
    {    
      //fprintf(stderr, "Thread %d, sleeping...\n", t->thread_id);
      //fprintf(stderr, "Step: %d and Schedule: %d\n", step, schedule[step]);
      pthread_cond_wait(&condition, &step_mutex);
    } 
  pthread_mutex_unlock(&step_mutex);
  /*fprintf(stderr, "Thread %d awake ...\n", thread_id);*/
}

