#include <stdio.h>
#include <limits.h>

#include "scheduler.h"

int schedule[];

int schedule_size(int* schedule_1)
{
  int count = 0;
  int* sched = schedule_1;
  while(*sched != INT_MAX)
    {
      count++;
      sched++;
    }
  
  /*count = sizeof(schedule)/sizeof(int);*/

  return count;
}

void print_schedule(int* schedule_1)
{
  schedule_size(schedule_1);
  int i = 0;
  printf("Schedule is : ");
  printf("[");
  for(i = 0; i < schedule_size(schedule_1); ++i)
    {
      printf("%d ", schedule[i]); 
    }
  printf("]\n");
}

/*check if the thread is still in the schedule*/
int thread_in_schedule(int i, int* schedule_1)
{
  int j;
  for(j = 0; j < schedule_size(schedule_1); ++j)
    {
      if(schedule_1[j] == i)
	return 1;
    }

  return 0;
}
