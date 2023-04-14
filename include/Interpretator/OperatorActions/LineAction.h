#pragma once

enum class LineActionType : char {
	Null,

	// Actions
	MathActionType,
	LogicalActionType,

	/// LineActionTypes
	DefineSpecificVariableType,
	AssertActionType,
	RemoveSpecificVariableType,
	SkipRemoveMethLocalVariableType,

	GotoActionType,
	GotoActionWithIfType,

	GoToMethodEmptyType,

	ReturnFromMethodType,
};

class LineAction {
protected:
	LineActionType m_lineActType = LineActionType::Null;

public:
	LineAction(LineActionType lineActType) noexcept : m_lineActType(lineActType) {};

	LineActionType GetLineActionType() noexcept {
		return m_lineActType;
	}
};
