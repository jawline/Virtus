#ifndef _CONFIGURATION_INSTANCE_DEF_H_
#define _CONFIGURATION_INSTANCE_DEF_H_
#include <vector>
#include "ConfigurationNode.h"
#include <Filesystem/File.h>

class ConfigurationInstance
{
private:

    std::vector<ConfigurationNode*> m_Nodes;

    void unloadAndFreeAll();

    File* m_instanceFile;

    void loadFromPath();
    void writeToPath();

    ConfigurationNode* internalSearchNode(const char* Path, ConfigurationNode* cNode);

public:

    ConfigurationInstance(File* fileToLoad);
    ~ConfigurationInstance();

    void saveInstance();

    ConfigurationNode* getNode(unsigned int i);
    unsigned int getNumberOfNodes();

    ConfigurationNode* getNode(const char* Path);
};

#endif //_CONFIGURATION_INSTANCE_DEF_H_
