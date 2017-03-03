#include "LBBMLFunctions.h"

CLBBMLFunctions::CLBBMLFunctions(void)
{
	__initialise();
}

CLBBMLFunctions::~CLBBMLFunctions(void)
{
	for (unsigned int i = 0; i < m_FunctionDatabase.size(); ++i)
		delete(m_FunctionDatabase.at(i));

	m_FunctionDatabase.clear();
}

//*********************************************
//Function:
CFunctionBase* CLBBMLFunctions::getFunctionByIndex(int vIndex)
{
	_ASSERT(vIndex >= 0, "invalid index!");

	vIndex = vIndex % m_FunctionDatabase.size();
	return m_FunctionDatabase.at(vIndex);
}

//*********************************************
//Function:
void CLBBMLFunctions::__initialise()
{
	CSigmoidFunction *pSigmoid = new CSigmoidFunction;
 	CLinearFunction *pLinear = new CLinearFunction;
 	CCosFunction *pCos = new CCosFunction;
 	CSinFunction *pSin = new CSinFunction;
	CPowFunction *pPow = new CPowFunction;
	CLogFunction *pLog = new CLogFunction;
	
	m_FunctionDatabase.push_back(pSigmoid);
 	m_FunctionDatabase.push_back(pLinear);
 	m_FunctionDatabase.push_back(pCos);
 	m_FunctionDatabase.push_back(pSin);
	m_FunctionDatabase.push_back(pPow);
	m_FunctionDatabase.push_back(pLog);
}