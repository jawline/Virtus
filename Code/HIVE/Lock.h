#ifndef _MUTEX_LOCK_DEF_H_
#define _MUTEX_LOCK_DEF_H_
#include "Mutex.h"

class Lock
{
private:
    Mutex* myMutex;

public:

    Lock(Mutex* mutex)
    {
        myMutex = mutex;
        myMutex->Lock();
    }

    ~Lock()
    {
        myMutex->Unlock();
    }

};

#endif //_MUTEX_LOCK_DEF_H_
