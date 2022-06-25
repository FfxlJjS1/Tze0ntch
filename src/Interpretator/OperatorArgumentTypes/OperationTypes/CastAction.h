#pragma once
#include "Variable.h"

class CastAction : public Variable {
private:
	Variable* m_fromCast = nullptr;
	Variable* m_toCast = nullptr;

public:
	CastAction(Variable* fromCast, Variable* toCast)
		: Variable(VariableType::CastActionType) {
		m_fromCast = fromCast;
		m_toCast = toCast;
	}

	Variable* GetFromCast() {
		return m_fromCast;
	}

	Variable* GetToCast() {
		return m_toCast;
	}

	CastAction* Clone() const override {
		return new CastAction(m_fromCast->Clone(), m_toCast->Clone());
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		if (stVar->GetVariableType() != m_variableType) {
			return false;
		}

		CastAction* castAct = static_cast<CastAction*>(stVar);

		return m_fromCast->IsEqualVarStructures(castAct->GetFromCast())
			&& m_toCast->IsEqualVarStructures(castAct->GetToCast());
	}

	~CastAction() {
		delete m_fromCast;
		delete m_toCast;
	}
};
