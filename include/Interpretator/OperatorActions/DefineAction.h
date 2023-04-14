#pragma once
#include "LineAction.h"
#include "LinkToVariable.h"

class DefineAction final : public LineAction {
private:
	LinkToVariable* m_setTo = nullptr;
	Variable* m_unsignedByte = nullptr;

public:
	DefineAction() noexcept : LineAction(LineActionType::DefineSpecificVariableType) {};

	DefineAction(LinkToVariable* setTo, Variable* variable) noexcept 
		: DefineAction() {
		m_setTo = setTo;
		m_unsignedByte = variable;
	};

	void SetSetToType(LinkToVariable*& setTo) noexcept {
		m_setTo = setTo;
	}

	LinkToVariable*& GetSetToType() noexcept {
		return m_setTo;
	}

	void SetVariableLink(Variable*& variable) noexcept {
		m_unsignedByte = variable;
	}

	Variable*& GetVariableLink() noexcept {
		return m_unsignedByte;
	}

	~DefineAction() {
		delete m_setTo;
		delete m_unsignedByte;
	}
};
