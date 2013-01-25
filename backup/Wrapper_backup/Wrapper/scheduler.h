
/*global schedule*/
int schedule[] = {
  #include "schedules/schedule#2.h"
};


int schedule_size()
{
  int count = 0;
  int* sched = schedule;
  while(*sched != 0)
    {
      count++;
      sched++;
    }

  //printf("Schedule size = %d\n", count);

  return count;
}

void print_schedule()
{
  schedule_size();
  int i = 0;
  printf("Schedule is : ");
  printf("[");
  for(i = 0; i < schedule_size(); ++i)
    {
      printf("%d ", schedule[i]); 
    }
  printf("]\n");
}

/*check if the thread is still in the schedule*/
int thread_in_schedule(int i)
{
  int j;
  for(j = 0; j < schedule_size(); ++j)
    {
      if(schedule[j] == i)
	return 1;
    }

  return 0;
}
