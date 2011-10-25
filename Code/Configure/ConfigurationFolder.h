#ifndef _CONFIGURATION_FOLDER_DEF_H_
#define _CONFIGURATION_FOLDER_DEF_H_
#include "ConfigurationNode.h"
#include <vector>

/**
 * @brief A implementation of a configuration folder, it has a number of children pointers to them and a name
 */

class ConfigurationFolder : public ConfigurationNode
{

private:

    std::vector <ConfigurationNode*> m_childNodes;

    void freeChildren();

public:

    ConfigurationFolder(const char* Name);
    ~ConfigurationFolder();

    /**
     * @brief Return that the node is a configuration node
     */

    int getType() { return configurationFolder; }

    /**
     * @brief Add the specified child to this node
     */

    void addChild(ConfigurationNode* node);

    /**
     * @brief Return the specified child of the node
     */

    ConfigurationNode* getChild(unsigned int iter) { return m_childNodes.at(iter); }

    /**
     * @brief Return the number of children in this node
     */
    size_t numberOfChildren() { return m_childNodes.size(); }

};

#endif //_CONFIGURATION_FOLDER_DEF_H_
