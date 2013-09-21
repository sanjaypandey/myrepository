/*
 * handler_thread.h
 *
 *  Created on: May 11, 2013
 *      Author: pandit
 */

#ifndef HANDLER_THREAD_H
#define HANDLER_THREAD_H


/* handler thread parameters structure.                      */
/* this is used to pass a thread several parameters,         */
/* even thought a thread's function gets only one parameter. */
struct handler_thread_params {
    int thread_id;                      /* 'id' of thread                  */
    pthread_mutex_t* request_mutex;     /* mutex to access requests queue. */
    pthread_cond_t*  got_request;       /* condition variable of queue.    */
    struct requests_queue* requests;    /* queue of pending requests.      */
};
/* a handler thread's main loop function */
extern void*
handle_requests_loop(void* thread_params);


#endif /* HANDLER_THREAD_H_ */
