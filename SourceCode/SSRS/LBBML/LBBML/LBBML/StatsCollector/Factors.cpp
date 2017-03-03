#include "Factors.h"

CFactor::CFactor(void)
{
}

CFactor::~CFactor(void)
{
}

//******************************************************************
//FUNCTION:
CFactor::CFactor(const boost::any& vFactorValue)
{
	__initialValue(vFactorValue);
}

//******************************************************************
//FUNCTION:
CFactor::CFactor(const std::string& vFactorName, const boost::any& vFactorValue)
{
	__initialName(vFactorName);
	__initialValue(vFactorValue);
}

//******************************************************************
//FUNCTION:
void CFactor::setName(const std::string& vFactorName)
{
	__initialName(vFactorName);
}

//******************************************************************
//FUNCTION:
void CFactor::setValue(const boost::any& vFactorValue)
{
	__initialValue(vFactorValue);
}

//******************************************************************
//FUNCTION:
const std::string& CFactor::getName() const
{
	return m_Name;
}

//******************************************************************
//FUNCTION:
const std::string& CFactor::getDataType() const
{
	_ASSERT(!m_DataType.empty(), "data type is empty !");

	return m_DataType;
}

//******************************************************************
//FUNCTION:
void CFactor::__initialName(const std::string& vFactorName)
{
	_ASSERT(!vFactorName.empty(), "invalid factor name !");

	m_Name = vFactorName;
}

//******************************************************************
//FUNCTION:
void CFactor::__initialValue(const boost::any& vFactorValue)
{
	_ASSERT(!vFactorValue.empty(), "invalid factor value !");

	m_Value = vFactorValue;

	__setDataType();
}

//******************************************************************
//FUNCTION:
void CFactor::__setDataType()
{
	m_DataType = m_Value.type().name();
}