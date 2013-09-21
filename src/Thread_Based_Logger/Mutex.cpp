#include "Mutex.h"
#include "MutexImplementation.h"

/**
Imlementation for Normal Mutex.
*/

Mutex::Mutex()
{
    //printf("\ninside the costructor of Mutex\n");
   _impl=new MutexImpl(0);
}


Mutex::~Mutex()
{
    //printf("\n inside the destrucor of Mutex class\n");
    if(_impl!=NULL)
    delete _impl;
}


void Mutex::lock()
{
  if(_impl!=NULL)
  {
     _impl->mutex_lock();
   }
}

void Mutex::unlock()
{
  if(_impl!=NULL)
  {
     _impl->mutex_unlock();
  }
}

bool Mutex::tryLock()
{
    if(_impl!=NULL)
    return _impl->mutex_try_lock();
}

bool Mutex::unlockNoThrow()
{
    try
    {
        if(_impl!=NULL)
        _impl->mutex_unlock();
        return true;
    }
    catch(...)
    {}

    return false;
}

/**
implementation fo Recursive mutex.
*/

RecursiveMutex::RecursiveMutex()
{
   _impl=new MutexImpl(1);
}

RecursiveMutex::~RecursiveMutex()
{
    delete _impl;
}


void RecursiveMutex::lock()
{
    _impl->mutex_lock();
}


bool RecursiveMutex::tryLock()
{
    return _impl->mutex_try_lock();
}


void RecursiveMutex::unlock()
{
    _impl->mutex_unlock();
}


bool RecursiveMutex::unlockNoThrow()
{
    try
    {
        _impl->mutex_unlock();
        return true;
    }
    catch(...)
    {}

    return false;
}

