#include "ConfigurationFolder.h"

ConfigurationFolder::ConfigurationFolder(const char* Name) : ConfigurationNode(Name)
{
}

ConfigurationFolder::~ConfigurationFolder()
{
    freeChildren();
}

void ConfigurationFolder::addChild(ConfigurationNode* Node)
{

    m_childNodes.push_back(Node);

}

void ConfigurationFolder::freeChildren()
{
    unsigned int len = m_childNodes.size();

    for (unsigned int i = 0; i < len; i++)
    {
        delete m_childNodes[i];
    }

    m_childNodes.clear();

    return;
}
