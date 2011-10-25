#include "ConfigurationNumber.h"

ConfigurationNumber::ConfigurationNumber(char* name, float val) : ConfigurationNode(name)
{

    m_numberValue = val;

}

ConfigurationNumber::~ConfigurationNumber()
{

}
