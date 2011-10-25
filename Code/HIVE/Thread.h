#ifndef _THREAD_DEF_H_
#define _THREAD_DEF_H_

#if defined(_BUILD_LINUX_)

    #include <pthread.h>

#else

    #error "Unsupported platform (Thread.h)"

#endif

/**
 * @brief An abstraction of a thread
 *
 */
class Thread
{
    private:

        #if defined(_BUILD_LINUX_)

            pthread_t this_thread;

        #endif

        void* m_threadArg;
        bool m_startedThread;

        static void entryPoint(void* thread);

    protected:
        void enter();
        virtual void run(void* Arg) = 0;

    public:
        Thread();
        virtual ~Thread();

        void start(void* Arg);
        bool running() { return m_startedThread; }
};

#endif //_THREAD_DEF_H_
