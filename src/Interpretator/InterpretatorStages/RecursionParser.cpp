#include "RecursionParser.h"

void RecursionParser::MakeRecourseParsing(const vertexIdType startModuleId) {
    vector<vertexIdType> modulesForParsing;

    modulesForParsing.push_back(startModuleId);

    for (size_t index = 0; index < modulesForParsing.size(); index++) {
        m_moduleIdInParsing = modulesForParsing.at(index);
        m_moduleText = GetModuleText(m_moduleIdInParsing);
        m_moduleTextIndex = 0;
        m_moduleTextLineNumber = 1;

        // Parse modules in need order
        ModuleTable* table = ParseModule();

        m_moduleTables.push_back(table);

        // Add need modules to parse need order
        vector<vertexIdType> needModuleIdsForThat =
            m_moduleTables.back()->GetNeedModules();

        for (vertexIdType needModuleId : needModuleIdsForThat) {
            if (find(modulesForParsing.begin(), modulesForParsing.end(), needModuleId)
                == modulesForParsing.end()) {
                modulesForParsing.push_back(needModuleId);
            }
        }

        // Check start method if this parsed module is Main with id 1
        if (table->GetModuleId() == startModuleId) {
            m_startModuleMethodTable = table->GetMethodTable("main", vector<Variable*>{});
        }
    };

    if (m_startModuleMethodTable == nullptr) {
        throw std::string("Not defined start void method \"main\" without arguments in start module with id " + std::to_string(startModuleId));
    }
}

vector<vertexIdType> RecursionParser::MakeRecourseReparsing(const vertexIdType moduleId) {
    vector<vertexIdType> reparsedModuleIds;

    // If module is exists
    for (size_t index = 0; index < m_moduleTables.size(); index++) {
        if (*(m_moduleTables.at(index)) == moduleId) {
            delete m_moduleTables.at(index);

            m_moduleTables.erase(m_moduleTables.begin() + index);

            break;
        }
    }

    vector<vertexIdType> modulesForParsing = { moduleId };

    for (size_t index = 0; index < modulesForParsing.size(); index++) {
        m_moduleIdInParsing = modulesForParsing.at(index);
        m_moduleText = GetModuleText(m_moduleIdInParsing);
        m_moduleTextIndex = 0;
        m_moduleTextLineNumber = 1;

        // Parse modules in need order
        ModuleTable* table = ParseModule();

        m_moduleTables.push_back(table);
        reparsedModuleIds.push_back(table->GetModuleId());

        // Add need modules to parse need order
        vector<vertexIdType> needModuleIdsForThat =
            m_moduleTables.back()->GetNeedModules();

        for (const vertexIdType needModuleId : needModuleIdsForThat) {
            bool found = false;

            for (ModuleTable*& moduleTable : m_moduleTables) {
                if (*moduleTable == needModuleId) {
                    found = true;

                    break;
                }
            }

            if (!found) {
                modulesForParsing.push_back(needModuleId);
            }
        }
    };

    return reparsedModuleIds;
}

ModuleTable* RecursionParser::ParseModule() {
    ModuleTable* moduleTable = new ModuleTable(m_moduleIdInParsing, GetModuleName(m_moduleIdInParsing));
    
    for (string lex = GetLex(); lex != "}" && lex != "EOF"; lex = GetLex()) {
        m_moduleTextIndex -= lex.size();

        if (lex == "using") {
            ParseModuleImport(moduleTable);
        }
        else {
            ParseContainerTableElement(moduleTable);
        }
    }

    return moduleTable;
}

void RecursionParser::ParseModuleImport(ModuleTable* moduleTable) {
    string lex = GetLex();

    if (lex != "using") {
        throw FormalizeThrowText("Is not exist using after required operator");
    }

    if (GetLex() != "\"") {
        throw FormalizeThrowText("Is not exist open double quote (\") begin name of using module");
    }

    string libruaryName;

    char c = ' ';
    while ((c = GetChar()) != '\"') {
        libruaryName += c;
    }

    moduleTable->AddNeedModule(GetModuleId(libruaryName));
}

