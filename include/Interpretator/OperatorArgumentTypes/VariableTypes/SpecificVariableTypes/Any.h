#pragma once
#include "Variable.h"

class Any : public Variable {
private:
	Variable* m_anyVar = nullptr;

public:
	Any() : Variable(VariableType::AnyType) {};

	Any(Variable* anyVar) : Any() {
		m_anyVar = anyVar;
	}

	void DeleteAnyVar() {
		delete m_anyVar;
	}

	Variable* GetAnyVar() const {
		return m_anyVar;
	}

	void SetAnyVar(Variable* anyVar) {
		m_anyVar = anyVar;
	}

	Variable* Clone() const {
		return new Any(m_anyVar->Clone());
	}

	bool IsEqualVarStructures(Variable* stVar) const {
		return true;
	}

	~Any(){
		delete m_anyVar;
	}
};
