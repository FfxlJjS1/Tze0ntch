#pragma once
#include <string>

#include "LinkToVariable.h"

#include "Struct.h"

using std::string;

// For unoin struct name and array name
class NamedObject : public LinkToVariable {
protected:
	string m_variableName = "";

	NamedObject(VariableType moduleVariableType,
		string moduleVariableName)
		: LinkToVariable(moduleVariableType) {
		m_variableName = moduleVariableName;
	};

public:
	NamedObject(string moduleVariableName)
		: LinkToVariable(VariableType::NamedObjectType) {
		m_variableName = moduleVariableName;
	}

	string GetVariableName() const {
		return m_variableName;
	}

	void SetVariableName(const string& name) {
		m_variableName = name;
	}

	Variable* Clone() const {
		return new NamedObject(m_variableName);
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		switch (stVar->GetVariableType()) {
		case VariableType::StructType:
		{
			Struct* stStruct = static_cast<Struct*>(stVar);

			if (stStruct->GetStructName() != "" && stStruct->GetStructName() != m_variableName) {
				return false;
			}

			return true;
		}
			break;
		case VariableType::NamedObjectType:
			return static_cast<NamedObject*>(stVar)->GetVariableName() == m_variableName;
			break;
		default:
			return false;
			break;
		}
	}
};
