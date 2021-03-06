/*defining _GNU_SOURCE gives access to the RTLD_NEXT variable */
#define _GNU_SOURCE


#include <stdio.h>
#include <stdint.h>
#include <bits/pthreadtypes.h>
#include <dlfcn.h>
#include <malloc.h>
#include <pthread.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <execinfo.h>

#include "thread_map.h"
#include "read_file.h"
#include "build_sched.h"
#include "print_trace.h"

/*
 * Global integers used to specify what mode of execution the tool is in.
 */
/*Follow to the end of the schedule and resume normal execution*/
static int FOLLOW_TO_END = 0;
/*Follow to the end of the schedule and terminate.*/
static int FOLLOW_AND_TERMINATE = 0;
/*Run with verbose output*/
static int VERBOSE = 0;
/*RECORD mode is use when there is no schedule*/
static int RECORD_MODE = 0;
/*MONITOR mode is used when there is as schedule and is 
used to prevent the tool from deadlocking.*/
static int MONITOR_MODE = 0;
/*DISABLE flag is used for debugging purposes*/
static int DISABLE = 0;

/*Current id is the ID of the each thread being created*/
static int current_id = 1;
/*Step is incremented at each sychronisation point to move the schedule forward*/
static int step = 0;


/*Pointer to the schedule built in C++ calls*/
int *schedule;


/*arguments to go to a thread*/
typedef struct {
  void* (*function)(void*);
  int thread_id;
  void* other_args;
} params;


/*mutex and condition variables*/
/*These are used internally by the tool for synchronisation*/
pthread_mutex_t step_mutex;
pthread_cond_t step_condition;
pthread_mutex_t id_lock;
pthread_mutex_t follow;
pthread_mutex_t nofollow;
pthread_mutex_t end_lock;
pthread_mutex_t terminate_lock;
pthread_mutex_t output_mutex;
pthread_mutex_t follow_lock;



/*Monitor thread used in RECORD mode to trace the progress of the program */
pthread_t monitor_thread;



/*functions to prevent recursive function calls*/
int mutex_lock(pthread_mutex_t*);
int mutex_unlock(pthread_mutex_t*);
int cond_wait(pthread_cond_t*, pthread_mutex_t*);



/*utlity functions*/
void wait();
void step_and_notify();
int schedule_length();
int get_schedule();
void output_schedule();
void check_status();
void set_follow();
int no_follow();
int terminate_set();
void end();
void check_verbose();
void* run_thread(void*);
void update_followed();
void* monitor_progress();

/*check if this is the first call to pthread_create. Also used to alert us to whether or not instrumentation has begun.*/
static int first_call = 0;





/*************************OVERIDDEN PTHREAD FUNCTIONS***************************/

/*intercepts*/
int pthread_create (pthread_t* thread,
		    __const pthread_attr_t* attr, 
		    void *(*start) (void *), 
		    void *arg) {

  static int (*real_create)(pthread_t * , pthread_attr_t *, void * (*start)(void *), void *) = NULL; /*function pointer to real create function*/
  
  if (!real_create)
    real_create = dlsym(RTLD_NEXT, "pthread_create");
  if(!first_call)
    {
      set_follow();
      check_verbose();
      int got_sched = get_schedule();
      if(!got_sched)
	{
	  fprintf(stderr, "Failed to find the schedule! .... tool will now exit\n");
	  exit(1);
	}
      first_call = 1;
      if(VERBOSE) {
	fprintf(stderr, "Schedule length is: %d\n", schedule_length());
      }
      if(got_sched == 1) {
	fprintf(stderr, "Schedule to be followed:\n");
	output_schedule();
      }
      mutex_lock(&id_lock);
      put_instrumented_id(pthread_self(), 0);
      mutex_unlock(&id_lock);
      if(RECORD_MODE || MONITOR_MODE) {
	fprintf(stderr, "Creating monitor thread ... \n");
	real_create(&monitor_thread, NULL, monitor_progress, NULL);
      }
    }

  /*set up the thread arguments*/
  params* args = malloc(sizeof(params));
  args->thread_id = current_id;
  args->other_args = arg;
  args->function = start;

  current_id++;
  int rc;
  if(no_follow()) {
	return real_create(thread, NULL, start, args);
	}
  if(VERBOSE) {
    printf("Current schedule step: %d\n", schedule[step]);
  }
  /*Run the thread wrapped in another thread*/
  rc = real_create(thread, NULL, run_thread, args);
  step_and_notify();
  if(!no_follow()) {  
    wait();
  }
  
  return rc;
}


