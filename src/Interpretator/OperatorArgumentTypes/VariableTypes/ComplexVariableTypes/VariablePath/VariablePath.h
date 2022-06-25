#pragma once
#include <vector>

#include "LinkToVariable.h"

using std::vector;

class VariablePath final : public LinkToVariable {
protected:
	// Can be varLink and ArrayIndex (array), string (subfield struct), GoToMethod (method)
	vector<Variable*> m_variabicalPath;

public:
	VariablePath() : LinkToVariable(VariableType::VariablePathType) {}

	void AddVariabicalPath(Variable* varPath) {
		m_variabicalPath.push_back(varPath);
	}

	Variable* GetVarPathAt(const size_t index) {
		return m_variabicalPath.at(index);
	}

	Variable* GetVarPathFront() {
		return m_variabicalPath.front();
	}

	Variable* GetVarPathBack() {
		return m_variabicalPath.back();
	}

	size_t GetVarPathSize() const {
		return m_variabicalPath.size();
	}

	VariablePath* Clone() const {
		VariablePath* clone = new VariablePath();

		for (Variable* var : m_variabicalPath) {
			clone->AddVariabicalPath(var->Clone());
		}

		return clone;
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}

	~VariablePath() {
		for (Variable* var : m_variabicalPath) {
			delete var;
		}
	}
};