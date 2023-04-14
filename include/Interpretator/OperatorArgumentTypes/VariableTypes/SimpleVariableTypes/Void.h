#pragma once
#include "Variable.h"

class Void final : public Variable {
public:
	Void() : Variable(VariableType::VoidType) {};

	Variable* Clone() const {
		return new Void;
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};