#ifndef _WORKER_JOB_DEF_H_
#define _WORKER_JOB_DEF_H_

const static int jobStatusWaiting = 1;
const static int jobStatusRunning = 2;
const static int jobStatusDone = 3;
const static int jobStatusError = 4;

/**
 * @brief Virtual class for a job for a worker bee
 *
 */
class WorkerJob
{
protected:
    int m_jobStatus;
    char* m_errorDetails;

    void setError(const char* Details);

public:

    WorkerJob();
    virtual ~WorkerJob();

    int getStatus()
    {
        return m_jobStatus;
    }

    const char* getErrorDetails();

    virtual void processJob() = 0;

};

#endif //_WORKER_JOB_DEF_H_
