#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <boost/any.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include "../../Common/LBBML_INTERFACE_EXPORTS.h"

enum EAttributeType
{
	ATTRIBUTE_INT = 0,
	ATTRIBUTE_FLOAT,
	ATTRIBUTE_STRING,
};

class LBBML_DLL_EXPORTS CConfig
{
public:
	CConfig(void);
	virtual ~CConfig(void);
	
	CConfig* fetchConfigByName(const std::string& vConfigName);

	void addSubConfig(CConfig* vConfig);
	void addAttribute(const std::string& vAttributeName, const EAttributeType& vAttributeType);

	void setAttribute(const std::string& vAttributeName, boost::any vAttributeValue);
	void setName(const std::string& vName);

	const std::string& getName() const;

	bool isConfigValid();

	template<class T>
	T getAttribute(const std::string& vAttributeName)
	{
		std::string AttributeName = boost::algorithm::to_upper_copy(vAttributeName);

		CConfig* pConfig  = this;
		pConfig = pConfig->fetchConfigByName(AttributeName);

		if (pConfig)
		{
			T AttributeValue = boost::any_cast<T>(pConfig->m_Attribute);

			return AttributeValue;
		}
		else
		{
			std::cout << "There is no such an attribute." << std::endl;
		}	
	}

private:
	void __extractAttributes(boost::property_tree::ptree vPTree);
	void __initConfig();
	void __finalizeConfig();

	std::map<std::string, EAttributeType>	m_AttributeTypeMap;
	std::vector<CConfig*>					m_SubConfigSet;
	std::string								m_Name;
	boost::any                              m_Attribute;

	bool m_IsLeafConfig;

	friend class CConfigParser;
};