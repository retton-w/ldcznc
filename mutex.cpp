
#include "mutex.h"

Mutex::Mutex()
{
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
        pthread_mutex_init(&_mutex, &attr);
        pthread_mutexattr_destroy(&attr);
}


Mutex::~Mutex()
{

}


bool Mutex::lock()
{
        if (pthread_mutex_lock(&_mutex)) {
                return false;
        }
        return true;
}


bool Mutex::tryLock()
{
        /* int rc = pthread_mutex_trylock(&_mutex);
           bool result = (rc == 0);
           return result;
        */
        return true;
}


bool Mutex::unlock()
{
        int rc = pthread_mutex_unlock(&_mutex);
        return (rc == 0);
}

