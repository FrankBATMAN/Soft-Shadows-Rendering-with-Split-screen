#include "PowFunction.h"
#include "../../MathWorker/LBBMLMathWorker.h"

CPowFunction::CPowFunction(void)
{
}

CPowFunction::~CPowFunction(void)
{
}

//******************************************************************
//FUNCTION:
float CPowFunction::calculateFunctionValueV(float vX)
{
	m_PowerNumber = LBBMLMathWorker::generateRandomf(-3.0, 3.0);

	return pow(vX, m_PowerNumber);
}

//******************************************************************
//FUNCTION:
void CPowFunction::printMyFormulaV()
{
	std::cout << "x^" << m_PowerNumber;
}