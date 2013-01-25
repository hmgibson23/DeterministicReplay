int mutex_lock(pthread_mutex_t* mutex)
{
    static int(*real_lock)(pthread_mutex_t*) = NULL;
    if(!real_lock)
        real_lock = dlsym(RTLD_NEXT, "pthread_mutex_lock");
    
    int res = real_lock(mutex);
    return res;
}

int mutex_unlock(pthread_mutex_t* mutex)
{
    static int(*real_unlock)(pthread_mutex_t* mutex) = NULL;
    if(!real_unlock)
        real_unlock = dlsym(RTLD_NEXT, "pthread_mutex_unlock");
    
    int res = real_unlock(mutex);
    return res;
}

int cond_wait(pthread_cond_t* cond, pthread_mutex_t* mut)
{
    
    static int(*real_wait)(pthread_cond_t*, pthread_mutex_t*) = NULL;
    if(!real_wait)
    /*use dlvsym() to prevent the cond variable passed back from being changed to an outdated version*/
        real_wait = dlvsym(RTLD_NEXT, "pthread_cond_wait", "GLIBC_2.3.2");
    
    int res = real_wait(cond, mut);
    return res;
}
