#pragma once
#include <string>

#include "Variable.h"

using std::string;

class String_t final : public Variable {
private:
	string m_string = "";

public:
	String_t() : Variable(VariableType::StringType) {};

	String_t(const string& variable) : String_t() {
		m_string = variable;
	}

	string GetString() {
		return m_string;
	}

	void SetString(const string& variable) {
		m_string = variable;
	}

	Variable* Clone() const {
		return new String_t(m_string);
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};
