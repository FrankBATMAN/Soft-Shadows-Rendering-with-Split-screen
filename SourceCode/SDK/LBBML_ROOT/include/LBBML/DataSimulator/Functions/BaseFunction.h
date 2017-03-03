#pragma once
#include <iostream>
#include <math.h>

class CFunctionBase
{
public:
	CFunctionBase(void){};
	virtual ~CFunctionBase(void){};

	virtual float calculateFunctionValueV(float vX) = 0;
	virtual void  printMyFormulaV() = 0;
};