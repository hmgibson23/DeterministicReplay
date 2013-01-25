int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex)
{
  step_and_notify();
  if(VERBOSE) {
    fprintf(stderr, "Thread %d waiting on condition....\n", get_instrumented_id(pthread_self()));
  }
  mutex_unlock(mutex);
  wait();
  mutex_lock(mutex);
  step_and_notify();

  return 0;
}
