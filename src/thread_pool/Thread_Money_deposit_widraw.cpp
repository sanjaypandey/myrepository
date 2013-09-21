/*
 * Thread_Money_deposit_widraw.cpp
 *
 *  Created on: May 10, 2013
 *      Author: pandit
 */
#include "constant.h"

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

int balance =500;
void* deposit (void *sum)
{


	for(int i=0;i<500000000;i++);
    int rc=pthread_mutex_lock(&mutex);
	int currbalance=balance ;                    /** read balance */
	printf("\nBefore deposite:%d\n",balance);
	currbalance+=*(int *)sum ;
	balance=currbalance ; 						/** write balance  */
	printf("\n After deposite:%d\n",balance);
    pthread_mutex_unlock(&mutex);
	return NULL;
}
void* withdraw (void *sum)
{

	int rc=pthread_mutex_lock(&mutex);
	int currbalance=balance ;   				/** read balance */
	if ( currbalance >0){
		printf("\n Before witdraw:%d\n",balance);
		currbalance=currbalance-*(int *)sum ;
	}
		balance=currbalance ; 					/**  write balance */
		//for(int i=0;i<5000000;i++);
	printf("\n After withdraw:%d\n",balance);
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void driver()
{
	int i=0,moneyD=100,moneyW=200,m=300;
	pthread_t tpid[3];
	while(i<3)
	{
		int err;
		switch(i)
		{
			case 0: err=pthread_create(&tpid[i],NULL,&deposit,&moneyD);
					break;
			case 1: err=pthread_create(&tpid[i],NULL,&withdraw,&moneyW);
					break;

			case 2:
				    err=pthread_create(&tpid[i],NULL,&deposit,&m);
					break;

			default:break;
		}

		if(err==0)
			printf("\nthread created successfully...");
		else
			printf("\n some problem in creating the thread..");
		i++;
	}
    for(i=0;i<3;i++)
    	pthread_join(tpid[i],NULL);
    printf("\nfinal value of balance:%d\n",balance);
}



