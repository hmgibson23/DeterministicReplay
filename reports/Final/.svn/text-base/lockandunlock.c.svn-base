int pthread_mutex_lock(pthread_mutex_t *mutex)
{
  int res;
  if(!no_follow()){
    int id = get_instrumented_id(pthread_self());
    step_and_notify();
    wait();
    
    if(pthread_mutex_trylock(mutex))
      {
	return 1;
      }
    
    if(!no_follow()) {
      step_and_notify();
      wait();
    }
    
    
    if(pthread_mutex_trylock(mutex))
      {
	return 1;
      }
    
    assert(false);
  } else {
    res = mutex_lock(mutex);
    return res;
  }
}


int pthread_mutex_unlock(pthread_mutex_t* mutex)
{
  if(!no_follow()) {
    step_and_notify();
    wait();


    int id = get_instrumented_id(pthread_self());

  }

  int res = mutex_unlock(mutex);
  
  return res;
}
