#ifndef _MUTEX_CLASS_DEF_H_
#define _MUTEX_CLASS_DEF_H_

#if defined(_BUILD_LINUX_)

    #include <pthread.h>

#else

    #error "Unsupported platform (Mutex.h)"

#endif

/**
 * @brief The actual mutex lock used by the lock class
 *
 */
class Mutex
{
    protected:

    #if defined(_BUILD_LINUX_)

        pthread_mutex_t mutexLock;

    #endif

    public:
        Mutex();
        ~Mutex();

        int Lock();
        int Unlock();
        int TryLock();

};

#endif //_MUTEX_CLASS_DEF_H_
