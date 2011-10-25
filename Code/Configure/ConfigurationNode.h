#ifndef _CONFIGURATION_NODE_DEF_H_
#define _CONFIGURATION_NODE_DEF_H_
#include <string.h>
#include <locale.h>
#include <stdio.h>

enum configurationNodeType
{
    configurationError = 0,
    configurationFolder,
    configurationString,
    configurationNumber
};

/* The "virtual" configuration node, or the superclass
    All configuration nodes have a name and type
*/

class ConfigurationNode
{

private:
    char* m_nodeName;

public:

    ConfigurationNode(const char* Name);
    virtual ~ConfigurationNode();

    virtual int getType() { return configurationError; }
    const char* getName() { return m_nodeName; }

};

#endif //_CONFIGURATION_NODE_DEF_H_
