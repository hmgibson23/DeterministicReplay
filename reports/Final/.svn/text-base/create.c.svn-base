int pthread_create (pthread_t* thread,
		    __const pthread_attr_t* attr, 
		    void *(*start) (void *), 
		    void *arg) {

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
      output_schedule();
      mutex_lock(&id_lock);
      put_instrumented_id(pthread_self(), 0);
      mutex_unlock(&id_lock);
    }

  /*set up the thread arguments*/
  params* args = malloc(sizeof(params));
  args->thread_id = current_id;
  args->other_args = arg;
  args->function = start;
  current_id++;
  int rc;
  
  static int (*real_create)(pthread_t * , pthread_attr_t *, void * (*start)(void *), void *) = NULL;

  if (!real_create)
    real_create = dlsym(RTLD_NEXT, "pthread_create");
  
  mutex_lock(&id_lock);
  mutex_unlock(&id_lock);
  rc = real_create(thread, NULL, run_thread, args);
 
    step_and_notify();
    wait();
  return rc;
}
