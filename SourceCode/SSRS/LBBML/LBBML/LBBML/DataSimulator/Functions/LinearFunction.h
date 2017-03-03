#pragma once
#include "BaseFunction.h"

class CLinearFunction : public CFunctionBase
{
public:
	CLinearFunction(void);
	virtual ~CLinearFunction(void);

	float calculateFunctionValueV(float vX) override;
	void  printMyFormulaV() override;
};