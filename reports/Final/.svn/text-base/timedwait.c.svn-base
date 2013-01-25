int pthread_cond_timedwait(pthread_cond_t *__restrict cond,
			   pthread_mutex_t *__restrict mutex,
			   __const struct timespec *__restrict
			  abstime) {
    pthread_mutex_lock(mutex);
    pthread_mutex_unlock(mutex);
    return ETIMEDOUT;
}
