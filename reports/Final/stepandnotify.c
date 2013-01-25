/*step and notify()*/
void step_and_notify()
{

//mutex_lock and mutex_unlock call the original pthread_mutex_lock and pthread_mutex_unlock functions through
//the dlsym() handle
  mutex_lock(&step_mutex);
  step++;
  pthread_cond_broadcast(&step_condition);
  mutex_unlock(&step_mutex);
}




