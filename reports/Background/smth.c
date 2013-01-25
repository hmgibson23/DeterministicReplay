/*wait*/
void wait(int thread_id)
{
  pthread_mutex_lock(&step_mutex);
  while(thread_id != schedule[step] && step < schedule_size()-1)
    {    
      fprintf(stderr, "Thread %d, sleeping...\n", thread_id);
      pthread_cond_wait(&condition, &step_mutex);
    } 
  pthread_mutex_unlock(&step_mutex);

}


/*step and notify */
void step_and_notify()
{

  pthread_mutex_lock(&step_mutex);

  step++;


  pthread_cond_broadcast(&condition);
  pthread_mutex_unlock(&step_mutex);
}
