#include <pthread.h>
#include <stdio.h>
#include "constant.h"
#define NUM_THREADS     2

pthread_key_t key;
pthread_once_t once=PTHREAD_ONCE_INIT;

void clean_memory(void *arg)
{
  if(arg)
  {
	  printf("\nI am going to clean the memory..and value is:%d\n",*(int *)arg);
	  free(arg);
  }
  else
	  printf("\nNo memotry for clean up..\n");
}

void cleanup_after_malloc(void* allocated_memory)
{
	printf("\nwe are inside the clean_after_malloc function\n");
    if (allocated_memory){
        free(allocated_memory);
        allocated_memory=NULL;
    }
}

void initialize( )
{
   	printf("\nit is in the intialization block..\n");
}

void *PrintHello(void *threadid)
{

	pthread_once(&once,initialize);
   int old_cancel_type;
   long tid;
   int *k=(int *)malloc(sizeof(int));
   *k=500;
   tid = (long)threadid;
   pthread_cleanup_push(cleanup_after_malloc,k);
   pthread_setspecific(key,k);
   //printf("Hello World! It's me, thread #%ld!  and  value is:%d\n", tid,*(int *)pthread_getspecific(key));

   pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old_cancel_type);
   pthread_cleanup_pop(1);
   /* restore the thread's previous cancellation mode.   */
   pthread_setcanceltype(old_cancel_type, NULL);

   pthread_exit(NULL);
   return NULL;
}

void *PrintHello1(void *threadid)
{
   long tid;
   int k=200;
   tid = (long)threadid;
   pthread_setspecific(key,&k);
   printf("Hello World! It's me, thread #%ld! and value is:%d\n", tid,*(int *)pthread_getspecific(key));
   pthread_exit(NULL);
   return NULL;
}

void driver()
{
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   pthread_key_create(&key,clean_memory);
   for(t=0; t<NUM_THREADS; t++){
      printf("In main: creating thread %ld\n", t);
      if(t==0)
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
      else
      rc= pthread_create(&threads[t], NULL, PrintHello, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   /* Last thing that main() should do */
   pthread_exit(NULL);
}
