#ifndef _MUTEX_LOCK_DEF_H_
#define _MUTEX_LOCK_DEF_H_
#include "Mutex.h"

/**
 * @brief A mutual exclusion lock used to try and ensure that the threads don't crash each other or the core thread (USE THIS IF ANY JOB INTERACTS WITH DATA TOUCHED BY ANY OTHER THREAD INCLUDING THE START THREAD)
 */

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
