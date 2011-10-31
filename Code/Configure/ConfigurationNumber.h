#ifndef _CONFIGURATION_NUMBER_DEF_H_
#define _CONFIGURATION_NUMBER_DEF_H_
#include "ConfigurationNode.h"

class ConfigurationNumber: public ConfigurationNode {
private:
	float m_numberValue;

public:

	ConfigurationNumber(char* Name, float value);
	~ConfigurationNumber();

	float getValue() {
		return m_numberValue;
	}

	void setValue(float val) {
		m_numberValue = val;
	}

	int getType() {
		return configurationNumber;
	}

};

#endif //_CONFIGURATION_NUMBER_DEF_H_
