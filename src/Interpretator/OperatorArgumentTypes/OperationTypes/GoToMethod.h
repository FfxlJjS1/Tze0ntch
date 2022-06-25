#pragma once
#include <string>

#include "LinkToVariable.h"

using std::string;

class GoToMethod final : public LinkToVariable {
private:
	string m_methodName = "";
	vector<Variable*> m_arguments;

public:
	GoToMethod(string methodName)
		: LinkToVariable(VariableType::GoToMethodType) {
		m_methodName = methodName;
	};

	string GetMethodName() const {
		return m_methodName;
	}

	vector<Variable*>& GetArguments() noexcept {
		return m_arguments;
	}

	void AddArgument(Variable* argument) {
		m_arguments.push_back(argument);
	}

	Variable*& GetArgAt(size_t index) {
		return m_arguments.at(index);
	}

	void SetArguments(vector<Variable*>& arguments) noexcept {
		m_arguments = arguments;
	}

	Variable* Clone() const override {
		GoToMethod* clone = new GoToMethod(m_methodName);

		for (Variable* var : m_arguments) {
			clone->AddArgument(var->Clone());
		}

		return clone;
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}

	~GoToMethod() {
		for (size_t index = 0; index < m_arguments.size(); index++) {
			delete m_arguments.at(index);
		}
	}
};
