int pthread_mutex_lock(pthread_mutex_t *mutex)
{
  int res;
  if(!no_follow() && !RECORD_MODE){
    int id = get_instrumented_id(pthread_self());
    step_and_notify(); 
    wait();
    if(pthread_mutex_trylock(mutex))
      {
	return 0;
      } 


     if(no_follow()) {
       return mutex_lock(mutex);
     }


     step_and_notify();  
     wait();


     if(pthread_mutex_trylock(mutex))
       {
	return 0;
      }
    assert(false);
  } else {
    step_and_notify();
    res = mutex_lock(mutex);
    return res;
  }
}
