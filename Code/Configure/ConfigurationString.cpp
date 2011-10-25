#include "ConfigurationString.h"

void ConfigurationString::freeStringData()
{

    if (m_stringData != 0)
        delete[] m_stringData;

    m_stringData = 0;

    return;

}

void ConfigurationString::setStringData(const char* data)
{

    freeStringData();

    m_stringData = new char[strlen(data) + 1];

    strcpy(m_stringData, data);

}

ConfigurationString::ConfigurationString(const char* name, const char* stringValue) : ConfigurationNode (name)
{

    m_stringData = 0;

    setStringData(stringValue);

}

ConfigurationString::~ConfigurationString()
{

    freeStringData();

}
