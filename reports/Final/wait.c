
/*wait*/
void wait()
{
  int thread_id = get_instrumented_id(pthread_self());

  mutex_lock(&step_mutex);
    while(thread_id != schedule[step])
      {
	//cond_wait calls the original pthread_cond_wait function through the dlsym() handle
	cond_wait(&step_condition, &step_mutex);
      }

    mutex_unlock(&step_mutex);
  }
}
