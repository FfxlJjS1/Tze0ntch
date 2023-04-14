#pragma once
#include <stack>

#include "DbController.h"
#include "RecursionParser.h"
#include "AcceptedClient.h"

#include "InterpretorHeaders.h"
#include "ModuleTable.h"
#include "StructTable.h"

extern std::mutex globalMutex;

using std::shared_ptr;

using std::stoi, std::stoll, std::stoull, std::to_string, std::stod;

class Interpretator final {
private:
	vector<ModuleTable*> m_moduleTables;
	shared_ptr<DbController> m_dbController;
	shared_ptr<AcceptedClient> m_acceptedClient = nullptr;
	MethodTable* m_startModuleMethodTable = nullptr;
	std::stack<Variable*> m_stack;

public:
	Interpretator(vector<ModuleTable*>& moduleTables, shared_ptr<DbController> dbController, shared_ptr<AcceptedClient> acceptedClient, MethodTable* startModuleMethodTable)
		: m_moduleTables(moduleTables), m_dbController(dbController), m_acceptedClient(acceptedClient), m_startModuleMethodTable(startModuleMethodTable) {};

	void Run();

	void ClearInterpretator();

private:
	/* 
	  Keep module id contains active execute a method at the moment
	  For reserver methods
	*/
	vertexIdType currModuleId = 0;
	/*
	  Keep container that where algorithm is working
	  Maybe ModuleTable or StructTable
	*/
	ContainerTable* container = nullptr;

	ModuleTable* GetParentModuleTable(ContainerTable* containerTable);

	void DefineAllModuleContainerVariables();

	void DefineModuleContainerVariables(ModuleTable* moduleTable);

	/*
	  For calculate and set to stack arguments
	  But not run new method.
	  For run new method perform GoToMethod method
	*/
	void RunGoToMethod(MethodTable* moduleTable);


	// Get from m_moduleTables module contains moduleId as m_moduleId
	ModuleTable* GetModuleTableByItsId(const vertexIdType moduleId) const;


	// Find with recursion method by requires and additional usings from startModule
	MethodTable* FindMethodByAdditions(GoToMethod* goToMethod, ContainerTable* containerFromSearch);

	// Find method only in contanier table
	MethodTable* FindMethodInContainerTable(GoToMethod* goToMethod, ContainerTable* containerForSearch);

	// Find global variable in modules. In struct getDefinedVariable is find
	ContainerVariable* FindNamedVariableByAdditions(const string& globVarName, ContainerTable* containerFromSearch);

	// Find structures in structure container and down in level
	StructTable* FindStructTableByAdditions(const string& structName, ContainerTable* containerFromSearch);


	template<typename T>
	T FindTByModules(vertexIdType startModuleForSearch, function<T(ModuleTable* modTab)> predicate) const {
		vector<vertexIdType> forSearch = { startModuleForSearch };

		for (size_t index = 0; index < forSearch.size(); index++) {
			ModuleTable* modTable = GetModuleTableByItsId(forSearch.at(index));

			T modElement = predicate(modTable);

			if (modElement != nullptr) {
				return modElement;
			}

			for (auto needModuleId : modTable->GetNeedModules()) {
				forSearch.push_back(needModuleId);
			}
		}

		return nullptr;
	}


	// Get method variable from local vision vector of defined variables from end to begin
	Variable* GetMethodVariableLink(vector<Variable*>& methodVars, size_t variableIndex);


	void PerfDefineAction(DefineAction* act, vector<Variable*>& localVars);

	void PerfAssertAction(AssertAction* act, vector<Variable*>& localVars);

	void PerfRemoveSpecificVariable(RemoveSpecificVariable* act, vector<Variable*>& localVars);

	void PerfSkipRemoveMethLocalCariable(vector<Variable*>& localVars) noexcept;

	/*
	  Calculare variable, with calculate contain method act
	  Not change the variable construction
	  So the method create new variable
	  Return finish result as new variable
	  Finish result maybe only simple or complex variable
	*/
	Variable* CalculateVariable(Variable* variableConstr,
		vector<Variable*>& methodLocalVars);

	Bool* PerfLogicalAction(LogicalAction* act,
		vector<Variable*>& methodLocalVars);

	Bool* PerfComparibleAction(ComparibleAction* act,
		vector<Variable*>& methodLocalVars);

	Variable* PerfMathAction(MathAction* act,
		vector<Variable*>& methodLocalVars);

	// If method return void type return nullptr
	Variable* PerfGoToMethod(GoToMethod* act, vector<Variable*>& methodLocalVars, ContainerTable* containerFromSearch);

	// If it is basci instruction of interpretator perform by interpretator
	bool PerfReservedMethod(GoToMethod* act, Variable** returnVar);

	// Instruction of dbController manage
	bool PerfReserverDbControllerMethods(GoToMethod* act, Variable** returnVar);

	// Instruction of iis manage
	bool PerfReservedIISMethods(GoToMethod* act, Variable** returnVar);

	// Instruction of net connection manage
	bool PerfReservedNetMethods(GoToMethod* act, Variable** returnVar);

	// Instruction of general action manage
	bool PerfReservedGeneralMethods(GoToMethod* act, Variable** returnVar);


	Variable* PerfCastAction(CastAction* act, vector<Variable*>& methodLocalVars);


	void AssertFromTo(Variable* to, Variable* from);

	bool CheckEqualArgumentsParametrs(const vector<Variable*>& parametrs, const vector<Variable*>& arguments) const;


	// Module or method variable
	Variable* GetDefinedVariable(LinkToVariable* specVar, vector<Variable*>& methodLocalVars);

	// Create new variable for define action
	Variable* CreateVariableForDefine(Variable* var, ContainerTable* containerForSearch, vector<Variable*> methodLocalVars);

	// Cast simple calculated variable from type to to type
	Variable* CastFromTo(Variable* from, Variable* to) const;


	Struct* Interpretator::GetFromSubNode(const SubNode subNode);

	Variable* GetFromStack() noexcept;

	void PushToStack(Variable* variable);
};