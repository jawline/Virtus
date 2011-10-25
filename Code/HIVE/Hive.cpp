#include "Hive.h"
#include "Bee.h"
#include <stdio.h>
#include <Core/GEngine.h>

BeeHive::BeeHive(unsigned int numberOfBees)
{
    //Create a list of bees
    m_bees = new WorkerBee*[numberOfBees];

    //Create each bee and initialize them
    for (unsigned int i = 0; i < numberOfBees; ++i)
    {

        m_bees[i] = new WorkerBee(i + 1);
        m_bees[i]->Start(this);

    }

    //Set the number of bees
    m_numberOfBees = numberOfBees;

    //Write this into to the log
    char logBuffer[256];
    sprintf(logBuffer, "BeeHive created with %i workers\n", m_numberOfBees);
    GEngine::getInstance()->getLog()->writeData(logBuffer);
}

BeeHive::~BeeHive()
{
    //TODO: KILL BEES FIRST
    GEngine::getInstance()->getLog()->writeData("WARNING: HIVE isn't able to kill bees yet. get on it\n");

    //Free up the bee's
    for (unsigned int i = 0; i < m_numberOfBees; ++i)
    {

        delete m_bees[i];

    }

    delete[] m_bees;

    GEngine::getInstance()->getLog()->writeData("HIVE destroyed\n");

}

bool BeeHive::allIdle()
{

    //Lock the queue mutex while we check how many jobs are waiting
    jobQueueMutex.Lock();

        int size = m_waitingJobs.size();

    //Unlock it once fetching the size is done
    jobQueueMutex.Unlock();

    //If there are waiting jobs then everything is definately idle
    if (size != 0) return false;

    //If not, scan through each bee
    for (unsigned int i = 0; i < m_numberOfBees; ++i)
    {

        //If that bee is busy then return false
        if (m_bees[i]->getBeeStatus() != beeIdle)
        {

            return false;

        }


    }

    //if the code reachs here then everythings free
    return true;
}


void BeeHive::issueOrders(WorkerJob* newJob)
{

    //LOCK ACCESS TO JOB QUEUE/WAIT FOR ANYTHING ELSE TO CLEAR IT
    jobQueueMutex.Lock();

        //Add the job
        m_waitingJobs.push(newJob);

    //UNLOCK IT SO OTHER PEOPLE CAN HAVE A TURN
    jobQueueMutex.Unlock();

}

WorkerJob* BeeHive::beeRequestNextJob()
{
    //LOCK ACCESS TO JOB QUEUE/WAIT FOR ANYTHING TO CLEAR IT
    jobQueueMutex.Lock();

        WorkerJob* nextJob = 0;

        //If there are waiting jobs
        if (m_waitingJobs.size() != 0)
        {
            //Get the next job from the queue
            nextJob = m_waitingJobs.front();
            m_waitingJobs.pop();

        }

    //UNLOCK HERE
    jobQueueMutex.Unlock();

    //Return the job you got
    return nextJob;
}
