#ifndef _CONFIGURATION_STRING_DEF_H_
#define _CONFIGURATION_STRING_DEF_H_
#include "ConfigurationNode.h"

/* A implementation of a configuration string, it has a string attached to a get function */

class ConfigurationString : public ConfigurationNode
{
private:

    char* m_stringData;

    void freeStringData();

public:

    ConfigurationString(const char* name, const char* stringValue);
    ~ConfigurationString();

    int getType() { return configurationString; }

    //Get the string data
    const char* getStringData() { return m_stringData; }

    //Set the string data
    void setStringData(const char* data);

};

#endif //_CONFIGURATION_STRING_DEF_H_
