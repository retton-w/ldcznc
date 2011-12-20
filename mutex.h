#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <pthread.h>

class Mutex
{

public:        
	Mutex();
        ~Mutex();

        bool lock();
        bool tryLock();
        bool unlock();

private:
        pthread_mutex_t _mutex;
};

#endif

