#pragma once
#include "Variable.h"

class LinkToVariable : public Variable {
public:
	LinkToVariable(VariableType varType) noexcept : Variable(varType) {};
};
