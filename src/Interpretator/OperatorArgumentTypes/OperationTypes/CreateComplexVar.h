#pragma once
#include "Variable.h"

class CreateComplexVar : public Variable {
private:
	Variable* m_varTypeForCreate = nullptr;

public:
	CreateComplexVar() : Variable(VariableType::CreateComplexVarType) {}

	CreateComplexVar(Variable* varForCreate) : CreateComplexVar() {
		m_varTypeForCreate = varForCreate;
	}

	Variable* GetVarTypeForCreate() {
		return m_varTypeForCreate;
	}

	void SetVarTypeForCreate(Variable* var) {
		m_varTypeForCreate = var;
	}
};