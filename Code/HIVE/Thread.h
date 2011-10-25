#ifndef _THREAD_DEF_H_
#define _THREAD_DEF_H_

#if defined(_BUILD_LINUX_)

    #include <pthread.h>

#else

    #error "Unsupported platform (Thread.h)"

#endif

class Thread
{
    private:

        #if defined(_BUILD_LINUX_)

            pthread_t this_thread;

        #endif

        void* m_threadArg;
        bool m_startedThread;

        static void EntryPoint(void* thread);

    protected:
        void Enter();
        virtual void Run(void* Arg) = 0;

    public:
        Thread();
        virtual ~Thread();

        void Start(void* Arg);
        bool Running() { return m_startedThread; }
};

#endif //_THREAD_DEF_H_
