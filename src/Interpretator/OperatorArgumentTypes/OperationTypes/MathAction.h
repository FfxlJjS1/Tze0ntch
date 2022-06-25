#pragma once
#include "Variable.h"

class MathAction final : public Variable {
private:
	Variable* m_var1 = nullptr;
	Variable* m_var2 = nullptr;

public:
	MathAction(VariableType mathOptionType) noexcept : Variable(mathOptionType) {};

	MathAction(VariableType mathOptionType, Variable* var1, Variable* var2) noexcept
		: Variable(mathOptionType) {
		m_var1 = var1;
		m_var2 = var2;
	}

	void SetVar1(Variable* var1) noexcept {
		m_var1 = var1;
	}

	Variable*& GetVar1() noexcept {
		return m_var1;
	}

	void SetVar2(Variable* var2) noexcept {
		m_var2 = var2;
	}

	Variable*& GetVar2() noexcept {
		return m_var2;
	}

	Variable* Clone() const override {
		return new MathAction(m_variableType, m_var1->Clone(), m_var2->Clone());
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		if (stVar->GetVariableType() != m_variableType) {
			return false;
		}

		MathAction* mAct = static_cast<MathAction*>(stVar);

		return m_var1->IsEqualVarStructures(mAct->GetVar1())
			&& m_var2->IsEqualVarStructures(mAct->GetVar2());
	}

	~MathAction() {
		delete m_var1;
		delete m_var2;
	}
};
