#include "ConfigurationNode.h"

ConfigurationNode::ConfigurationNode(const char* Name)
{
    m_nodeName = new char[ strlen(Name) + 1];
    strcpy(m_nodeName, Name);

}

ConfigurationNode::~ConfigurationNode()
{
    delete[] m_nodeName;
}
