#pragma once
#include "BaseFunction.h"

class CCosFunction : public CFunctionBase
{
public:
	CCosFunction(void);
	virtual ~CCosFunction(void);

	float calculateFunctionValueV(float vX) override;
	void  printMyFormulaV() override;
};
