#pragma once
#include <string>
#include <boost\any.hpp>

class CFactor
{
public:
	CFactor(void);
	virtual ~CFactor(void);

	CFactor(const boost::any& vFactorValue);
	CFactor(const std::string& vFactorName, const boost::any& vFactorValue);
	
	void setName(const std::string& vFactorName);
	void setValue(const boost::any& vFactorValue);
	
	const std::string& getName() const;
	const std::string& getDataType() const;
	
	template<class T>
	const T getValue() const
	{
			_ASSERT(!m_Value.empty(), "object is empty !");
			try
			{
				return boost::any_cast<T>(m_Value);
			}
			catch(...)
			{
				_ASSERT(false, "data type not match !");	
			}
	}

private:
	void __initialValue(const boost::any& vFactorValue);
	void __initialName(const std::string& vFactorName);
	void __setDataType();

	std::string		m_Name;
	std::string		m_DataType;
	boost::any		m_Value;
};