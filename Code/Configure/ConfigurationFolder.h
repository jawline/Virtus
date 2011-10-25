#ifndef _CONFIGURATION_FOLDER_DEF_H_
#define _CONFIGURATION_FOLDER_DEF_H_
#include "ConfigurationNode.h"
#include <vector>

/* A implementation of a configuration folder, it has a number of children pointers to them and a name */

class ConfigurationFolder : public ConfigurationNode
{

private:

    std::vector <ConfigurationNode*> m_childNodes;

    void freeChildren();

public:

    ConfigurationFolder(const char* Name);
    ~ConfigurationFolder();

    int getType() { return configurationFolder; }

    void addChild(ConfigurationNode* node);

    ConfigurationNode* getChild(unsigned int iter) { return m_childNodes.at(iter); }
    size_t numberOfChildren() { return m_childNodes.size(); }

};

#endif //_CONFIGURATION_FOLDER_DEF_H_
