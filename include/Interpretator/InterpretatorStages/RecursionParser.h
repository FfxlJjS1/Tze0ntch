#pragma once
#include "SemanticWeb.h"
#include "DbController.h"

#include "ContainerTable.h"

#include "ModuleTable.h"
#include "StructTable.h"
#include "MethodTable.h"

#include "InterpretorHeaders.h"

extern std::mutex globalMutex;

using std::shared_ptr;

using std::stoll, std::stoull;

class RecursionParser final {
private:
	vector<ModuleTable*> &m_moduleTables;
	shared_ptr<DbController> m_dbController;
	Node* m_programModules = nullptr;
	Node* m_moduleNames = nullptr;
	MethodTable* m_startModuleMethodTable = nullptr;

public:
	RecursionParser(vector<ModuleTable*>& moduleTables,
		shared_ptr<DbController> dbController) : m_moduleTables(moduleTables), m_dbController(dbController) {
		m_programModules = dbController->ReadNode(SWObjectProgramOfModules);
		m_moduleNames = dbController->ReadNode(SWObjectModulesName);
	};

	void MakeRecourseParsing(const vertexIdType startModuleId);

	vector<vertexIdType> MakeRecourseReparsing(const vertexIdType moduleId);


	MethodTable* GetStartModuleMethodTable() {
		return m_startModuleMethodTable;
	}

private:
	// For module parsing
	vertexIdType m_moduleIdInParsing = 0;
	string m_moduleText = "";
	size_t m_moduleTextIndex = 0;
	size_t m_moduleTextLineNumber = 1;
	MethodTable* m_methodInParsing = nullptr;

	// For method parsing
	vector<std::pair<string, Variable*>> m_variableNamesTypes;
	
	//vector<string> TW = { "required", "using",
	//			"void", "bool", "char", "int", "double",
	//			"string", "stack",
	//			"struct" /*For future updating*/,
	//			"for", "do", "while",
	//			"break", "continue",
	//			"if", "else" };

	//vector<string> TD = { ",", ".",
	//		":", ";",
	//		"(", ")", "{", "}",
	//		"\\", "\'","\"",
	//		"!", "||", "&&",
	//		">", "<", "==", "!=", ">=", "<=", "!=" 
	//		"+", "-", "*", "/",};
	
	//vector<string> TM = { 
	//		"string GetStringFromVertex(vertexIdType vertexId)",
	//		"void SetStringToVertex(vertexIdType vertexId, string text)"

	ModuleTable* ParseModule();

	/*
	  Parsing all syntaxis of module import
	  syntax:
	  using "<Module name>"
	*/
	void ParseModuleImport(ModuleTable* moduleTable);

	void ParseContainerTableElement(ContainerTable* containerTable);

	/*
	  Parsing syntaxis after 'struct' word
	  <struct name> \{
	  [<var type> <varName> [= <variable constr>];
	  [,...]]
	  \}
	*/
	//StructTable* ParseStructTable(ContainerTable*& moduleTable);

	/*
	  Parsing syntax of method begin from open broce of parametrs
	  syntax:
	  [<var type> <var name>, ...]) \{<new block level>
	*/
	MethodTable* ParseMethod(Variable* methodRerurnVarType, const string& methodName);

	/*
	  Pasing syntax of new block level after '{'
	  syntax:
	  [{<line action> | anything construction}, ...] \}
	*/
	void ParseNewBlockLevel(vector<LineAction*>* lineActions);

	void ParseNewBlockLevelOrLineAction(vector<LineAction*>* lineActions);

	void ParseExpandeLineAction(vector<LineAction*>* lineActions,
		size_t& toClearInTheEnd);

	void ParseLineAction(vector<LineAction*>* lineActions,
		size_t& toClearInTheEnd);

	/*
	  Parsing all syntax of assert variable construction to method variable
	  syntax:
	  = <variable construction>;
	*/
	AssertAction* ParseAssertAction(
		LinkToVariable* definedVar);

	/*
	  Parsing all syntax of define action
	  syntax:
	  <var type> {<variable> [= <variable constructions>]
	  [, <variable> [= <variable constructions>][, ...]]};
	*/
	void ParseDefineAction(
		vector<LineAction*>* lineActions,
		Variable* varType,
		size_t& toClearInTheEnd);

	/*
	  Parsing all syntax of conditional construction
	  syntax:
	  if({<logical var> | <comparible var>})
	  {<line action> | \{<new level block>}
	  [else {<line action> | \{<new level block> }]
	*/
	void ParseConditionalConstr(vector<LineAction*>* lineActions);

	/*
	  Parsing all syntax of cyclic construction
	  syntax:
	  do [<line action> | \{<new level block>] while(<logical constr>)
	  | while(<logical constr>) {[<line action>] | \{ <new level block>}
	*/
	void ParseCyclicConsrt(vector<LineAction*>* lineActions, const string& cyclingKeyWord);

	/*
	  Parsing syntax after 'return' word
	  syntax:
	  [<Variable construction>];
	*/
	void ParseReturnConstr(vector<LineAction*>* lineActions, size_t toClearInTheEnd);

	/*
	* Parse construction of math, comparible, logic and simple, complex variable
	* Return new Variable with type of variable
	* Stoped before ',', ';', ')'
	*/
	Variable* ParseVariableConstruction();

	Variable* ParseLowLogicalConstr();

	// Parse after !
	LogicalNOT* ParseLogicalNOT();

	ComparibleAction* ParseComparibleConstr(Variable* firstVarConstr);

	/*
	  Parsing all syntax of math construction
	  syntax:
	  <variable> [+ | - | * | / | (] <variable> [)] 
	  [[+ | - | * | / | (] <variable> [)] [...]]
	*/
	MathAction* ParseMathAction(Variable* firstVarConstr);

	/*
	  Parsing all syntax of method construction
	  <method name>([<variable const> [, ...]])
	*/
	GoToMethod* ParseMethodGoTo();

	// Combine parse definedVariable and varType
	Variable* ParseDefinedOrVarType();

	/*
	  Parse gotoMethod, struct variable,
	  global variable, local variable,
	  literal
	*/
	Variable* ParseDefinedVariable(const bool parseVarPath = true, const bool canBeMethodLocalVar = true);
	
	// Path to struct, method in struct or var in struct
	VariablePath* ParseVariablePath(Variable* firstPathElem = nullptr);

	Variable* ParseVarType();

	/*
	  Parsing syntax after variable name;
	  syntax:
	  [= <variable construction>]
	*/
	ContainerVariable* ParseGlobalVar(Variable* varType, const string& varName);

	size_t FindRIndexLocalVar(const string& varName);

	string GetLex();

	char GetChar();

	vertexIdType GetModuleId(const string& moduleName);

	string GetModuleName(const vertexIdType moduleId);

	string GetModuleText(const vertexIdType moduleId);

	string FormalizeThrowText(const string& errorText) {
		return std::string("Module name: " + GetModuleName(m_moduleIdInParsing) + "; line: " + std::to_string(m_moduleTextLineNumber) + "; Error message: " + errorText + ";");
	}
};
