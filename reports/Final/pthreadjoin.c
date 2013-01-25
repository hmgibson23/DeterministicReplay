int pthread_join(pthread_t tid, void** value_ptr)
{ 
    step_and_notify();
    wait();
    

    int res = pthread_tryjoin_np(tid,value_ptr);
    if(res != EBUSY)
      {
	return res;
      }
    
    step_and_notify();
    wait();
    
    
    res = pthread_tryjoin_np(tid,value_ptr);
    if(res != EBUSY)
      {
	return res;
      }
    assert(false && "Invalid Schedule!");
}