void RecursionParser::ParseContainerTableElement(ContainerTable* containerTable) {
    string lex = GetLex();

    if (lex == "struct") { // Define struct
        lex = GetLex();

        if (!isalpha(lex.at(0))) {
            throw FormalizeThrowText("struct must begin with letter or '_'");
        }

        StructTable* structTable = new StructTable(lex);
        structTable->SetParent(containerTable);

        if (GetLex() != "{") {
            throw FormalizeThrowText("Must be open bracket ({)");
        }

        for (lex = GetLex(); lex != "}" && lex != "EOF"; lex = GetLex()) {
            m_moduleTextIndex -= lex.size();

            ParseContainerTableElement(structTable);
        }

        if (lex == "EOF") {
            throw FormalizeThrowText("Must be close bracket (})");
        }

        containerTable->AddStructTable(structTable);
    }
    else if (isalpha(lex.at(0)) || lex.at(0) == '_') { // Global variable or method
        m_moduleTextIndex -= lex.size();

        const size_t controlPoint = m_moduleTextIndex;

        // Define method or global variable
        Variable* varType = ParseVarType();
        string name = GetLex();
        lex = GetLex();

        if (lex == "(") { // Method
            MethodTable* methodTable = ParseMethod(varType, name);

            methodTable->SetParent(containerTable);

            containerTable->AddMethodTable(methodTable);
        }
        else if (lex == "=" || lex == ";") { // Global variable
            m_moduleTextIndex -= lex.size();
            lex = name;

            // If it more one variable with one variable type
            do {
                containerTable->AddGlobalVariable(ParseGlobalVar(varType, lex));

                lex = GetLex();
            } while (lex == ",");
        }
        else {
            throw FormalizeThrowText("Unexpected global object: " + name + " " + lex);
        }
    }
    else {
        throw FormalizeThrowText("Unexpected global object: " + lex);
    }
}

MethodTable* RecursionParser::ParseMethod(Variable* methodReturnVarType, const string& methodName) {
    MethodTable* methodTable = new MethodTable(methodName);

    methodTable->SetReturnVariableType(methodReturnVarType);
    m_methodInParsing = methodTable;
    m_variableNamesTypes.clear();
    
    vector<LineAction*>* lineActions = methodTable->GetLineActionsLink();

    // Reading parametrs
    for (string lex = GetLex(); lex != ")";) {
        m_moduleTextIndex -= lex.size();

        Variable* paramType = ParseVarType();
        
        methodTable->AddParametr(paramType);

        m_variableNamesTypes.push_back(std::pair<string, Variable*>{GetLex(), paramType});

        lineActions->push_back(new DefineAction(
            new MethodVariable(m_variableNamesTypes.size()),
            new StackElement())
        );

        if ((lex = GetLex()) == ",") {
            lex = GetLex();
        }
    }

    // Reading Zero block
    if (GetLex() == "{") {
        ParseNewBlockLevel(lineActions);
    }
    else {
        throw FormalizeThrowText("Undefined open bracket ('{' for line actions)");
    }
    
    if (methodReturnVarType->GetVariableType() == VariableType::VoidType
        && lineActions->back()->GetLineActionType() != LineActionType::ReturnFromMethodType) {
        lineActions->push_back(new ReturnFromMethodAction());
    }

    return methodTable;
}

void RecursionParser::ParseNewBlockLevel(vector<LineAction*>* lineActions) {
    string lex = "";
    size_t toClearInTheEnd = 0;

    while (true) {
        lex = GetLex();

        if (lex == "{") {
            ParseNewBlockLevel(lineActions);
        }
        else if (lex == "}") {
            if (lineActions->back()->GetLineActionType() != LineActionType::ReturnFromMethodType
                && toClearInTheEnd > 0) {
                lineActions->push_back(new RemoveSpecificVariable(VariableType::MethodVariableLinkType, toClearInTheEnd));

                for (size_t number = 1; number <= toClearInTheEnd; number++) {
                    m_variableNamesTypes.pop_back();
                }
            }

            return;
        }
        else if (lex == "EOF") {
            throw FormalizeThrowText("Method not closed");
        }
        else if (lex != ";") {
            m_moduleTextIndex -= lex.size();

            ParseExpandeLineAction(lineActions, toClearInTheEnd);
        }
    }
}

void RecursionParser::ParseNewBlockLevelOrLineAction(vector<LineAction*>* lineActions) {
    string lex = GetLex();
    
    if (lex == "{") {
        ParseNewBlockLevel(lineActions);
    }
    else {
        m_moduleTextIndex -= lex.size();

        size_t toClearInTheEndIf = 0;
        ParseExpandeLineAction(lineActions, toClearInTheEndIf);

        if (toClearInTheEndIf > 0) {
            lineActions->push_back(new RemoveSpecificVariable(VariableType::MethodVariableLinkType, toClearInTheEndIf));

            for (size_t number = 1; number <= toClearInTheEndIf; number++) {
                m_variableNamesTypes.pop_back();
            }
        }
    }
}

void RecursionParser::ParseExpandeLineAction(vector<LineAction*>* lineActions, size_t& toClearInTheEnd) {
    string lex = GetLex();
    
    if (lex == "if" /* || lex == "switch" */) {
        m_moduleTextIndex -= lex.size();

        ParseConditionalConstr(lineActions);
    }
    else if (lex == "for" || lex == "while" || lex == "do") {
        ParseCyclicConsrt(lineActions, lex);
    }
    else if (lex == "return") {
        ParseReturnConstr(lineActions, toClearInTheEnd);
    }
    else if (lex != ";") {
        m_moduleTextIndex -= lex.size();

        ParseLineAction(lineActions, toClearInTheEnd);
    }
}

