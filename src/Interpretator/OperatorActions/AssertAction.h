#pragma once
#include "LineAction.h"
#include "LinkToVariable.h"

class AssertAction final : public LineAction {
private:
	LinkToVariable* m_lValue = nullptr;
	Variable* m_rValue = nullptr;

public:
	AssertAction() : LineAction(LineActionType::AssertActionType) {};

	AssertAction(LinkToVariable* lValue,
		Variable* rValue) : AssertAction(){
		m_lValue = lValue;
		m_rValue = rValue;
	}

	void SetLValue(LinkToVariable*& lValue) {
		m_lValue = lValue;
	}

	LinkToVariable*& GetLValue() {
		return m_lValue;
	}

	void SetRValue(Variable* rValue) {
		m_rValue = rValue;
	}

	Variable*& GetRightValue() {
		return m_rValue;
	}

	~AssertAction() {
		delete m_lValue;
		delete m_rValue;
	}
};
