#ifndef LOGGER_MUTEXIMPL_GCC_X86_64_H
#define LOGGER_MUTEXIMPL_GCC_X86_64_H

#include "Constants.h"

class MutexImpl
{
   pthread_mutex_t _mutex;
   public:
   MutexImpl(int status);
   ~MutexImpl();
   void mutex_lock();
   void mutex_unlock();
   bool mutex_try_lock();
};

class ReadWriteMutexImpl
{
    public:
        ReadWriteMutexImpl();
        ~ReadWriteMutexImpl();
        void readLock();
        bool tryReadLock();
        void writeLock();
        bool tryWriteLock();
        void unlock();

    private:
        pthread_rwlock_t _rwl;
};

#endif
