#pragma once
#include "LogicalAction.h"

class LogicalTwoArgs final : public LogicalAction {
private:
	Variable* m_firstLogicVar = nullptr;
	Variable* m_secondLogicVar = nullptr;

public:
	LogicalTwoArgs(VariableType logTwoArgsType) noexcept
		: LogicalAction(logTwoArgsType) {};

	LogicalTwoArgs(VariableType logTwoArgsType,
		Variable* firstVar, Variable* secondVar)
		: LogicalTwoArgs(logTwoArgsType) {
		m_firstLogicVar = firstVar;
		m_secondLogicVar = secondVar;
	}

	void SetFirstLocalVar(Variable* firstLogVar) noexcept {
		m_firstLogicVar = firstLogVar;
	}

	Variable*& GetFirstLogicalVar() noexcept {
		return m_firstLogicVar;
	}

	void SetSecondLogicalVar(Variable* secondLogVar) noexcept {
		m_secondLogicVar = secondLogVar;
	}

	Variable*& GetSecondLogicalVar() noexcept {
		return m_secondLogicVar;
	}

	Variable* Clone() const override {
		return new LogicalTwoArgs(m_variableType, m_firstLogicVar->Clone(), m_secondLogicVar->Clone());
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		if (stVar->GetVariableType() != m_variableType) {
			return false;
		}

		LogicalTwoArgs* logTwoArg = static_cast<LogicalTwoArgs*>(stVar);

		return m_firstLogicVar->IsEqualVarStructures(logTwoArg->GetFirstLogicalVar())
			&& m_secondLogicVar->IsEqualVarStructures(logTwoArg->GetSecondLogicalVar());
	}

	~LogicalTwoArgs() {
		delete m_firstLogicVar;
		delete m_secondLogicVar;
	}
};