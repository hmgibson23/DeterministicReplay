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
  wait();

  /*run the function*/
  void* result = args->function((void*)args->other_args);
 
  /*indicate that the thread's function has been run*/
  step_and_notify();

  return result;
}
