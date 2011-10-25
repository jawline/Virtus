#include "Job.h"
#include <string.h>

WorkerJob::WorkerJob()
{

    m_jobStatus = jobStatusWaiting;

    m_errorDetails = 0;

    return;
}

WorkerJob::~WorkerJob()
{
    if (m_errorDetails != 0)
    {

        delete[] m_errorDetails;

    }

    m_errorDetails = 0;

    return;
}

void WorkerJob::setError(const char* Details)
{

    m_jobStatus = jobStatusError;

    m_errorDetails = new char[strlen(Details) + 1];

    strncpy(m_errorDetails, Details, strlen(Details) + 1);

}

const char* WorkerJob::getErrorDetails()
{

    return m_errorDetails;

}
