#include "CosFunction.h"

CCosFunction::CCosFunction(void)
{
}

CCosFunction::~CCosFunction(void)
{
}

//******************************************************************
//FUNCTION:
float CCosFunction::calculateFunctionValueV(float vX)
{
	return cos(vX);
}

//******************************************************************
//FUNCTION:
void CCosFunction::printMyFormulaV()
{
	std::cout << "cos(x)";
}