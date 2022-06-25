#pragma once
#include <vector>
#include <string>

#include "ContainerElementTable.h"

#include "LineAction.h"

#include "Variable.h"

using std::vector, std::string;

class MethodTable final : public ContainerElementTable {
private:
	string m_methodName = "";
	Variable* m_returnVariableType = nullptr;
	vector <Variable*> m_parametrs;
	vector <LineAction*> m_lineActions;

public:
	MethodTable(const string& methodName) noexcept : m_methodName(methodName) {};

	MethodTable(const string& methodName,
		Variable* returnVariableType,
		vector<Variable*> parametrs) noexcept
		: m_methodName(methodName),
		m_returnVariableType(returnVariableType),
		m_parametrs(parametrs) {};

	string GetMethodName() const noexcept {
		return m_methodName;
	}

	void SetReturnVariableType(Variable*& returnVarType) noexcept {
		m_returnVariableType = returnVarType;
	}

	Variable*& GetReturnVariableType() noexcept {
		return m_returnVariableType;
	}

	void AddParametr(Variable*& parametrType) {
		m_parametrs.push_back(parametrType);
	}

	vector<Variable*>& GetParametrs() noexcept {
		return m_parametrs;
	}

	size_t GetParametrsCount() const noexcept {
		return m_parametrs.size();
	}

	bool IsParametrsEqualArguments(const vector<Variable*>& arguments) {
		if (m_parametrs.size() != arguments.size()) {
			return false;
		}

		for (size_t index = 0; index < m_parametrs.size(); index++) {
			VariableType paramType = m_parametrs[index]->GetVariableType();

			if (paramType != VariableType::AnyType) {
				if (!m_parametrs[index]->IsEqualVarStructures(arguments[index])) {
					return false;
				}
			}
		}

		return true;
	}

	vector<LineAction*>& GetLineActionVector() noexcept {
		return m_lineActions;
	}

	vector<LineAction*>* GetLineActionsLink() noexcept {
		return &m_lineActions;
	}

	void AddLineAction(LineAction*& lineAction) {
		m_lineActions.push_back(lineAction);
	}

	void RemoveLastLineActions(int countToRemove) {
		for (size_t index = m_lineActions.size() - 1;
			index >= m_lineActions.size() - countToRemove - 1;
			index--) {
			delete m_lineActions[index];
			m_lineActions.pop_back();
		}
	}

	bool operator==(const string methodName) noexcept {
		return m_methodName == methodName;
	}

	~MethodTable() {
		delete m_returnVariableType;

		for (Variable* parametr : m_parametrs) {
			delete parametr;
		}

		for (LineAction* lineAct : m_lineActions) {
			delete lineAct;
		}
	}
};
