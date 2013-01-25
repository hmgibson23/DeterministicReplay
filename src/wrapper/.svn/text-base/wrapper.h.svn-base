#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>

#include "wrapper.h"
#include "thread_map.h"
#include "scheduler.h"

typedef struct {
  void* (*function)(void*);
  int thread_id;
  void* other_args;
} thread;



/*current step and id*/
int step = 0;
int current_id = 1;


/*follow flag*/

/*If this flag is set the tool should follow the schedule to
 *its end and then continue execution as normal
 */
int FOLLOW = 0;



/*mutex for step*/
pthread_mutex_t step_mutex;
/*mutex for thread_map*/
pthread_mutex_t map_lock;


/*condition variable for scheduling threads*/
pthread_cond_t condition;



pthread_t intercept_pthread_create(void* (*fn)(void*), void* o_args);
int intercept_pthread_join(pthread_t, void**);
void intercept_mutex_lock(pthread_mutex_t*);
void intercept_mutex_unlock(pthread_mutex_t*);

void* run_thread(void *);
void wait();
void step_and_notify();


int first_call = 0;
/*intercept pthread_create*/
pthread_t intercept_pthread_create(void* (*fn)(void*), void* o_args)
{   

  if(!first_call)
    {
      put_instrumented_id(pthread_self(), 0);
      first_call = 1;
    }

  /*set up the thread parameters*/
  pthread_t a;
  thread *args = malloc(sizeof(thread));
  args->function = fn;
  args->thread_id = current_id; 

  args->other_args = o_args;


  current_id++;

  pthread_create(&a, NULL, run_thread, (void*) args); 

  /*put the instrumented id in the map*/
  put_instrumented_id(a, args->thread_id);



  step_and_notify();
  wait();

  return a;   
}

/*start new thread*/
void* run_thread(void* arg)
{
  thread* args = (thread*) arg;
  put_instrumented_id(pthread_self(), args->thread_id);
  wait();

  /*run the function*/
  void* result = args->function((void*)args->other_args);
 
  /*indicate that the thread's function has been run*/
  step_and_notify();

  return result;
}





/*step and notify */
void step_and_notify()
{


  pthread_mutex_lock(&step_mutex);
  step++;
  pthread_cond_broadcast(&condition);
  pthread_mutex_unlock(&step_mutex);
}


/*wait function*/
void wait()
{
  
  int thread_id = get_instrumented_id(pthread_self());
  
  pthread_mutex_lock(&step_mutex);
  while(thread_id != schedule[step])
    {    
      fprintf(stderr, "Thread %d, sleeping...schedule[%d], step:%d\n", 
	thread_id, schedule[step], step);
      pthread_cond_wait(&condition, &step_mutex);
    }
  
  
  pthread_mutex_unlock(&step_mutex);
  
}


// TODO: you should probably add "void **value_ptr" as argument
int intercept_pthread_join(pthread_t t, void** value_ptr)
{
  step_and_notify();
  wait();

//TODO: This should probably be t, not self 
  int id = get_instrumented_id(t);
  fprintf(stderr, "Waiting for thread %d to join!\n", id);
  
//new code from now on

  int res = pthread_tryjoin_np(t,value_ptr);
  if(res != EBUSY)
  {
    //thread t has exited
    return res;
  }
  //thread t has not exited yet.
  //current thread should now block.
  step_and_notify();
  wait();
  
  res = pthread_tryjoin_np(t,value_ptr);
  if(res != EBUSY)
  {
    //thread t has exited
    return res;
  }
  
  /*
  fprintf(stderr, "Schedule[%d] at failure!\n and step:%d \n", schedule[step], step);
  assert(0 && "Invalid schedule!");*/
}


/*intercept mutex lock*/
void intercept_mutex_lock(pthread_mutex_t* m)
{

  int id = get_instrumented_id(pthread_self());
  fprintf(stderr, "Thread with id: %d trying to lock mutex, schedule[%d], step:%d\n",
	  id, schedule[step], step);
  step_and_notify();
  wait();
  if(pthread_mutex_trylock(m))
    {
      return;
    }

  step_and_notify();
  wait();

  if(pthread_mutex_trylock(m))
    {
      return;
    }

  assert(false);
}

/*intercept mutex unlock*/
void intercept_mutex_unlock(pthread_mutex_t* m)
{
  step_and_notify();
  int id = get_instrumented_id(pthread_self());
  wait();

  fprintf(stderr, "Thread %d unlocking mutex schedule[%d] and step:%d\n",
	  id, schedule[step], step);

  pthread_mutex_unlock(m);
}
