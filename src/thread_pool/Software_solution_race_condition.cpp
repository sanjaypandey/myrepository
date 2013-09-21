/*
 * Software_solution_race_condition.cpp
 *
 *  Created on: May 14, 2013
 *      Author: pandit
 */
#include "constant.h"
#define LOOP 15
#define N 2
#define INTRESTED 1
#define NOTINTRESTED 0

int flag=0;
int thread_status[N];

/**
 * This was the first software solution given for insuring the mulual exclution between the processes.
 * this is called strict altration technique for insuring the mutual exclution.
 *
 * It insures the mutual exclution and strict altration but it does not ensure the FOURTH PROPERTY
 * of solution , which tells that no process should block the other process.., while it is running in
 * it's crictical section.
 *
 * Solution for the above problem was given by PETERSON.s
 *
 */


//void* thraed1(void *arg)
//{
//
//	int i=*(int *)arg;
//	for(int j=0;j<LOOP;j++)
//	{
//	  while(flag!=0);
//
//	  /**
//	   *
//	   * Excuting in the critical section..
//	   */
//	  printf("\nThread:%d is excuting in the critical section..\n",i);
//	  for(int i=0;i<500000;i++);
//	  printf("\nThread:%d is laeving in the critical section..\n\n\n",i);
//	  flag=1;
//	  /**
//	   *
//	   * Excuting in the Non-critical section..
//	   *
//	   */
//	  printf("\nThread:%d is excuting in the non-critical section..\n",i);
//	  for(int i=0;i<500000;i++);
//	  printf("\nThread:%d is laeving in the non-critical section..\n\n\n",i);
//
//	}
//	return NULL;
//}
//
//void* thraed2(void *arg)
//{
//   int i=*(int *)arg;
//   for(int j=0;j<LOOP;j++)
//   {
//	   while(flag!=1);
//
//	   	  /**
//	   	   *
//	   	   * Excuting in the critical section..
//	   	   */
//	   	  printf("\nThread:%d is excuting in the critical section..\n",i);
//	   	  for(int i=0;i<500000;i++);
//	   	  printf("\nThread:%d is laeving in the critical section..\n\n\n",i);
//	   	  flag=0;
//	   	  /**
//	   	   *
//	   	   * Excuting in the Non-critical section..
//	   	   *
//	   	   */
//	   	  printf("\nThread:%d is excuting in the non-critical section..\n",i);
//	   	  for(int i=0;i<500000;i++);
//	   	  printf("\nThread:%d is laeving in the non-critical section..\n\n\n",i);
//
//   }
//   return NULL;
//}



/**
 * PETERSON's solution for insuring the mutual exclution ,between the processes.s
 */

void* thraed1(void *arg)
{

	int i=*(int *)arg;
	int other=N-i-1;
	for(int j=0;j<LOOP;j++)
	{
		flag=0;
		thread_status[i]=INTRESTED;
		while(flag!=0&&thread_status[other]);
	  /**
	   *
	   * Excuting in the critical section..
	   */
	  printf("\nThread:%d is excuting in the critical section..\n",i);
	  for(int i=0;i<500000;i++);
	  printf("\nThread:%d is laeving  the critical section..\n\n\n",i);
	  flag=1;
	  thread_status[i]=NOTINTRESTED;
	  /**
	   *
	   * Excuting in the Non-critical section..
	   *
	   */
	  printf("\nThread:%d is excuting in the non-critical section..\n",i);
	  for(int i=0;i<500000;i++);
	  printf("\nThread:%d is laeving  the non-critical section..\n\n\n",i);

	}
	return NULL;
}

void* thraed2(void *arg)
{
   int i=*(int *)arg;
   int other=N-i-1;
   for(int j=0;j<LOOP;j++)
   {

	   	   	flag=1;
	   		thread_status[i]=INTRESTED;
	   		while(flag!=1&&thread_status[other]);

	   	  /**
	   	   *
	   	   * Excuting in the critical section..
	   	   */
	   	  printf("\nThread:%d is excuting in the critical section..\n",i);
	   	  for(int i=0;i<500000;i++);
	   	  printf("\nThread:%d is laeving  the critical section..\n\n\n",i);
	   	  flag=0;
	   	  thread_status[i]=NOTINTRESTED;
	   	  /**
	   	   *
	   	   * Excuting in the Non-critical section..
	   	   *
	   	   */
	   	  printf("\nThread:%d is excuting in the non-critical section..\n",i);
	   	  for(int i=0;i<500000;i++);
	   	  printf("\nThread:%d is laeving  the non-critical section..\n\n\n",i);

   }
   return NULL;
}

void driver()
{
	pthread_t tid[2];
	int i,j,k;
	for(i=0;i<2;i++)
	{

		if(i==0){
			j=i;
			pthread_create(&tid[i],NULL,thraed1,&j);

		}
		else{
			k=i;
			pthread_create(&tid[i],NULL,thraed2,&k);
		}
	}

	for(i=0;i<2;i++)
	{
		if(i==0)
			pthread_join(tid[i],NULL);
		else
			pthread_join(tid[i],NULL);
	}

}