void RecursionParser::ParseLineAction(vector<LineAction*>* lineActions, size_t &toClearInTheEnd) {
    // Simple line actions. Without goto in current method
    const size_t startPoint = m_moduleTextIndex;

    Variable* definedOrVarType = ParseDefinedOrVarType();

    string lex = GetLex();

    m_moduleTextIndex -= lex.size();

    if (definedOrVarType->GetVariableType() == VariableType::GoToMethodType
        || (definedOrVarType->GetVariableType() == VariableType::VariablePathType
            && static_cast<VariablePath*>(definedOrVarType)->GetVarPathBack()->GetVariableType() == VariableType::GoToMethodType)) {
        lineActions->push_back(new GoToMethodEmpty((GoToMethod*&)definedOrVarType));
    }
    else if (lex == "=") {
        // Assert action
        lineActions->push_back(ParseAssertAction(static_cast<LinkToVariable*>(definedOrVarType)));
    }
    else if (lex.at(0) == '_' || isalpha(lex.at(0))) {
        // Define action
        ParseDefineAction(lineActions, definedOrVarType, toClearInTheEnd);
    }
    else {
        throw FormalizeThrowText("Is not programed in parser.");
    }

    if ((lex = GetLex()) == ")") { // I don't member why it is need...
        lex = GetLex();
    }

    if (lex != ";") {
        throw FormalizeThrowText("Here must be ;");
    }
}

AssertAction* RecursionParser::ParseAssertAction(LinkToVariable* definedVar) {
    Variable* rValue = nullptr;
    string lex = GetLex();

    if (lex == "=") {
        rValue = ParseVariableConstruction();
    }
    
    AssertAction* assertAct = new AssertAction(definedVar, nullptr);

    if (definedVar->GetVariableType() == VariableType::MethodVariableLinkType) {
        size_t rIndexLocalVar = static_cast<MethodVariable*>(definedVar)->GetVariableIndex();
        Variable* definedVarType = m_variableNamesTypes.at(m_variableNamesTypes.size() - 1 - rIndexLocalVar).second;
        
        if (definedVarType->GetVariableType() != VariableType::AnyType) {
            rValue = new CastAction(rValue, definedVarType);
        }

        assertAct->SetRValue(rValue);
    }
    else {
        assertAct->SetRValue(rValue);
    }

    return assertAct;
}

void RecursionParser::ParseDefineAction(vector<LineAction*>* lineActions, Variable* varType, size_t& toClearInTheEnd) {
    string lex = "";

    do {
        string varName = GetLex();
        Variable* varConstr = nullptr;

        if (!isalpha(varName.at(0)) && varName.at(0) != '_') {
            throw FormalizeThrowText("Variable name cannot be start from " + string(varName.at(0), 1) + " symbol");
        }

        lex = GetLex();

        if (lex == "=") {
            varConstr = ParseVariableConstruction();

            if (varType->GetVariableType() != VariableType::AnyType) {
                varConstr = new CastAction(varConstr, varType);
            }

            lex = GetLex();
        }
        else {
            varConstr = varType;
        }

        lineActions->push_back(
            new DefineAction(
                new MethodVariable(m_variableNamesTypes.size()),
                varConstr)
        );
        m_variableNamesTypes.push_back(std::pair<string, Variable*>(varName, varType));

        toClearInTheEnd++;
    } while (lex == ",");

    if (lex != ";") {
        throw FormalizeThrowText("Here must be semicolon (;)");
    }

    m_moduleTextIndex -= lex.size();
}

void RecursionParser::ParseConditionalConstr(vector<LineAction*>* lineActions) {
    string lex = GetLex(); // May be only "if"

    if ((lex = GetLex()) != "(") {
        throw FormalizeThrowText("Here must be arguments of if construction");
    }

    GotoActionWithIf* gotoActionWithIf = new GotoActionWithIf(new LogicalNOT(ParseVariableConstruction()));
    lineActions->push_back(gotoActionWithIf);

    if ((lex = GetLex()) != ")") {
        throw FormalizeThrowText("Here must be closing bracket (')'))");
    }

    size_t goAt = lineActions->size();

    ParseNewBlockLevelOrLineAction(lineActions);

    gotoActionWithIf->SetPtrDiff(lineActions->size() - goAt + 1);

    if ((lex = GetLex()) == "else") { // Else construction
        GotoAction* gotoAction = new GotoAction();
        lineActions->push_back(gotoAction);

        gotoActionWithIf->SetPtrDiff(gotoActionWithIf->GetPtrDiff() + 1);
        
        goAt = lineActions->size();

        ParseNewBlockLevelOrLineAction(lineActions);

        gotoAction->SetPtrDiff(lineActions->size() - goAt + 1);
    }
    else {
        m_moduleTextIndex -= lex.size();
    }
}

