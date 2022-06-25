#pragma once
#include "LineAction.h"
#include "Variable.h"

class RemoveSpecificVariable final : public LineAction {
private:
	VariableType m_removeThe = VariableType::Null; // Contains only specific types
	size_t m_countToRemove = 0;

public:
	RemoveSpecificVariable(VariableType removeThe, size_t countToRemove) : LineAction(LineActionType::RemoveSpecificVariableType) {
		m_removeThe = removeThe;
		m_countToRemove = countToRemove;
	}

	VariableType GetSpecificTypeToRemove() {
		return m_removeThe;
	}

	size_t GetCountToRemove() {
		return m_countToRemove;
	}
};
