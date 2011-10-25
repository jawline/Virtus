#include "Thread.h"

Thread::Thread()
{
    m_threadArg = 0;
    m_startedThread = false;
}

Thread::~Thread()
{

}

void Thread::start(void* Arg)
{
    m_threadArg = Arg;

    #if defined(_BUILD_LINUX_)

        pthread_create( &this_thread, NULL, (void* (*)(void*)) Thread::entryPoint, this);

    #endif
}

void Thread::enter()
{
    run(m_threadArg);
}

void Thread::entryPoint(void* This)
{
    Thread* NewThread = (Thread*) This;

    NewThread->enter();
}