int pthread_join(pthread_t tid, void** value_ptr)
{ 
  int rc;
  static int (*real_join)(pthread_t,void *) = NULL;
  
  if (!real_join)
    real_join = dlsym(RTLD_NEXT, "pthread_join");
  
  if(!no_follow() && !RECORD_MODE) {
    int id = get_instrumented_id(tid);
    if(VERBOSE) {
      fprintf(stderr, "Waiting for thread %d to join!\n", id);
    }
    step_and_notify();
    wait();
    
    int res = pthread_tryjoin_np(tid,value_ptr);
    if(res != EBUSY)
      {
	return res;
      }
    if(no_follow()) {
      return real_join(tid, value_ptr);
    }
    step_and_notify();
    wait();
    
    res = pthread_tryjoin_np(tid,value_ptr);
    if(res != EBUSY)
      {
	return res;
      }
    if(no_follow()) {
      pthread_cond_broadcast(&step_condition);
      return real_join(tid, value_ptr);
    }
    step_and_notify();
    wait();
    res = pthread_tryjoin_np(tid, value_ptr);
    if(res != EBUSY) {
      return res;
    }
    
    return pthread_tryjoin_np(tid, value_ptr);
  } else {
    step_and_notify();
    rc = real_join(tid, value_ptr);
    return rc;
  }
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{

  
  /*DISABLE flag is used only for debugging purposes and printing stack traces.*/
  if(no_follow() || DISABLE || !first_call)
    {
      return mutex_lock(mutex);
    }

  int res;
  if(!no_follow() && !RECORD_MODE){
    int id = get_instrumented_id(pthread_self());
    if(VERBOSE) {
    fprintf(stderr, "Thread with id: %d trying to lock mutex, schedule[%d], step:%d\n",
	    id, schedule[step], step);
    }

    /*
     Check that we have not reached the end of the schedule yet and if we have then use the normal
     pthread mutex lock. These checks are scattered all throughout the function since a thread might
     be halfway through execution when the flag is set.
     */
    if(no_follow()) {
      pthread_cond_broadcast(&step_condition);
      mutex_unlock(&step_mutex);
      return mutex_lock(mutex);
    }
    
    step_and_notify(); 
    wait();
    if(pthread_mutex_trylock(mutex))
      {
	if(no_follow()) {
	  pthread_cond_broadcast(&step_condition);
	  mutex_unlock(&step_mutex);
	  return mutex_lock(mutex);
	}
	if(VERBOSE) {
	  fprintf(stderr, "Thread: %d obtained the mutex\n", id);
	}
	return 0;
      } 
    
    
    if(no_follow()) {
      pthread_cond_broadcast(&step_condition);
      mutex_unlock(&step_mutex);
      return mutex_lock(mutex);
    }    
    step_and_notify();  
    wait();
    
    
    if(pthread_mutex_trylock(mutex))
      {
	if(no_follow()) {
	  return mutex_lock(mutex);
	}
	if(VERBOSE) {
	  fprintf(stderr, "Thread: %d obtained the mutex\n", id);
	}
	return 0;
      }
    
    if(pthread_mutex_trylock(mutex))
      {
	if(no_follow()) {
	  return mutex_lock(mutex);
	}
	if(VERBOSE) {
	  fprintf(stderr, "Thread: %d obtained the mutex\n", id);
	}
	return 0;
       }
    
    if(no_follow()) {
      return mutex_lock(mutex);
    }
    assert(false);
  } else {
    /*step and notify used here for recording purposes.*/
    step_and_notify();
    res = mutex_lock(mutex);
    return res;
  }
}


int pthread_mutex_unlock(pthread_mutex_t* mutex)
{
  if(no_follow() || DISABLE || !first_call)
    {
      pthread_cond_broadcast(&step_condition);
      return mutex_unlock(mutex);
    }
  if(!no_follow() && !RECORD_MODE) {
    int id = get_instrumented_id(pthread_self());
    step_and_notify();
    wait();
    if(no_follow()) {
      pthread_cond_broadcast(&step_condition);
      mutex_unlock(&step_mutex);
      return mutex_unlock(mutex);
    }
    
    if(VERBOSE) {
      if(no_follow()) {
	pthread_cond_broadcast(&step_condition);
	mutex_unlock(&step_mutex);
	return mutex_unlock(mutex);
      }
      
      fprintf(stderr, "Thread %d unlocking mutex schedule[%d] and step:%d\n",
	      id, schedule[step], step);
    }
    int res = mutex_unlock(mutex);
    return res;
  } else {
    /*step and notify used here for recording purpose only.*/
    step_and_notify();
    return mutex_unlock(mutex);
  }
}

int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex)
{
  if(VERBOSE) {
    fprintf(stderr, "Thread %d waiting on condition....\n", get_instrumented_id(pthread_self()));
  }
  if(!no_follow() && !RECORD_MODE) {
    mutex_unlock(mutex);
    step_and_notify();
    wait();
    mutex_lock(mutex);
  } else {
    /*step and notify used here for recording purposes only.*/
    step_and_notify();
    return cond_wait(cond, mutex);
  }
  return 0;
}

