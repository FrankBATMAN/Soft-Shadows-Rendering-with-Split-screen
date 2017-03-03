#pragma once
#include "BaseFunction.h"

class CLogFunction : public CFunctionBase
{
public:
	CLogFunction(void);
	virtual ~CLogFunction(void);

	float calculateFunctionValueV(float vX) override;
	void  printMyFormulaV() override;
};

