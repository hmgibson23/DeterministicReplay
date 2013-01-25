/*wait*/
void wait()
{
  int thread_id = get_instrumented_id(pthread_self());

  if(FOLLOW_TO_END)
    {
      mutex_lock(&follow);
      check_status();
      mutex_unlock(&follow);
    }
  


  if(!no_follow()) {
    mutex_lock(&step_mutex);
    while(thread_id != schedule[step])
      {
	check_status();
	if(no_follow()) {
	  mutex_unlock(&step_mutex);
	  return;
	} else if(terminate_set()) { end(); }
	if(VERBOSE) {
	  fprintf(stderr, "Thread %d sleeping ... schedule[%d], step:%d\n", 
		  thread_id, schedule[step], step);
	}
	cond_wait(&step_condition, &step_mutex);
      }

    mutex_unlock(&step_mutex);
  }
}


