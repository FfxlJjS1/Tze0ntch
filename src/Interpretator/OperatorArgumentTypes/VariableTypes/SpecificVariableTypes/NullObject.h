#pragma once
#include "Variable.h"

class NullObject : public Variable {
public:
	NullObject() : Variable(VariableType::NullObjectType) {};

	bool IsEqualVarStructures(Variable* stVar) const {
		return stVar->GetVariableType() == VariableType::NullObjectType;
	}

	Variable* Clone() const override {
		return new NullObject();
	}
};
