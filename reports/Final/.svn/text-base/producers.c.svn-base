#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<sys/time.h>

int	q[5];						
int	first  = 0;					
int last   = 0;					
int	numInQ = 0;					
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;	
pthread_mutex_t empty = PTHREAD_MUTEX_INITIALIZER;	

void putOnQ(int x) {
	pthread_mutex_lock(&mutex);		
	q[first] = 1;					
	first = (first+1) % 5;
	numInQ++;						
	pthread_mutex_unlock(&mutex);	
	pthread_mutex_unlock(&empty);	
}

int getOffQ(void) {
	int thing;						
	while (numInQ == 0) pthread_mutex_lock(&empty);
    pthread_mutex_lock(&mutex);		
    thing = q[last];				
    last = (last+1) % 5;
    numInQ--;						
    pthread_mutex_unlock(&mutex);	
	return thing;
}

void * prod(void *arg) {
	int	mynum;						
	int	i;
	mynum = *(int *)arg;
	for (i = 0; i < 3; i++) {
		sleep(i);					
		puts("putting");
		putOnQ(i+100*mynum);		
		printf("put %d on queue\n", i+100*mynum);
	}
 }

void * con(void *arg) {
	int	i;
	int	stuff;						
	for (i = 0; i < 6; i++) {
		puts("getting");
		stuff = getOffQ();			
		printf("got %d from queue\n", stuff);
	}
 }

int main()
{
 pthread_t threadp1, threadp2, threadc1;
 int zero = 0;  int one = 1;

 pthread_create(&threadp1, NULL, prod, &zero);	
 pthread_create(&threadp2, NULL, prod, &one);	
 pthread_create(&threadc1, NULL, con,  NULL);	

 return 0;							
}
