#include "Mutex.h"

#if defined(_BUILD_LINUX_)

Mutex::Mutex()
{
    pthread_mutex_init(&mutexLock, 0);
}

#endif

#if defined(_BUILD_LINUX_)

Mutex::~Mutex()
{
    pthread_mutex_destroy(&mutexLock);
}

#endif

#if defined(_BUILD_LINUX_)

int Mutex::Lock()
{
    return pthread_mutex_lock(&mutexLock);
}

#endif


#if defined(_BUILD_LINUX_)

int Mutex::Unlock()
{
    return pthread_mutex_unlock(&mutexLock);
}

#endif

#if defined(_BUILD_LINUX_)

int Mutex::TryLock()
{
    return pthread_mutex_trylock(&mutexLock);
}

#endif
