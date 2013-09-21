#ifndef LOGGER_MUTEX_GCC_X86_64_H
#define LOGGER_MUTEX_GCC_X86_64_H

#include "Constants.h"
/**
This gives the normal lock on mutex variable.
*/
class Mutex
{
        private:
        class MutexImpl* _impl;

        public:
         Mutex();
        ~Mutex();

    /**
    Getting the lock on the mutex variable Through MutexImpl class.
    */
        void lock();
        bool tryLock();
        void unlock();
        bool unlockNoThrow();

        MutexImpl& impl()
        {
           return *_impl;
        }

};

class MutexLock
{
   private:
            Mutex& _mutex;
            bool _isLocked;
   public:
            MutexLock(Mutex& m, bool doLock = true, bool isLocked = false)
            : _mutex(m)
            , _isLocked(isLocked)
            {
                //printf("\ninside the Mutex lock class.\n");
                if(doLock)
                this->lock();
            }

            ~MutexLock()
            {
                if(_isLocked)
                _mutex.unlockNoThrow();
            }

            void lock()
            {
                if(!_isLocked)
                {
                  _mutex.lock();
                  _isLocked = true;
                }
            }

            void unlock()
            {
                if(_isLocked)
                {
                    _mutex.unlock();
                    _isLocked = false;
                }
            }

            Mutex& mutex()
            {
              return _mutex;
            }

            const Mutex& mutex() const
            {
                return _mutex;
            }


};


/**
This is used for providing the recursive lock.
*/
class RecursiveMutex
{
    private:
        class MutexImpl* _impl;

    public:
        RecursiveMutex();
        ~RecursiveMutex();
        void lock();
        bool tryLock();
        void unlock();
        bool unlockNoThrow();

        MutexImpl& impl()
        {
           return *_impl;
        }

};

class RecursiveLock
{
   private:
           RecursiveMutex& _mutex;
           bool _isLocked;
   public:
          RecursiveLock(RecursiveMutex &mutex,bool doLock=true ,bool isLocked=false)
          :_mutex(mutex),_isLocked(isLocked)
          {
              if(!_isLocked)
                this->lock();

          }
          ~RecursiveLock()
          {
              if(_isLocked)
                this->unlockNoThrow();
           }

           void lock()
           {
             if(!_isLocked)
             {
               _mutex.lock();
               _isLocked=true;
             }
           }

           void unlock()
           {
             if(_isLocked)
             {
               _mutex.unlock();
               _isLocked=false;
             }
           }

           void unlockNoThrow()
           {
              if(_isLocked)
             {
               _mutex.unlock();
               _isLocked=false;
             }
           }

           RecursiveMutex& mutex()
           {
              return _mutex;
            }

            const RecursiveMutex& mutex() const
           {
              return _mutex;
            }


};


class ReadWriteMutex
{
  private:
     class ReadWriteMutexImpl* _impl;
  public:
      ReadWriteMutex();
     ~ReadWriteMutex();
      void readLock();
      bool tryReadLock();
      void writeLock();
      bool tryWriteLock();
      void unlock();
      bool unlockNoThrow();
};

/**

This is used to get the lock before reading any file.
*/
class ReadLock
{
   private:
        ReadWriteMutex& _mutex;
        bool _locked;
   public:
        ReadLock(ReadWriteMutex& m, bool doLock = true, bool isLocked = false)
        : _mutex(m)
        , _locked(isLocked)
        {
            if(doLock)
                this->lock();
        }

        ~ReadLock()
        {
            if(_locked)
                _mutex.unlockNoThrow();
        }

        void lock()
        {
            if(!_locked )
            {
                _mutex.readLock();
                _locked = true;
            }
        }

        void unlock()
        {
            if(_locked)
            {
                _mutex.unlock();
                _locked = false;
            }
        }

        ReadWriteMutex& mutex()
        {
            return _mutex;
        }

        const ReadWriteMutex& mutex() const
        {
            return _mutex;
        }
};

/**
This is used to get the write lock on the file before writing into that file.
*/
class WriteLock
{
    public:
        WriteLock(ReadWriteMutex& m, bool doLock = true, bool isLocked = false)
        : _mutex(m)
        , _locked(isLocked)
        {
            if(doLock)
                this->lock();
        }

        ~WriteLock()
        {
            if(_locked)
                _mutex.unlockNoThrow();
        }

        void lock()
        {
            if(!_locked )
            {
                _mutex.writeLock();
                _locked = true;
            }
        }

        void unlock()
        {
            if(_locked)
            {
                _mutex.unlock();
                _locked = false;
            }
        }

        ReadWriteMutex& mutex()
        { return _mutex; }

    private:
        ReadWriteMutex& _mutex;
        bool _locked;

};

#endif