void RecursionParser::ParseCyclicConsrt(vector<LineAction*>* lineActions, const string& cyclingKeyWord) {
    string lex = cyclingKeyWord;
    size_t goAt = lineActions->size();

    if (lex == "do") {
        ParseNewBlockLevelOrLineAction(lineActions);

        if ((lex = GetLex()) != "while") {
            throw FormalizeThrowText("Here must be while with logical or comparible argument");
        }

        if ((lex = GetLex()) != "(") {
            throw FormalizeThrowText("Here must be opened bracket");
        }

        Variable* logicalVar = ParseVariableConstruction();

        if ((lex = GetLex()) != ")") {
            throw FormalizeThrowText("Here must be closed bracket");
        }

        lineActions->push_back(new GotoActionWithIf(goAt - lineActions->size(), logicalVar));
    }
    else if (lex == "while") {
        if ((lex = GetLex()) != "(") {
            throw FormalizeThrowText("Here must be opened bracket");
        }

        GotoActionWithIf* gotoActionToOut = new GotoActionWithIf(new LogicalNOT(ParseVariableConstruction()));

        lineActions->push_back(gotoActionToOut);

        if ((lex = GetLex()) != ")") {
            throw FormalizeThrowText("Here must be closed bracket");
        }

        ParseNewBlockLevelOrLineAction(lineActions);

        lineActions->push_back(new GotoAction(goAt - lineActions->size()));

        gotoActionToOut->SetPtrDiff(lineActions->size() - goAt);
    }
    else if (lex == "for") {
        if ((lex = GetLex()) != "(") {
            throw FormalizeThrowText("Here must be opened bracket");
        }

        size_t toClearInTheEnd = 0;
        vector<LineAction*>* lastLineActions = new vector<LineAction*>;

        ParseLineAction(lineActions, toClearInTheEnd);

        goAt = lineActions->size();

        GotoActionWithIf* toSkipForGoto = new GotoActionWithIf(lineActions->size(), new LogicalNOT(ParseVariableConstruction()));
        lineActions->push_back(toSkipForGoto);

        if ((lex = GetLex()) != ";") {
            throw FormalizeThrowText("Here must be ';' but not " + lex);
        }

        ParseLineAction(lastLineActions, toClearInTheEnd);

        if ((lex = GetLex()) != ")") {
            throw FormalizeThrowText("Here must be closed bracket");
        }

        ParseNewBlockLevelOrLineAction(lineActions);

        for (size_t index = 0; index < lastLineActions->size(); index++) {
            lineActions->push_back(lastLineActions->at(index));
        }

        lineActions->push_back(new GotoAction(goAt - lineActions->size()));

        toSkipForGoto->SetPtrDiff(lineActions->size() - toSkipForGoto->GetPtrDiff());

        // Clear defines at first line action
        if (toClearInTheEnd > 0) {
            lineActions->push_back(new RemoveSpecificVariable(VariableType::MethodVariableLinkType, toClearInTheEnd));

            for (size_t number = 1; number <= toClearInTheEnd; number++){
                m_variableNamesTypes.pop_back();
            }
        }
    }
    else {
        throw FormalizeThrowText("Not programmed cycling contruction with key word " + cyclingKeyWord);
    }
}

void RecursionParser::ParseReturnConstr(vector<LineAction*>* lineActions, size_t toClearInTheEnd) {
    string lex = GetLex();
    Variable* returnVar = nullptr;

    if (lex != ";") {
        m_moduleTextIndex -= lex.size();

        returnVar = ParseVariableConstruction();
    }

    if (m_methodInParsing->GetReturnVariableType()->GetVariableType() != VariableType::VoidType) {
        if (returnVar == nullptr) {
            throw FormalizeThrowText("return operator do not return variable but method must return specific variable"); // return var but method not return anyone
        }

        lineActions->push_back(
            new DefineAction(
                new StackElement(),
                new CastAction(returnVar, m_methodInParsing->GetReturnVariableType())
            )
        );

        // If return method variable, variable not clear for effencial perform
        if (returnVar->GetVariableType() == VariableType::MethodVariableLinkType) {
            size_t methVarRIndex = static_cast<MethodVariable*> (returnVar)->GetVariableIndex();

            if (toClearInTheEnd > 0 && methVarRIndex <= toClearInTheEnd - 1) {
                lineActions->push_back(new RemoveSpecificVariable(VariableType::MethodVariableLinkType, methVarRIndex));
                lineActions->push_back(new SkipRemoveMethLocalVariable());

                toClearInTheEnd -= methVarRIndex + 1;
            }
        }

        if (GetLex() != ";") {
            throw FormalizeThrowText("Here must be ';'");
        }
    }
    else if (returnVar != nullptr) {
        throw FormalizeThrowText("return operator try return variable but method do not return anyone"); // return var but method not return anyone
    }

    // Clear method variables
    if (toClearInTheEnd > 0) {
        lineActions->push_back(new RemoveSpecificVariable(VariableType::MethodVariableLinkType, toClearInTheEnd));
    }

    lineActions->push_back(new ReturnFromMethodAction());
}

