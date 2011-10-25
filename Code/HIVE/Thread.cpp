#include "Thread.h"

Thread::Thread()
{
    m_threadArg = 0;
    m_startedThread = false;
}

Thread::~Thread()
{

}

void Thread::Start(void* Arg)
{
    m_threadArg = Arg;

    #if defined(_BUILD_LINUX_)

        pthread_create( &this_thread, NULL, (void* (*)(void*)) Thread::EntryPoint, this);

    #endif
}

void Thread::Enter()
{
    Run(m_threadArg);
}

void Thread::EntryPoint(void* This)
{
    Thread* NewThread = (Thread*) This;

    NewThread->Enter();
}
