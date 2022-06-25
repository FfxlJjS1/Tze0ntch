#pragma once
#include <string>

#include "LinkToVariable.h"

using std::string;

class ContainerVariable final : public LinkToVariable {
private:
	string m_name = "";
	Variable* m_unsignedByte = nullptr;

public:
	ContainerVariable() : LinkToVariable(VariableType::NamedVariableType) {}

	ContainerVariable(string name, Variable* variable) : ContainerVariable() {
		m_name = name;
		m_unsignedByte = variable;
	}

	void SetName(string name) {
		m_name = name;
	}

	string GetName() {
		return m_name;
	}

	void SetVariable(Variable* variable) {
		m_unsignedByte = variable;
	}

	Variable* GetVariable() {
		return m_unsignedByte;
	}

	Variable* Clone() const override {
		return new ContainerVariable(m_name, m_unsignedByte->Clone());
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		if (stVar->GetVariableType() != VariableType::NamedVariableType) {
			return false;
		}

		ContainerVariable* contVar = static_cast<ContainerVariable*>(stVar);

		if (m_name != contVar->GetName()
			|| !m_unsignedByte->IsEqualVarStructures(contVar->GetVariable())) {
			return false;
		}

		return true;
	}

	bool operator==(string name) {
		return m_name == name;
	}

	~ContainerVariable() {
		delete m_unsignedByte;
	}
};