Variable* RecursionParser::ParseVariableConstruction() {
    Variable* forGlobal = nullptr; // General result
    VariableType forGlobalType = VariableType::Null;
    string lex = "";

    for(lex = GetLex(); lex != ")" && lex != ";" && lex != "," && lex != "]"; lex = GetLex()) {
        m_moduleTextIndex -= lex.size();

        if (lex == "(") {
            GetLex();

            forGlobal = ParseVariableConstruction();

            if (GetLex() != ")") {
                throw "Here must be ')'";
            }
        }
        else if (lex == "||") {
            GetLex();

            forGlobal = new LogicalTwoArgs(VariableType::OrLogicalActionType, forGlobal, ParseLowLogicalConstr());
        }
        else if (lex == "&&") {
            GetLex();

            LogicalTwoArgs* logicalAnd = nullptr;

            if (forGlobalType == VariableType::OrLogicalActionType
                || forGlobalType == VariableType::AndLogicalActionType) {
                static_cast<LogicalTwoArgs*>(forGlobal)->SetSecondLogicalVar(
                    new LogicalTwoArgs(VariableType::AndLogicalActionType,
                        static_cast<LogicalTwoArgs*>(forGlobal)->GetSecondLogicalVar(),
                        ParseLowLogicalConstr())
                );
            }
            else {
                forGlobal = new LogicalTwoArgs(VariableType::AndLogicalActionType, forGlobal, ParseLowLogicalConstr());
            }
        }
        else if (lex == "<" || lex == ">" || lex == "==" || lex == "!="
            || lex == "<=" || lex == ">=") { // Global var is defined var or math actioh
            forGlobal = ParseComparibleConstr(forGlobal);
        }
        else if (lex == "+" || lex == "-" || lex == "*" || lex == "/") { // Global var is defined var
            forGlobal = ParseMathAction(forGlobal);
        }
        else if (isalpha(lex.at(0)) || lex.at(0) == '_'
            || isdigit(lex.at(0)) // Global var is empty
            || lex == "{"
            || lex == "\'" || lex == "\"") {
            // For Method, variables and literals
            forGlobal = ParseDefinedVariable();
        }
        else if (lex == "!") {
            forGlobal = ParseLogicalNOT();
        }
        else {
            throw FormalizeThrowText("Unexpected variable object: " + lex);
        }

        forGlobalType = forGlobal->GetVariableType();
    }

    m_moduleTextIndex-= lex.size();

    return forGlobal;
}

Variable* RecursionParser::ParseLowLogicalConstr() {
    Variable* forReturn = nullptr;
    string lex = GetLex();

    if (lex == "!") {
        forReturn = ParseLogicalNOT();

        lex = GetLex();

        m_moduleTextIndex -= lex.size();
    }
    else {
        m_moduleTextIndex -= lex.size();

        forReturn = ParseDefinedVariable();

        lex = GetLex();

        m_moduleTextIndex -= lex.size();

        if (lex == "+" || lex == "-" || lex == "*" || lex == "/") {
            forReturn = ParseMathAction(forReturn);

            lex = GetLex();

            m_moduleTextIndex -= lex.size();
        }
    }

    if (lex == "<" || lex == ">"
        || lex == "==" || lex == "!="
        || lex == "<=" || lex == ">=") {
        forReturn = ParseComparibleConstr(forReturn);
    }

    if (forReturn == nullptr) {
        throw FormalizeThrowText("Undefined logical right construction. Lex: " + lex);
    }

    return forReturn;
}

LogicalNOT* RecursionParser::ParseLogicalNOT() {
    string lex = GetLex();

    if (lex == "(") {
        return new LogicalNOT(ParseVariableConstruction());
    }
    else {
        m_moduleTextIndex -= lex.size();

        return new LogicalNOT(ParseDefinedVariable());
    }
}

ComparibleAction* RecursionParser::ParseComparibleConstr(Variable* firstVarConstr) {
    VariableType compType = VariableType::Null;
    string lex = GetLex();

    if (lex == "<") {
        compType = VariableType::ComparibleLessType;
    }
    else if (lex == ">") {
        compType = VariableType::ComparibleGreaterType;
    }
    else if (lex == "==") {
        compType = VariableType::ComparibleEqualType;
    }
    else if (lex == "!=") {
        compType = VariableType::ComparibleNEqualType;
    }
    else if (lex == "<=") {
        compType = VariableType::ComparibleLessEqualType;
    }
    else if (lex == ">=") {
        compType = VariableType::ComparibleGreaterEqualType;
    }
    else {
        throw FormalizeThrowText("Comparible action not support operator " + lex);
    }

    Variable* secondVar = nullptr;

    lex = GetLex();

    if (lex == "!") {
        secondVar = ParseLogicalNOT();
    }
    else if (lex == "(") {
        secondVar = ParseVariableConstruction();
    }
    else {
        m_moduleTextIndex -= lex.size();

        secondVar = ParseDefinedVariable();
    }

    lex = GetLex();

    m_moduleTextIndex -= lex.size();

    if (lex == "+" || lex == "-" || lex == "*" || lex == "/") {
        secondVar = ParseMathAction(secondVar);
    }

    return new ComparibleAction(compType, firstVarConstr, secondVar);
}

