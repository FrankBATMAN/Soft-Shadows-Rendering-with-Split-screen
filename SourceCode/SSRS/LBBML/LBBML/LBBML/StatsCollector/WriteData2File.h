#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <boost/any.hpp>
#include "Factors.h"
#include "FactorDataTypeMap.h"

class CWriteData2File
{
public:
	CWriteData2File(void);
	virtual ~CWriteData2File(void);

	bool openCSVFile2Write(const std::string& vCSVFileName);
	void setCSVColumnNumber(const int vColumnNumber);
	void setCSVFileColumnNames(const std::vector<std::string>& vColumnNames);
	void writeData2CSVFile(const boost::any& vAny);
	void closeCSVFile();

private:
	void __initialise();
	void __setColumnNumber(const int vColumnNumber);
	void __writeFactorByTypeName(const std::string& vTypeName, const CFactor& vFactor);

	int				m_Column;
	int				m_ColumnIndex;
	std::string		m_Filename;
	std::ofstream	m_CSVFile;
	CDataTypeMap	*m_pTypeNameMap;
};