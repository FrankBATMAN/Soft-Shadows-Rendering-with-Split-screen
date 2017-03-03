#include "LogFunction.h"

CLogFunction::CLogFunction(void)
{
}

CLogFunction::~CLogFunction(void)
{
}

//******************************************************************
//FUNCTION:
float CLogFunction::calculateFunctionValueV(float vX)
{
	_ASSERT(vX > 0, "X must be greater than zero!");

	return log(vX);
}

//******************************************************************
//FUNCTION:
void CLogFunction::printMyFormulaV()
{
	std::cout << "log(x)";
}