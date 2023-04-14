#pragma once
#include "Variable.h"

class Struct final : public Variable {
private:
	string m_structName = "";
	vector<Variable*> m_vars;

public:
	Struct(const string& structName) : Variable(VariableType::StructType) {
		m_structName = structName;
	};

	Struct(const string& structName, vector<Variable*>& vars)
		: Struct(structName) {
		m_vars = vars;
	}

	string GetStructName() const {
		return m_structName;
	}

	void SetStructName(string structName) {
		m_structName = structName;
	}

	void AddVarToBack(Variable* var) {
		m_vars.push_back(var);
	}

	vector<Variable*>& GetVariables() {
		return m_vars;
	}

	size_t GetVariableCount() const {
		return m_vars.size();
	}

	Variable*& GetAt(const size_t index) {
		return m_vars.at(index);
	}

	Variable* GetCopyAt(const size_t index) const {
		return m_vars.at(index)->Clone();
	}

	void SetStructFrom(Struct* fromStruct) {
		m_structName = fromStruct->GetStructName();

		for (Variable* var : m_vars) {
			delete var;
		}

		m_vars.clear();

		for (size_t index = 0; index < fromStruct->GetVariableCount(); index++) {
			m_vars.push_back(fromStruct->GetAt(index));
		}
	}

	void SetStructCopyFrom(const Struct* fromStruct) {
		m_structName = fromStruct->GetStructName();

		for (Variable* var : m_vars) {
			delete var;
		}

		m_vars.clear();

		for (size_t index = 0; index < fromStruct->GetVariableCount(); index++) {
			m_vars.push_back(fromStruct->GetCopyAt(index));
		}
	}

	Variable* Clone() const {
		Struct* clone = new Struct(m_structName);

		for (Variable* var : m_vars) {
			clone->AddVarToBack(static_cast<Variable*>(var->Clone()));
		}

		return clone;
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		if (stVar->GetVariableType() != VariableType::StructType) {
			return false;
		}

		Struct* varAr = static_cast<Struct*>(stVar);

		if (m_structName == "") {
			if (varAr->GetVariableCount() != m_vars.size()) {
				return false;
			}

			for (size_t index = 0; index < m_vars.size(); index++) {
				if (!m_vars[index]->IsEqualVarStructures(varAr->GetAt(index))) {
					return false;
				}
			}
		}
		else if (varAr->GetStructName() != m_structName) {
			return false;
		}
		/* For entity equal between structs. Not using because it not helped method with argument and parametr where they is stuct but argument with name and arguments and parametr with only name of structur without arguments
		if (varAr->GetVariableCount() != m_vars.size()) {
			return false;
		}

		for (size_t index = 0; index < m_vars.size(); index++) {
			if (!m_vars[index]->IsEqualVarStructures(varAr->GetAt(index))) {
				return false;
			}
		}
		*/

		return true;
	}

	~Struct() {
		if (m_structName != "") {
			for (Variable* var : m_vars) {
				delete var;
			}
		}
	}
};
