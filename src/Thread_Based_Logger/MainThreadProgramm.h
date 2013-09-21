#ifndef LOGGER_MainThreadProgramm_GCC_X86_64_H
#define LOGGER_MainThreadProgramm_GCC_X86_64_H

#include "Constants.h"
#include "requests_queue.h"         /* requests queue routines/structs       */
#include "handler_thread.h"         /* handler thread functions/structs      */
#include "handler_threads_pool.h"   /* handler thread list functions/structs */
/* number of initial threads used to service requests, and max number */
/* of handler threads to create during "high pressure" times.         */
#define NUM_HANDLER_THREADS 3
#define MAX_NUM_HANDLER_THREADS 14
/* number of requests on the queue warranting creation of new threads */
#define HIGH_REQUESTS_WATERMARK 15
#define LOW_REQUESTS_WATERMARK 3

/**
This method is used to populate the threads for handling the multiple request..
*/
void driver();

#endif
