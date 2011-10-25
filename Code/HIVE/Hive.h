#ifndef _BEE_HIVE_DEF_H_
#define _BEE_HIVE_DEF_H_
#include "Job.h"
#include "Lock.h"
#include "Mutex.h"
#include <queue>

class WorkerBee;

class BeeHive
{
private:
    WorkerBee** m_bees;
    unsigned int m_numberOfBees;

    std::queue<WorkerJob*> m_waitingJobs;
    Mutex jobQueueMutex;

public:

    BeeHive(unsigned int numberOfBees);
    ~BeeHive();

    void issueOrders(WorkerJob* Job);
    WorkerJob* beeRequestNextJob();

    bool allIdle();
};

#endif //_BEE_HIVE_DEF_H_
