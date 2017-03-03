#pragma once
#include <map>
#include <string>
#include <typeinfo>
#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>
#include "DataTypeName.h"

class CDataTypeMap
{
public:
	CDataTypeMap(void);
	virtual ~CDataTypeMap(void);

	template<class T>
	void addTypeNamePair(const std::string& vDataTypeName)
	{
		std::string typeName = typeid(T).name();

		std::map<std::string, std::string>::const_iterator itr = m_TypeNameMap.find(typeName);
		if (itr == m_TypeNameMap.end())
			__insertNewDataTypeMap(typeName, vDataTypeName);
	}

	const std::string getDataTypeName(const std::string& vTypeIDName) const;

private:
	void __initialDataTypeMap();
	void __insertNewDataTypeMap(const std::string& vTypeIDName, const std::string& vDataTypeName);

	std::map<std::string, std::string> m_TypeNameMap;
};