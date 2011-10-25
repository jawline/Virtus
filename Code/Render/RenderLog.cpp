#include "RenderLog.h"
#include <time.h>

void RenderLog::writeToLogFile(const char* line)
{
    if (m_logfileHandle == 0) return;

    printf("Renderlog: %s\n", line);
    fputs(line, m_logfileHandle);
    fflush(m_logfileHandle);
}

RenderLog::RenderLog(const char* outputFile)
{
  printf("Opening render log\n");

  m_logfileHandle = fopen(outputFile, "w");

  if (m_logfileHandle == 0) return;

  char Buffer[4098];

  sprintf(Buffer, "Render log\n");
  writeToLogFile(Buffer);

  //Get the current time
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );

  //Write the current date and time
  sprintf(Buffer, "The date/time is %s\n", asctime(timeinfo));
  writeToLogFile(Buffer);

}

RenderLog::~RenderLog()
{
    printf("Log closed\n");
    fclose(m_logfileHandle);
    m_logfileHandle = 0;
}
