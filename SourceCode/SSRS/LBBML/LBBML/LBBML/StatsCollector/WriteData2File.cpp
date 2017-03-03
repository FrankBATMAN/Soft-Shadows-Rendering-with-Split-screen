#include "WriteData2File.h"
#include <string>
#include <iostream>

CWriteData2File::CWriteData2File(void)
{
	__initialise();
}

CWriteData2File::~CWriteData2File(void)
{
	delete m_pTypeNameMap;
}

//******************************************************************
//FUNCTION:
bool CWriteData2File::openCSVFile2Write(const std::string& vCSVFileName)
{
	_ASSERT(!vCSVFileName.empty(), "filename empty!");

	m_Filename = vCSVFileName;

	m_CSVFile = std::ofstream(m_Filename, std::ios::app);
	return m_CSVFile.is_open();
}

//******************************************************************
//FUNCTION:
void CWriteData2File::setCSVColumnNumber(const int vColumnNumber)
{
	_ASSERT(vColumnNumber >= 1, "column must be integer bigger than one!");
	__setColumnNumber(vColumnNumber);
}

//******************************************************************
//FUNCTION:
void CWriteData2File::setCSVFileColumnNames(const std::vector<std::string>& vColumnNames)
{
	_ASSERT(!vColumnNames.empty(), "column names cannot be empty!");

	int ColumnSize = vColumnNames.size();
	if (m_Column > 0)
		_ASSERT(m_Column == ColumnSize, "column names does not match column size!");
	else
		__setColumnNumber(ColumnSize);

	for (auto it = vColumnNames.begin(); it != vColumnNames.end(); ++it)
	{
		if (it != vColumnNames.end() - 1)
			m_CSVFile << (it)->c_str() << ',';
		else
			m_CSVFile << (it)->c_str() << std::endl;
	}
}

//******************************************************************
//FUNCTION:
void CWriteData2File::writeData2CSVFile(const boost::any& vAny)
{
	_ASSERT(m_Column > 0, "Column not set!");

	CFactor vFactor(vAny);
	std::string TypeName = m_pTypeNameMap->getDataTypeName(vFactor.getDataType());
	__writeFactorByTypeName(TypeName, vFactor);
	if (m_ColumnIndex < m_Column)
	{
		m_CSVFile << ',';
		m_ColumnIndex++;
	}
	else
	{
		m_CSVFile << std::endl;
		m_ColumnIndex = 1;
	}
}

//******************************************************************
//FUNCTION:
void CWriteData2File::closeCSVFile()
{
	m_CSVFile.close();
}

//******************************************************************
//FUNCTION:
void CWriteData2File::__initialise()
{
	m_pTypeNameMap = new CDataTypeMap;
	m_ColumnIndex = 1;
}

//******************************************************************
//FUNCTION:
void CWriteData2File::__setColumnNumber(const int vColumnNumber)
{
	m_Column = vColumnNumber;
}

//******************************************************************
//FUNCTION:
void CWriteData2File::__writeFactorByTypeName(const std::string& vTypeName, const CFactor& vFactor)
{
	if (vTypeName == FACTORDATATYPE_BOOL) 
		m_CSVFile << vFactor.getValue<bool>();
	else if (vTypeName == FACTORDATATYPE_CHAR) 
		m_CSVFile << vFactor.getValue<char>();
	else if (vTypeName == FACTORDATATYPE_INT) 
		m_CSVFile << vFactor.getValue<int>();
	else if (vTypeName == FACTORDATATYPE_UNSIGNED_INT) 
		m_CSVFile << vFactor.getValue<unsigned int>();
	else if (vTypeName == FACTORDATATYPE_FLOAT) 
		m_CSVFile << vFactor.getValue<float>();
	else if (vTypeName == FACTORDATATYPE_DOUBLE) 
		m_CSVFile << vFactor.getValue<double>();
	else if (vTypeName == FACTORDATATYPE_STRING) 
		m_CSVFile << vFactor.getValue<std::string>().c_str();
	else if (vTypeName == FACTORDATATYPE_VECTOR2)
	{
		glm::vec2 V2 = vFactor.getValue<glm::vec2>();
		m_CSVFile << V2[0] << ',' << V2[1];
	}
	else if (vTypeName == FACTORDATATYPE_VECTOR3)
	{
		glm::vec3 V3 = vFactor.getValue<glm::vec3>();
		m_CSVFile << V3[0] << ',' << V3[1] << ',' << V3[2];
	}
	else if (vTypeName == FACTORDATATYPE_VECTOR4)
	{
		glm::vec4 V4 = vFactor.getValue<glm::vec4>();
		m_CSVFile << V4[0] << ',' << V4[1] << ',' << V4[2] << ',' << V4[3];
	}
	else
		_ASSERT(false, "data cannot be write directly!");
}