#ifndef _CONFIGURATION_WRITER_DEF_H_
#define _CONFIGURATION_WRITER_DEF_H_
#include "ConfigurationNode.h"
#include <Filesystem/File.h>

class ConfigurationWriter
{
private:

    static void startNewLine(unsigned int depth, DataOutputStream* outs);

public:

    static void outputNode(ConfigurationNode* Node, unsigned int depth, DataOutputStream* outs);

};

#endif //_CONFIGURATION_WRITER_DEF_H_