int pthread_cond_timedwait(pthread_cond_t *__restrict cond,
			   pthread_mutex_t *__restrict mutex,
			   __const struct timespec *__restrict
			   abstime) {
  if(!no_follow() && !RECORD_MODE) {
    pthread_mutex_lock(mutex);
    pthread_mutex_unlock(mutex);
    return ETIMEDOUT;
  } else {
    step_and_notify();
    static int(*real_timed)(pthread_cond_t *__restrict __cond,
			   pthread_mutex_t *__restrict __mutex,
			    __const struct timespec *__restrict
			    __abstime) = NULL;
    real_timed = dlvsym(RTLD_NEXT, "pthread_cond_timedwait", "GLIBC_2.3.2");
    return real_timed(cond, mutex, NULL);
  }
}


/*******************************ORIGINAL API FUNCTIONS*************************/

/*All these functions do is call the original non-overridden versions of the functions.*/

int mutex_lock(pthread_mutex_t* mutex)
{
  static int(*real_lock)(pthread_mutex_t*) = NULL;
  if(!real_lock)
    real_lock = dlsym(RTLD_NEXT, "pthread_mutex_lock");
  
  int res = real_lock(mutex);
  return res;
}

int mutex_unlock(pthread_mutex_t* mutex)
{
  static int(*real_unlock)(pthread_mutex_t* mutex) = NULL;
  if(!real_unlock)
    real_unlock = dlsym(RTLD_NEXT, "pthread_mutex_unlock");
  
  int res = real_unlock(mutex);
  return res;
}

int cond_wait(pthread_cond_t* cond, pthread_mutex_t* mut)
{
  
  static int(*real_wait)(pthread_cond_t*, pthread_mutex_t*) = NULL;
    if(!real_wait)
      /*use dlvsym() to prevent the cond variable passed back from being changed to an outdated version*/
      real_wait = dlvsym(RTLD_NEXT, "pthread_cond_wait", "GLIBC_2.3.2");
    
    int res = real_wait(cond, mut);
    return res;
}








/*********************************RUN THREAD*******************************/

/*Run the functions pthread_create was passed*/
void* run_thread(void* arg) {
  
  params* args = (params*) arg;
  mutex_lock(&id_lock);
  put_instrumented_id(pthread_self(), args->thread_id);
  mutex_unlock(&id_lock);
  if(!no_follow()) {  
    wait();
    if(VERBOSE && !no_follow()) {
      fprintf(stderr, "About to run thread %d...\n\n", args->thread_id);
    }
  }
  if(no_follow()) {
    pthread_cond_broadcast(&step_condition);
    mutex_unlock(&step_mutex);
  }
  void* result = args->function((void*)args->other_args);
  step_and_notify();
  if(!no_follow())
	wait();
  step_and_notify();
  return result;
}





/*********************UTILITY FUNCTIONS***********************************/

/*wait*/
void wait()
{
  if(no_follow()) {
    pthread_cond_broadcast(&step_condition);
    mutex_unlock(&step_mutex);
    return;
  }
  /*If in RECORD mode the tool should not be in here anyway*/
  if(RECORD_MODE) {
    return;
  }
 
  int thread_id = get_instrumented_id(pthread_self());
  if(!no_follow()) {
    mutex_lock(&step_mutex);
    while(thread_id != schedule[step])
      {
	if(no_follow()) {
	  pthread_cond_broadcast(&step_condition);
	  mutex_unlock(&step_mutex);
	  return;
	}
	if(terminate_set()) 
	  { 
	    end(); 
	  }
	if(VERBOSE) {
	  fprintf(stderr, "Thread %d sleeping ... schedule[%d], step:%d\n", 
		  thread_id, schedule[step], step);
	}
	if(no_follow()) {
	  pthread_cond_broadcast(&step_condition);
	  mutex_unlock(&step_mutex);
	  return;
	}
	cond_wait(&step_condition, &step_mutex);
      }
  }
  mutex_unlock(&step_mutex);
}


/*step and notify()*/
void step_and_notify()
{
  mutex_lock(&step_mutex);
  step++;
    
    /*Commented out as only used for debugging.*/
  DISABLE=1;
  print_trace();
  DISABLE=0;
  
  update_followed();
  pthread_cond_broadcast(&step_condition);
  mutex_unlock(&step_mutex);
}

