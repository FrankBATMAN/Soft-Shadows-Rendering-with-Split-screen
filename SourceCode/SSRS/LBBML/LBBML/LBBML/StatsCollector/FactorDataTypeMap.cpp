#include "FactorDataTypeMap.h"
#include <iostream>

CDataTypeMap::CDataTypeMap(void)
{
	__initialDataTypeMap();
}

CDataTypeMap::~CDataTypeMap(void)
{

}

//******************************************************************
//FUNCTION:
const std::string CDataTypeMap::getDataTypeName(const std::string& vTypeIDName) const
{
	auto Itr = m_TypeNameMap.find(vTypeIDName);

	return Itr == m_TypeNameMap.end() ? NULL : Itr->second;
}

//******************************************************************
//FUNCTION:
void CDataTypeMap::__initialDataTypeMap()
{
	__insertNewDataTypeMap(typeid(bool).name(), FACTORDATATYPE_BOOL);
	__insertNewDataTypeMap(typeid(char).name(), FACTORDATATYPE_CHAR);
	__insertNewDataTypeMap(typeid(int).name(), FACTORDATATYPE_INT);
	__insertNewDataTypeMap(typeid(unsigned int).name(), FACTORDATATYPE_UNSIGNED_INT);
	__insertNewDataTypeMap(typeid(float).name(), FACTORDATATYPE_FLOAT);
	__insertNewDataTypeMap(typeid(double).name(), FACTORDATATYPE_DOUBLE);
	__insertNewDataTypeMap(typeid(std::string).name(), FACTORDATATYPE_STRING);
	__insertNewDataTypeMap(typeid(glm::vec2).name(), FACTORDATATYPE_VECTOR2);
	__insertNewDataTypeMap(typeid(glm::vec3).name(), FACTORDATATYPE_VECTOR3);
	__insertNewDataTypeMap(typeid(glm::vec4).name(), FACTORDATATYPE_VECTOR4);
}

//******************************************************************
//FUNCTION:
void CDataTypeMap::__insertNewDataTypeMap(const std::string& vTypeIDName, const std::string& vDataTypeName)
{
	m_TypeNameMap.insert(std::map<std::string, std::string>::value_type(vTypeIDName, vDataTypeName));
}