MathAction* RecursionParser::ParseMathAction(Variable* firstVarConstr) {
    MathAction* globalMathAction = nullptr;
    MathAction* lastMathAction = nullptr;
    VariableType mathType = VariableType::Null;
    string lex = GetLex();

    if (lex != "+" && lex != "-" && lex != "*" && lex != "/") {
        throw FormalizeThrowText("Math action not support operator " + lex);
    }

    do {
        if (lex == "+") {
            mathType = VariableType::PlusActionType;
        }
        else if (lex == "-") {
            mathType = VariableType::MinesActionType;
        }
        else if (lex == "*") {
            mathType = VariableType::TimesActionType;
        }
        else if (lex == "/") {
            mathType = VariableType::DivideActionType;
        }
        
        Variable* secondVarConstr = nullptr;

        if ((lex = GetLex()) == "(") {
            secondVarConstr = ParseMathAction(ParseDefinedVariable());

            if ((lex = GetLex()) != ")") {
                throw FormalizeThrowText("Must be closed brace (')')");
            }
        }
        else {
            m_moduleTextIndex -= lex.size();

            secondVarConstr = ParseDefinedVariable();
        }

        MathAction* localMathAct = new MathAction(mathType, firstVarConstr, secondVarConstr);
        firstVarConstr = localMathAct->GetVar2();

        if (lastMathAction != nullptr) {
            if (mathType == VariableType::PlusActionType || mathType == VariableType::MinesActionType) {
                localMathAct->SetVar1(globalMathAction);

                globalMathAction = localMathAct;
            }
            else {
                lastMathAction->SetVar2(localMathAct);
                
                lastMathAction = localMathAct;
            }
        }
        else {
            lastMathAction = localMathAct;
            globalMathAction = localMathAct;
        }

        lex = GetLex();
    } while (lex == "+" || lex == "-" || lex == "*" || lex == "/" || lex == "(");

    m_moduleTextIndex -= lex.size();

    return globalMathAction;
}

GoToMethod* RecursionParser::ParseMethodGoTo() {
    GoToMethod* goToMethod = new GoToMethod(GetLex());
    string lex = GetLex();

    // Reading method arguments
    if (lex == "(") {
        lex = GetLex();

        if (lex != ")") {
            m_moduleTextIndex -= lex.size();

            do {
                goToMethod->AddArgument(ParseVariableConstruction());
            } while ((lex = GetLex()) == ",");

            if (lex != ")") {
                throw FormalizeThrowText("Here must be close bracket ')'");
            }
        }
    }
    else {
        throw FormalizeThrowText("Here must be open bracket ('(')");
    }

    return goToMethod;
}

Variable* RecursionParser::ParseDefinedOrVarType() {
    size_t controlPoint = m_moduleTextIndex;
    string lex = "";

    // If it is varType
    Variable* retVar = ParseVarType();

    // If it is variable path or method
    lex = GetLex();

    if (lex == "." || lex == "(") {
        m_moduleTextIndex = controlPoint;
        delete retVar;

        return ParseDefinedVariable();
    }
    else {
        m_moduleTextIndex -= lex.size();

        Variable* mayNamedObj = retVar;

        if (retVar->GetVariableType() == VariableType::ArrayTemplateType) {
            mayNamedObj = static_cast<ArrayTemplate*>(retVar)->GetObjectTemplate();
        }

        if (mayNamedObj->GetVariableType() == VariableType::NamedObjectType) {
            size_t rIndex = FindRIndexLocalVar(static_cast<NamedObject*>(mayNamedObj)->GetVariableName());

            if (rIndex < m_variableNamesTypes.size()) {
                delete retVar;

                retVar = new MethodVariable(rIndex);
            }
        }
    }

    return retVar;
}

