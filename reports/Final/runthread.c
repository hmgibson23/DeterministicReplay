void* run_thread(void* arg) {

  params* args = (params*) arg;
  mutex_lock(&id_lock);
  put_instrumented_id(pthread_self(), args->thread_id);
  mutex_unlock(&id_lock);
  wait();

  void* result = args->function((void*)args->other_args);
  step_and_notify();


  return result;
}
