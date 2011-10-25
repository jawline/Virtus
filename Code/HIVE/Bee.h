#ifndef _WORKER_BEE_DEF_H_
#define _WORKER_BEE_DEF_H_
#include "Thread.h"
#include "Job.h"
#include "Mutex.h"
#include "Lock.h"

class BeeHive;

static const int beeIdle = 0;
static const int beeProcessing = 1;

/**
 * @brief Implementation of a thread that takes and processes job from a hive and processes it
 */
class WorkerBee : public Thread
{
private:
    BeeHive* m_hive;

    //For output/debugging purposes
    int m_beeNumber;

    int m_beeStatus;
    Mutex beeStatusAccessMutex;


public:
    WorkerBee(int number);
    ~WorkerBee();

    /**
     * @brief Entry point overloaded from the base Thread class, this is the first function to be run
     */

    void run(void* Arg);

    void hiveUpdate();

    int getBeeStatus();


};

#endif //_WORKER_BEE_DEF_H_