Variable* RecursionParser::ParseDefinedVariable(const bool parseVarPath, const bool canBeMethodLocalVar) {
    Variable* var = nullptr;
    string lex = GetLex();
    const size_t indexStart = m_moduleTextIndex - lex.size();

    if (lex == "\"") {
        String_t* varStr = new String_t();

        for (lex = GetChar();
            lex != "\"" ||
            (lex == "\"" && varStr->GetString().size() > 0 && varStr->GetString().back() == '\\');
            lex = GetChar()) {
            varStr->SetString(varStr->GetString() + lex);
        }

        var = varStr;
    }
    else if (lex == "\'") {
        lex = GetLex();

        if (lex.size() != 1) {
            throw FormalizeThrowText("Char can contain only single symbol");
        }
        if (GetLex() != "\'") {
            throw FormalizeThrowText("Is not exists close \"\'\"");
        }

        var = new UByte(lex.at(0));
    }
    else if (lex == "{") {
        lex = GetLex();
        Array* arrRet = new Array();
        vector<Variable*> vars;

        while (lex != "}") {
            m_moduleTextIndex -= lex.size();

            vars.push_back(ParseDefinedVariable());

            lex = GetLex();

            if (lex == ",") {
                lex = GetLex();
            }
        }

        if (vars.size() > 0) {
            arrRet->SetArray(vars, vars[0]->Clone());
        }

        var = arrRet;
    }
    else if (lex == "-") {
        var = new Long(-stoll(GetLex()));
    }
    else if (isdigit(lex.at(0))) {
        var = new ULong(stoull(lex));
    }
    else if (lex == "true" || lex == "false") {
        var = new Bool(lex == "true");
    }
    else if (lex == "null") {
        var = new NullObject();
    }
    else if (isalpha(lex.at(0)) || lex.at(0) == '_') { // Method | global, local variable | struct path
        string nextLex = GetLex();
        m_moduleTextIndex = indexStart;

        if (nextLex == "(") { // Method
            var = ParseMethodGoTo();

            if (parseVarPath) {
                nextLex = GetLex();;

                m_moduleTextIndex -= nextLex.size();

                if (nextLex == "." || nextLex == "[") {
                    var = ParseVariablePath(var);
                }
            }
        }
        else if (parseVarPath && (nextLex == "." || nextLex == "[")) { // Struct path (method, variable, array element or struct variable)
            var = (Variable*)ParseVariablePath();
        }
        else { // Global or local variable
            lex = GetLex();
            const size_t rIndexLocalVar = FindRIndexLocalVar(lex);

            if (rIndexLocalVar < m_variableNamesTypes.size() && canBeMethodLocalVar) {
                var = new MethodVariable(rIndexLocalVar);
            }
            else {
                var = new NamedObject(lex);
            }
        }
    }
    else {
        throw FormalizeThrowText("Unexpected variable object: " + lex);
    }

    return var;
}

VariablePath* RecursionParser::ParseVariablePath(Variable* firstPathElem) {
    VariablePath* varPath = new VariablePath();
    size_t controllPoint = m_moduleTextIndex;

    if (firstPathElem == nullptr) {
        firstPathElem = ParseDefinedVariable(false);
    }

    varPath->AddVariabicalPath(firstPathElem);

    string lex = GetLex();

    do {
        if (lex == ".") {
            varPath->AddVariabicalPath(ParseDefinedVariable(false, false));
        }
        else if (lex == "[") {
            varPath->AddVariabicalPath(new ArrayIndex(ParseVariableConstruction()));

            if ((lex = GetLex()) != "]") {
                throw FormalizeThrowText("Must be closed ']'. Lex: " + lex);
            }
        }
        else {
            throw FormalizeThrowText("Undefined object in variable path. Lex: " + lex);
        }

        lex = GetLex();
    } while (lex == "." || lex == "[");

    m_moduleTextIndex -= lex.size();

    return varPath;
}

Variable* RecursionParser::ParseVarType() {
    Variable* varType = nullptr;
    string lex = GetLex();

    if (lex == "any") {
        varType = new Any();
    }
    else if (lex == "void") {
        varType = new Void();
    }
    else if (lex == "bool") {
        varType = new Bool();
    }
    else if (lex == "byte") {
        varType = new Byte();
    }
    else if (lex == "ubyte") {
        varType = new UByte();
    }
    else if (lex == "int") {
        varType = new Int();
    }
    else if (lex == "uint") {
        varType = new UInt();
    }
    else if (lex == "long") {
        varType = new Long();
    }
    else if (lex == "ulong") {
        varType = new ULong();
    }
    else if (lex == "double") {
        varType = new Double();
    }
    else if (lex == "string") {
        varType = new String_t();
    }
    else if (lex == "tree") {
        varType = new Tree();
    }
    else {
        if (!(isalpha(lex.at(0)) || lex.at(0) == '_')) {
            throw FormalizeThrowText("Undefined varType or variable name");
        }

        varType = new NamedObject(lex);
    }

    size_t controlPoint = m_moduleTextIndex;
    lex = GetLex();

    if (lex == "[") {
        // TODO: Multiarrea array make logic
        lex = GetLex();
       
        if (lex == "]") {
            varType = new ArrayTemplate(varType, 0);
        }
        else if (isdigit(lex.at(0))) {
            varType = new ArrayTemplate(varType, stoull(lex));

            if (GetLex() != "]") {
                throw FormalizeThrowText("Error");
            }
        }
        else if (isalpha(lex.at(0)) || lex.at(0) == '_') {
            m_moduleTextIndex = controlPoint;

            if (varType->GetVariableType() == VariableType::NamedObjectType) {
                const size_t rIndexLocalVar = FindRIndexLocalVar(static_cast<NamedObject*>(varType)->GetVariableName());

                if (rIndexLocalVar < m_variableNamesTypes.size()) {
                    delete varType;

                    varType = new MethodVariable(rIndexLocalVar);
                }
            }

            varType = ParseVariablePath(varType);
        }
        else {
            throw FormalizeThrowText("Undefined array size");
        }
    }
    else {
        m_moduleTextIndex -= lex.size();
    }

    return varType;
}

