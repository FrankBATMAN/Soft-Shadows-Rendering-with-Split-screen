#pragma once
#include <string>
#include "Config.h"

class CConfigParser
{
public:
	CConfigParser(void);
	virtual ~CConfigParser(void);

	bool parseConfig(const std::string& vConfigFilePath, CConfig* voConfig);

private:
	void __loadConfig(const std::string& vConfigFilePath);
	void __initConfigParser();
	void __finalizeConfigParser();

	CConfig* m_pConfig;
};