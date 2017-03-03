#include "LBBMLStatsCollector.h"

void LBBMLStatsCollector::writeData(const std::string& vFilename, const std::vector<boost::any>& vDataVec)
{
	_ASSERT(!vFilename.empty() && vDataVec.size() > 0, "invalid writing request");

	CWriteData2File* pCSV = new CWriteData2File;
	if (pCSV->openCSVFile2Write(vFilename))
	{
		pCSV->setCSVColumnNumber(vDataVec.size());
		for (unsigned int i = 0; i < vDataVec.size(); ++i)
			pCSV->writeData2CSVFile(vDataVec[i]);

		pCSV->closeCSVFile();
	}
	delete(pCSV);
}