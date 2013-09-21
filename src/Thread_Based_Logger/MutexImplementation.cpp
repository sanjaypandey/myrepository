#include "MutexImplementation.h"

/**
Implementation for normal lock.
*/
MutexImpl::MutexImpl(int status=0)
{
    //printf("\ninside the constriutor of Mutex impl\n");
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    if(status==0)
      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    else
      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE );
    int rc = pthread_mutex_init(&_mutex,&attr);
    if (rc != 0)
        throw  string("pthread_mutex_init");

}

MutexImpl::~MutexImpl()
{
   //printf("\ndestructor of mutex impls\n");
   pthread_mutex_destroy(&_mutex);
}

void  MutexImpl::mutex_lock()
{
      int rc = pthread_mutex_lock(&_mutex);
      if( rc != 0 )
       throw string("pthread_mutex_lock failed");
}


void MutexImpl::mutex_unlock()
{
      int rc = pthread_mutex_unlock(&_mutex);
      if( rc != 0 )
       throw  string("pthread_mutex_lock failed");
}

bool MutexImpl::mutex_try_lock()
{
    int rc = pthread_mutex_trylock(&_mutex);
    if( rc != 0 && rc != EBUSY )
        throw string("pthread_mutex_trylock");
    return rc != EBUSY;
}

/**
Implementation for ReaderWriter lock.
*/
ReadWriteMutexImpl::ReadWriteMutexImpl()
{
    int rc = pthread_rwlock_init(&_rwl, NULL);
    if( rc != 0 )
        throw string("pthread_rwlock_init");
}


ReadWriteMutexImpl::~ReadWriteMutexImpl()
{
    pthread_rwlock_destroy(&_rwl);
}


void ReadWriteMutexImpl::readLock()
{
    int rc = pthread_rwlock_rdlock(&_rwl);
    if( rc != 0 )
        throw string("pthread_rwlock_rdlock");
}


bool ReadWriteMutexImpl::tryReadLock()
{
    int rc = pthread_rwlock_tryrdlock(&_rwl);

    if( rc != 0 && rc != EBUSY )
        throw string("pthread_rwlock_tryrdlock");

    return rc != EBUSY;
}


void ReadWriteMutexImpl::writeLock()
{
    int rc = pthread_rwlock_wrlock(&_rwl);
    if( rc != 0)
        throw string("pthread_rwlock_wrlock");
}


bool ReadWriteMutexImpl::tryWriteLock()
{
    int rc = pthread_rwlock_trywrlock(&_rwl);

    if( rc != 0 && rc != EBUSY)
        throw string("pthread_rwlock_trywrlock");

    return rc != EBUSY;
}


void ReadWriteMutexImpl::unlock()
{
    int rc = pthread_rwlock_unlock(&_rwl);
    if( rc != 0 )
        throw string( "pthread_rwlock_unlock");
}