ContainerVariable* RecursionParser::ParseGlobalVar(Variable* varType, const string& varName) {
    Variable* defineState = varType;
    string lex = GetLex();
    
    if (lex == "=") {
        defineState = new CastAction(ParseVariableConstruction(), defineState);
    }
    else {
        m_moduleTextIndex -= lex.size();
    }

    return new ContainerVariable(varName, defineState);
}

size_t RecursionParser::FindRIndexLocalVar(const string& varName) {
    for (size_t indexInEnd = 0; indexInEnd < m_variableNamesTypes.size(); indexInEnd++) {
        if (m_variableNamesTypes.at(m_variableNamesTypes.size() - indexInEnd - 1).first == varName) {
            return indexInEnd;
        }
    }

    return m_variableNamesTypes.size();;
}

string RecursionParser::GetLex() {
    enum class state { H, IDENT, NUMB, COM, LOGICAL, ALE, COMMENT };
    string buf = "";
    state CS = state::H;
    char c = ' ';
    do {
        if (m_moduleTextIndex >= m_moduleText.size()) {
            return "EOF";
        }

        c = GetChar();

        switch (CS) {
        case state::H:
            if (c == '\n') {
                m_moduleTextLineNumber++;
            }
            else if ( c == ' ' || c == '\r' || c == '\t');
            else if (isdigit(c)) {
                buf.push_back(c);

                CS = state::NUMB;
            }
            else if (isalpha(c) || c == '_') {
                buf.push_back(c);

                CS = state::IDENT;
            }
            else if (c == '|' || c == '&' || c == '!') {
                buf.push_back(c);

                CS = state::LOGICAL;
            }
            else if (c == '/') {
                buf.push_back(c);

                CS = state::COMMENT;
            }
            else if (c == '<' || c == '>' || c == '=') {
                buf.push_back(c);

                CS = state::ALE;
            }
            else {
                buf.push_back(c);

                return buf;
            }
            break;
        case state::IDENT:
            if (isalpha(c) || isdigit(c) || c == '_') {
                buf.push_back(c);
            }
            else {
                m_moduleTextIndex--;

                return buf;
            }
            break;
        case state::NUMB:
            if (isdigit(c)) {
                buf.push_back(c);
            }
            else {
                m_moduleTextIndex--;

                return buf;
            }
            break;
        case state::LOGICAL:
            if (c == '=') {
                m_moduleTextIndex--;

                CS = state::ALE;
            }
            else if (c == '|' || c == '&') {
                buf.push_back(c);

                return buf;
            }
            else {
                m_moduleTextIndex--;

                return buf;
            }
            break;
        case state::COMMENT:
            if (c == '/') {
                while (c != '\n' && m_moduleTextIndex < m_moduleText.size()) {
                    c = GetChar();
                }

                m_moduleTextLineNumber++;
            }
            else {
                m_moduleTextIndex--;

                return buf;
            }
            break;
        case state::ALE:
            if (c == '=') {
                buf.push_back(c);

                return buf;
            }
            else {
                m_moduleTextIndex--;

                return buf;
            }
            break;
        default:
            throw "Undefined error";
            break;
        }
    } while (true);
};

char RecursionParser::GetChar() {
	return m_moduleText.at(m_moduleTextIndex++);
};

vertexIdType RecursionParser::GetModuleId(const string& moduleName) {
#ifdef MULTIUSERS
    std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

	vertexIdType moduleId = 0;

	vector<SubNode>* subNodes = m_dbController->ReadNode(SWObjectModulesName)->GetSubNodes();

	for (SubNode& subNode : *subNodes) {
		Vertex* vertex = m_dbController->ReadVertex(subNode.GetSecondVertexId());

		if (vertex->GetText() == moduleName) {
			moduleId = subNode.GetFirstVertexId();

			break;
		}
	}

	return moduleId;
}

string RecursionParser::GetModuleName(const vertexIdType moduleId) {
#ifdef MULTIUSERS
    std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

    vector<SubNode> subNodes = m_dbController->ReadNode(SWObjectModulesName)->FindByFirstVertexId(moduleId);

    if (subNodes.size() > 0) {
        return m_dbController->ReadVertex(subNodes.at(0).GetSecondVertexId())->GetText();
    }
    
    return "";
}

string RecursionParser::GetModuleText(const vertexIdType moduleId) {
#ifdef MULTIUSERS
    std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

    string moduleText = "";

    vector<SubNode> subNodes = m_dbController->ReadNode(SWObjectProgramOfModules)->FindByFirstVertexId(moduleId);

    if (subNodes.size() > 0) {
        return m_dbController->ReadVertex(subNodes.at(0).GetSecondVertexId())->GetText();
    }
    
    return "";
}
