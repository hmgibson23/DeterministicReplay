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
