#include "Bee.h"
#include "Hive.h"
#include <stdio.h>
#include <iostream>

WorkerBee::WorkerBee(int beenm)
{

    m_beeNumber = beenm;

    beeStatusAccessMutex.Lock();

        m_beeStatus = beeIdle;

    beeStatusAccessMutex.Unlock();
}

WorkerBee::~WorkerBee()
{

}

void WorkerBee::Run(void* Args)
{
        printf("Worker bee %i running\n", m_beeNumber);

        m_hive = static_cast<BeeHive*>(Args);

        while (1)
        {
            WorkerJob* newJob = m_hive->beeRequestNextJob();

            if (newJob != 0)
            {

                beeStatusAccessMutex.Lock();

                    m_beeStatus = beeProcessing;

                beeStatusAccessMutex.Unlock();

                    newJob->processJob();

                    if (newJob->getStatus() == jobStatusError)
                    {
                        printf("Job exit with error %s\n", newJob->getErrorDetails());
                    }

                beeStatusAccessMutex.Lock();

                    m_beeStatus = beeIdle;

                beeStatusAccessMutex.Unlock();
            }
            else
            {
                //Idle
            }

        }
}

int WorkerBee::getBeeStatus()
{
    beeStatusAccessMutex.Lock();

        int l_beeStatus = m_beeStatus;

    beeStatusAccessMutex.Unlock();

    return l_beeStatus;
}