/*get the length of the passed schedule*/
int schedule_length()
{

  int *p = schedule;
  int count = 0;
  while(*p != INT_MAX)
    {
      count++;
      p++;
    }

 
  return count;
}


/*gets and set the schedule*/
int get_schedule() 
{
  if(RECORD_MODE) 
    return 2;

  char* env = getenv("KLEE_SCHED");
  
  schedule = get_vector(env);
  
  if(schedule)
    return 1; 
  else
    return 0;

}

/*print the schedule*/
void output_schedule()
{
  printf("[");
  int i;
  for(i = 0; i < schedule_length(); ++i)
    {
      printf("%d ", schedule[i]);
    }
  printf("]\n");
}


/*check the status of the following environment variables to determine what mode the tool should be
  run in*/
void set_follow()
{
  char* val = getenv("FOLLOW_AND_RESUME");
  char* fval = getenv("FOLLOW_AND_TERMINATE");
  char* rval = getenv("RECORD_MODE");
  char* mval = getenv("MONITOR_MODE");
  if(!val && !fval && !rval && !mval)
    {
      return;
    }
  else if(val)
    {
      mutex_lock(&end_lock);
      FOLLOW_TO_END = 1;
      mutex_unlock(&end_lock);
      if(VERBOSE) {
	fprintf(stderr, "FOLLOW_TO_END flag is active\n");
      }
    } 
  else if(fval)
    {
      mutex_lock(&terminate_lock);
      FOLLOW_AND_TERMINATE = 1;
      mutex_unlock(&terminate_lock);
      if(VERBOSE) {
	fprintf(stderr, "FOLLOW_AND_TERMINATE flag is active\n");
      }
    }
  else if(rval) {
    mutex_lock(&end_lock);
    RECORD_MODE = 1;
    mutex_unlock(&end_lock);
    if(VERBOSE) {
      fprintf(stderr, "RECORD flag set\n");
    }
  } 
  if(mval){
    mutex_lock(&end_lock);
    MONITOR_MODE = 1;
    mutex_unlock(&end_lock);
    if(VERBOSE) {
      fprintf(stderr, "MONITOR flag set\n");
    }
  }
}

/*check flag and step*/
int spoken = 0;
int no_follow()
{
  mutex_lock(&nofollow);
  if(FOLLOW_TO_END && step >= schedule_length()) {
    if(!spoken) {
      fprintf(stderr, "Program has run to the end of the schedule and is resuming normal execution ...\n");
      spoken++;
    }
    mutex_unlock(&nofollow);
    pthread_cond_broadcast(&step_condition);
    return 1;
  }
  mutex_unlock(&nofollow);
  return 0;
}
/*check is VERBOSE mode is active*/
void check_verbose()
{
  char* v = getenv("KLEE_VERBOSE_MODE");
  if(!v)
    {
      return;
    }
  else if(v)
    {
      VERBOSE = 1;
      fprintf(stderr, "Running in verbose mode..................\n");
    }
}

int terminate_set() {
  mutex_lock(&nofollow);
  if(FOLLOW_AND_TERMINATE && step >= schedule_length()) {
    mutex_unlock(&nofollow);
    return 1;
  }
  mutex_unlock(&nofollow);
  return 0;
}

void end() {
  fprintf(stderr, "The program has run to the end of the schedule: ");
  mutex_lock(&output_mutex);
  output_schedule();
  mutex_unlock(&output_mutex);
  fprintf(stderr, "A bug was found at the end of this schedule at step: %d\n",
	  step);
  mutex_lock(&output_mutex);
  print_followed();
  mutex_unlock(&output_mutex);
  //exit code 4 for matching schedules
  if(schedules_matched(schedule, schedule_length())) {
    exit(4);
  } else {
    //exit code 3 for failure to follow passed schedule
    exit(3);
  }
} 

/*update_followed*/
void update_followed() {
  mutex_lock(&follow_lock);
  add_to_followed(get_instrumented_id(pthread_self()));
  mutex_unlock(&follow_lock);
}

/*monitor thread to check satisfactory progress*/
void* monitor_progress() {
  int last_step = step;
  while(true) {
    sleep(2);
    if(last_step == step) {
      /*if there has been no change after execution, assume satisfactory progress has
	not been made and report a deadlock*/
      fprintf(stderr, "\n\nPotential deadlock detected!\nTool will now exit with error code: %d\n\n"
	      , 5);
      print_followed();
      write_to_file();

      exit(5);
    } else {
      last_step = step;
    }
  }
}

