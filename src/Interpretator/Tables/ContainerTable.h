#pragma once
#include <string>

#include "MethodTable.h"

#include "NamedVariable.h"

using std::string;

enum class ContainerTableType : char {
	Null,
	ModuleTableType,
	StructTableType
};

class ContainerTable {
protected:
	string m_containerName = "";
	ContainerTableType m_cTableType = ContainerTableType::Null;
	vector<ContainerTable*> m_subContainerTables;
	vector<MethodTable*> m_methodTables;
	vector<ContainerVariable*> m_globalVariables;

public:
	ContainerTable(const string& containerName, const ContainerTableType cTableType) : m_containerName(containerName), m_cTableType(cTableType) {};

	string GetContainerName() const {
		return m_containerName;
	}

	ContainerTableType GetContainerTableType() const noexcept {
		return m_cTableType;
	}

	/*
	  Return link to NamedVariable if contains it with name as arg
	  If not found return nullptr
	*/
	ContainerVariable* GetGlobalVariable(const string& glVarName) {
		for (ContainerVariable* glVar : m_globalVariables) {
			if (*glVar == glVarName) {
				return glVar;
			}
		}

		return nullptr;
	}

	ContainerVariable* GetGlobalVariable(const size_t index) {
		return m_globalVariables.at(index);
	}

	/*
	  Return index of global variable with name
	  If not found return index greater than size of global variables vector
	*/
	size_t GetGlobalVariableIndex(const string& glVarName) const {
		for (size_t index = 0; index < m_globalVariables.size(); index++) {
			if (m_globalVariables[index]->GetName() == glVarName) {
				return index;
			}
		}

		return m_globalVariables.size() + 1;
	}

	void AddGlobalVariable(ContainerVariable* glVar) {
		m_globalVariables.push_back(glVar);
	}

	size_t GetGlobalVariablesCount() const {
		return m_globalVariables.size();
	}

	/*
	  Return lint to SubContainerTable if contains it with name as arg
	  If not found return nullptr
	*/
	ContainerTable* GetSubContainerTable(const string& structName) {
		for (ContainerTable* structTable : m_subContainerTables) {
			if (*structTable == structName) {
				return structTable;
			}
		}

		return nullptr;
	}

	void AddStructTable(ContainerTable* structTable) {
		m_subContainerTables.push_back(structTable);
	}

	size_t GetStructTablesCount() const {
		return m_subContainerTables.size();
	}

	/*
	  Return lint to MethodTable with name
	  If not found return nullptr
	*/
	MethodTable* GetMethodTable(const string& methodName) {
		for (MethodTable* methodTable : m_methodTables) {
			if (*methodTable == methodName) {
				return methodTable;
			}
		}

		return nullptr;
	}

	/*
	  Return link to MethodTable with name and parametrs equal arguments
	  If not found return nullptr
	*/
	MethodTable* GetMethodTable(const string& methodName, const vector<Variable*>& arguments) {
		for (MethodTable* conTabMethodTable : m_methodTables) {
			if (*conTabMethodTable == methodName &&
				conTabMethodTable->IsParametrsEqualArguments(arguments)) {
				return conTabMethodTable;
			}
		}

		return nullptr;
	}

	size_t GetMethodTablesCount() const {
		return m_methodTables.size();
	}

	void AddMethodTable(MethodTable* methodTable) {
		m_methodTables.push_back(methodTable);
	}

	bool operator==(const string& containerName) const noexcept {
		return m_containerName == containerName;
	}

	~ContainerTable() {
		for (ContainerVariable* glVar : m_globalVariables) {
			delete glVar;
		}

		for (ContainerTable* structTable : m_subContainerTables) {
			delete structTable;
		}

		for (MethodTable* methodTable : m_methodTables) {
			delete methodTable;
		}
	}
};
