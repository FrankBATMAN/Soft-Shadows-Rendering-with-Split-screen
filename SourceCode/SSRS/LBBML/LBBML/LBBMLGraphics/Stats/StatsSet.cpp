#include "StatsSet.h"
#include <iostream>
#include "../../LBBML/Common/LBBMLInterface.h"

CLBBMLGraphicsStats::CLBBMLGraphicsStats()
{

}

CLBBMLGraphicsStats::~CLBBMLGraphicsStats()
{
	__writeStats2File();
	m_Stats.clear();
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsStats::addStat(const std::vector<boost::any>& vStat)
{
	_ASSERT(vStat.size() > 0, "Stat can not be Empty!");

	if (m_Stats.size() == 0)
		m_Stats.push_back(vStat);
	else if (m_Stats.size() > 0 && vStat.size() == m_Stats[0].size())
		m_Stats.push_back(vStat);
	else
	{
		std::cout << "Warning : Size does not match !" << std::endl;
		m_Stats.push_back(vStat);
	}
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsStats::__writeStats2File()
{
	if (m_StatsFilename.empty()) m_StatsFilename = std::string("StatsData.csv");
	if (m_Stats.size() > 0)
	{
		for (unsigned int i = 0; i < m_Stats.size(); ++i)
			LBBML::writeData(m_StatsFilename, m_Stats[i]);
	}
}