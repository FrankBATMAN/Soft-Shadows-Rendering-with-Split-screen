#pragma once

#include <string>
#include <vector>
#include <string>
#include <boost/any.hpp>
#include "../GraphicsCommon/LBBML_GRAPHICS_INTERFACE_EXPORTS.h"

class LBBML_GRAPHICS_DLL_EXPORTS CLBBMLGraphicsStats
{
public:
	CLBBMLGraphicsStats();
	virtual ~CLBBMLGraphicsStats();

	void addStat(const std::vector<boost::any>& vStat);
	void setStatsFilePath(const std::string& vFilePath) {m_StatsFilename = vFilePath;}
	void doWrite() {__writeStats2File();}

	const std::vector<std::vector<boost::any>>& getCollectedStats() const {return m_Stats;}

private:
	void __writeStats2File();

	std::string m_StatsFilename;
	std::vector<std::vector<boost::any>> m_Stats;
};