#include "../../../include/Interpretator/Interpretator.h"

void Interpretator::Run() {
	container = m_startModuleMethodTable->GetParent();

	DefineAllModuleContainerVariables();

	RunGoToMethod(m_startModuleMethodTable);
}

void Interpretator::ClearInterpretator() {
	// Clear module tables
	for (auto moduleTable : m_moduleTables) {
		delete moduleTable;
	}

	// Clear stack
	while (!m_stack.empty()) {
		delete GetFromStack();
	}
}

ModuleTable* Interpretator::GetParentModuleTable(ContainerTable* containerTable) {
	if (containerTable->GetContainerTableType() == ContainerTableType::ModuleTableType) {
		return static_cast<ModuleTable*>(containerTable);
	}

	ContainerTable* parentTable = static_cast<StructTable*>(containerTable)->GetParent();

	while (parentTable->GetContainerTableType() != ContainerTableType::ModuleTableType) {
		parentTable = static_cast<StructTable*>(parentTable)->GetParent();
	}

	return static_cast<ModuleTable*>(parentTable);
}

void Interpretator::DefineAllModuleContainerVariables() {
	for (size_t index = 0; index < m_moduleTables.size(); index++) {
		DefineModuleContainerVariables(m_moduleTables.at(index));
	}
}

void Interpretator::DefineModuleContainerVariables(ModuleTable* moduleTable) {
	ContainerTable* FirstlyContainerTable = container;
	container = moduleTable;

	for (size_t index = 0; index < moduleTable->GetGlobalVariablesCount(); index++) {
		ContainerVariable* contVar = moduleTable->GetGlobalVariable(index);

		contVar->SetVariable(CalculateVariable(contVar->GetVariable(), vector<Variable*>{}));
	}

	container = FirstlyContainerTable;
}

void Interpretator::RunGoToMethod(MethodTable* methodTable) {
	currModuleId = GetParentModuleTable(container)->GetModuleId();
	vector<Variable*> currMethodVariables;
	vector<LineAction*> lineActions = methodTable->GetLineActionVector();

	for (auto iterator = lineActions.begin(); iterator != lineActions.end(); iterator++) {
		LineAction* lineAction = *iterator;
		const LineActionType lineActionType = lineAction->GetLineActionType();

		if (lineActionType == LineActionType::DefineSpecificVariableType) {
			PerfDefineAction(static_cast<DefineAction*>(lineAction), currMethodVariables);
		}
		else if (lineActionType == LineActionType::AssertActionType) {
			PerfAssertAction(static_cast<AssertAction*>(lineAction), currMethodVariables);
		}
		else if (lineActionType == LineActionType::GotoActionType) {
			iterator += static_cast<GotoAction*>(lineAction)->GetPtrDiff() - 1;
		}
		else if (lineActionType == LineActionType::GotoActionWithIfType) {
			Variable* logicalVar = CalculateVariable(static_cast<GotoActionWithIf*>(lineAction)->GetOperation(), currMethodVariables);

			if (logicalVar->GetVariableType() != VariableType::BoolType) {
				throw "Is not boolean type";
			}

			if (static_cast<Bool*>(logicalVar)->GetBool()) {
				GotoActionWithIf* gotoAct = static_cast<GotoActionWithIf*>(lineAction);

				iterator += gotoAct->GetPtrDiff() - 1;
			}

			delete logicalVar;
		}
		else if (lineActionType == LineActionType::GoToMethodEmptyType) {
			Variable* goToMethodVar = static_cast<GoToMethodEmpty*>(lineAction)->GetGoToMethod();

			if (goToMethodVar->GetVariableType() == VariableType::VariablePathType) {
				delete GetDefinedVariable(static_cast<VariablePath*>(goToMethodVar), currMethodVariables);
			}
			else if (goToMethodVar->GetVariableType() == VariableType::GoToMethodType) {
				delete PerfGoToMethod(static_cast<GoToMethod*>(goToMethodVar), currMethodVariables, container);
			}
		}
		else if (lineActionType == LineActionType::RemoveSpecificVariableType) {
			PerfRemoveSpecificVariable(static_cast<RemoveSpecificVariable*>(lineAction), currMethodVariables);
		}
		else if (lineActionType == LineActionType::SkipRemoveMethLocalVariableType) {
			PerfSkipRemoveMethLocalCariable(currMethodVariables);
		}
		else if (lineActionType == LineActionType::ReturnFromMethodType) {
			return;
		}
		else {
			throw "Not programmed in interpretator";
		}
	}
}


Variable* Interpretator::CalculateVariable(Variable* variableConst, vector<Variable*>& methodLocalVars) {
	const VariableType varConstrType = variableConst->GetVariableType();

	if (varConstrType == VariableType::AnyType) {
		return static_cast<Any*>(variableConst)->GetAnyVar();
	}
	else if (varConstrType == VariableType::VoidType
		|| varConstrType == VariableType::BoolType 
		|| varConstrType == VariableType::ByteType || varConstrType == VariableType::UByteType
		|| varConstrType == VariableType::IntType || varConstrType == VariableType::UIntType
		|| varConstrType == VariableType::LongType || varConstrType == VariableType::ULongType
		|| varConstrType == VariableType::DoubleType
		|| varConstrType == VariableType::StringType) {
		return variableConst->Clone();
	}
	else if (varConstrType == VariableType::ArrayType || varConstrType == VariableType::StructType || varConstrType == VariableType::TreeType) {
		return variableConst;
	}
	else if (varConstrType == VariableType::ArrayTemplateType) {
		ArrayTemplate* arrTempl = static_cast<ArrayTemplate*>(variableConst);
		new Array(arrTempl->GetObjectTemplate(), arrTempl->GetArraySizeVar());
	}
	else if (varConstrType == VariableType::StackElementType) {
		return GetFromStack();
	}
	else if (varConstrType == VariableType::MethodVariableLinkType || varConstrType == VariableType::NamedObjectType || varConstrType == VariableType::VariablePathType) {
		return CalculateVariable(GetDefinedVariable(static_cast<LinkToVariable*>(variableConst), methodLocalVars), methodLocalVars);
	}
	else if (varConstrType == VariableType::GoToMethodType) {
		return PerfGoToMethod(static_cast<GoToMethod*>(variableConst), methodLocalVars, container);
	}
	else if (varConstrType == VariableType::AndLogicalActionType || varConstrType == VariableType::OrLogicalActionType
		|| varConstrType == VariableType::NotLogicalActionType) {
		return PerfLogicalAction(static_cast<LogicalAction*>(variableConst), methodLocalVars);
	}
	else if (varConstrType == VariableType::ComparibleEqualType || varConstrType == VariableType::ComparibleNEqualType
		|| varConstrType == VariableType::ComparibleGreaterType || varConstrType == VariableType::ComparibleGreaterEqualType
		|| varConstrType == VariableType::ComparibleLessType || varConstrType == VariableType::ComparibleLessEqualType) {
		return PerfComparibleAction(static_cast<ComparibleAction*>(variableConst), methodLocalVars);
	}
	else if (varConstrType == VariableType::PlusActionType || varConstrType == VariableType::MinesActionType
		|| varConstrType == VariableType::TimesActionType || varConstrType == VariableType::DivideActionType) {
		return PerfMathAction(static_cast<MathAction*>(variableConst), methodLocalVars);
	}
	else if (varConstrType == VariableType::GoToMethodType) {
		return PerfGoToMethod(static_cast<GoToMethod*>(variableConst), methodLocalVars, container);
	}
	else if (varConstrType == VariableType::CastActionType) {
		return PerfCastAction(static_cast<CastAction*>(variableConst), methodLocalVars);
	}
	else {
		throw "Undefined variable for calculate";
	}
}

Bool* Interpretator::PerfLogicalAction(LogicalAction* act, vector<Variable*>& methodLocalVars) {
	Bool* result = nullptr;

	if (act->GetVariableType() == VariableType::AndLogicalActionType || act->GetVariableType() == VariableType::OrLogicalActionType) {
		LogicalTwoArgs* twoLogArgAct = static_cast<LogicalTwoArgs*>(act);

		Bool* firstSimple = static_cast<Bool*>(CalculateVariable(twoLogArgAct->GetFirstLogicalVar(), methodLocalVars));
		
		if (act->GetVariableType() == VariableType::OrLogicalActionType && firstSimple->GetBool() == true) {
			result = new Bool(true);
		}
		else if (act->GetVariableType() == VariableType::AndLogicalActionType && firstSimple->GetBool() == false) {
			result = new Bool(false);
		}
		else {
			delete firstSimple;

			Bool* secondSimple = static_cast<Bool*>(CalculateVariable(twoLogArgAct->GetSecondLogicalVar(), methodLocalVars));

			result = secondSimple;
		}
	}
	else if (act->GetVariableType() == VariableType::NotLogicalActionType) {
		LogicalNOT* logNotAct = static_cast<LogicalNOT*>(act);

		result = static_cast<Bool*>(CalculateVariable(logNotAct->GetLogicalVar(), methodLocalVars));

		result->SetBool(!result->GetBool());
	}
	else {
		throw "Such logical action is not programmed in interpretator";
	}

	return result;
}

Bool* Interpretator::PerfComparibleAction(ComparibleAction* act, vector<Variable*>& methodLocalVars) {
	bool compareResult = false;

	const VariableType compariableType = act->GetVariableType();
	Variable* firstSimpleTypeVar = CalculateVariable(act->GetVar1(), methodLocalVars);
	Variable* secondSimpleTypeVar = CalculateVariable(act->GetVar2(), methodLocalVars);

	if (compariableType == VariableType::ComparibleEqualType || compariableType == VariableType::ComparibleNEqualType
		|| compariableType == VariableType::ComparibleGreaterEqualType || compariableType == VariableType::ComparibleLessEqualType) {
		if (firstSimpleTypeVar->GetVariableType() == VariableType::BoolType) {
			compareResult = static_cast<Bool*>(firstSimpleTypeVar)->GetBool() == static_cast<Bool*>(secondSimpleTypeVar)->GetBool();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::ByteType) {
			compareResult = static_cast<Byte*>(firstSimpleTypeVar)->GetByte() == static_cast<Byte*>(secondSimpleTypeVar)->GetByte();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::UByteType) {
			compareResult = static_cast<UByte*>(firstSimpleTypeVar)->GetUByte() == static_cast<UByte*>(secondSimpleTypeVar)->GetUByte();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::IntType) {
			compareResult = static_cast<Int*>(firstSimpleTypeVar)->GetInt() == static_cast<Int*>(secondSimpleTypeVar)->GetInt();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::UIntType) {
			compareResult = static_cast<UInt*>(firstSimpleTypeVar)->GetUInt() == static_cast<UInt*>(secondSimpleTypeVar)->GetUInt();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::LongType) {
			compareResult = static_cast<Long*>(firstSimpleTypeVar)->GetLong() == static_cast<Long*>(secondSimpleTypeVar)->GetLong();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::ULongType) {
			compareResult = static_cast<ULong*>(firstSimpleTypeVar)->GetULong() == static_cast<ULong*>(secondSimpleTypeVar)->GetULong();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::DoubleType) {
			compareResult = static_cast<Double*>(firstSimpleTypeVar)->GetDouble() == static_cast<Double*>(secondSimpleTypeVar)->GetDouble();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::StringType) {
			compareResult = static_cast<String_t*>(firstSimpleTypeVar)->GetString() == static_cast<String_t*>(secondSimpleTypeVar)->GetString();
		}
		else {
			throw "Interpretator not programmed for compare equal such variable types";
		}
	}

	if (compariableType == VariableType::ComparibleNEqualType) {
		compareResult = !compareResult;
	}
	else if (!compareResult &&
		(compariableType == VariableType::ComparibleGreaterType || compariableType == VariableType::ComparibleGreaterEqualType)) {
		if (firstSimpleTypeVar->GetVariableType() == VariableType::BoolType) {
			compareResult = static_cast<Bool*>(firstSimpleTypeVar)->GetBool() > static_cast<Bool*>(secondSimpleTypeVar)->GetBool();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::ByteType) {
			compareResult = static_cast<Byte*>(firstSimpleTypeVar)->GetByte() > static_cast<Byte*>(secondSimpleTypeVar)->GetByte();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::UByteType) {
			compareResult = static_cast<UByte*>(firstSimpleTypeVar)->GetUByte() > static_cast<UByte*>(secondSimpleTypeVar)->GetUByte();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::IntType) {
			compareResult = static_cast<Int*>(firstSimpleTypeVar)->GetInt() > static_cast<Int*>(secondSimpleTypeVar)->GetInt();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::UIntType) {
			compareResult = static_cast<UInt*>(firstSimpleTypeVar)->GetUInt() > static_cast<UInt*>(secondSimpleTypeVar)->GetUInt();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::LongType) {
			compareResult = static_cast<Long*>(firstSimpleTypeVar)->GetLong() > static_cast<Long*>(secondSimpleTypeVar)->GetLong();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::ULongType) {
			compareResult = static_cast<ULong*>(firstSimpleTypeVar)->GetULong() > static_cast<ULong*>(secondSimpleTypeVar)->GetULong();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::DoubleType) {
			compareResult = static_cast<Double*>(firstSimpleTypeVar)->GetDouble() > static_cast<Double*>(secondSimpleTypeVar)->GetDouble();
		}
		else {
			throw "Interpretator not programmed for compare greater such variable types";
		}
	}
	else if (!compareResult &&
		(compariableType == VariableType::ComparibleLessType || compariableType == VariableType::ComparibleLessEqualType)) {
		if (firstSimpleTypeVar->GetVariableType() == VariableType::BoolType) {
			compareResult = static_cast<Bool*>(firstSimpleTypeVar)->GetBool() < static_cast<Bool*>(secondSimpleTypeVar)->GetBool();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::ByteType) {
			compareResult = static_cast<Byte*>(firstSimpleTypeVar)->GetByte() < static_cast<Byte*>(secondSimpleTypeVar)->GetByte();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::UByteType) {
			compareResult = static_cast<UByte*>(firstSimpleTypeVar)->GetUByte() < static_cast<UByte*>(secondSimpleTypeVar)->GetUByte();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::IntType) {
			compareResult = static_cast<Int*>(firstSimpleTypeVar)->GetInt() < static_cast<Int*>(secondSimpleTypeVar)->GetInt();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::UIntType) {
			compareResult = static_cast<UInt*>(firstSimpleTypeVar)->GetUInt() < static_cast<UInt*>(secondSimpleTypeVar)->GetUInt();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::LongType) {
			compareResult = static_cast<Long*>(firstSimpleTypeVar)->GetLong() < static_cast<Long*>(secondSimpleTypeVar)->GetLong();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::ULongType) {
			compareResult = static_cast<ULong*>(firstSimpleTypeVar)->GetULong() < static_cast<ULong*>(secondSimpleTypeVar)->GetULong();
		}
		else if (firstSimpleTypeVar->GetVariableType() == VariableType::DoubleType) {
			compareResult = static_cast<Double*>(firstSimpleTypeVar)->GetDouble() < static_cast<Double*>(secondSimpleTypeVar)->GetDouble();
		}
		else {
			throw "Interpretator not programmed for compare less such variable types";
		}
	}

	return new Bool(compareResult);
}

Variable* Interpretator::PerfMathAction(MathAction* act, vector<Variable*>& methodLocalVars) {
	Variable* resVar = nullptr;

	const VariableType mathActionType = act->GetVariableType();
	Variable* firstSimpleVar = CalculateVariable(act->GetVar1(), methodLocalVars);
	Variable* secondSimpleVar = CalculateVariable(act->GetVar2(), methodLocalVars);

	if (mathActionType == VariableType::PlusActionType) {
		if (firstSimpleVar->GetVariableType() == VariableType::ByteType) {
			resVar = new Byte(static_cast<Byte*>(firstSimpleVar)->GetByte() + static_cast<Byte*>(secondSimpleVar)->GetByte());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::UByteType) {
			resVar = new UByte(static_cast<UByte*>(firstSimpleVar)->GetUByte() + static_cast<UByte*>(secondSimpleVar)->GetUByte());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::IntType) {
			resVar = new Int(static_cast<Int*>(firstSimpleVar)->GetInt() + static_cast<Int*>(secondSimpleVar)->GetInt());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::UIntType) {
			resVar = new UInt(static_cast<UInt*>(firstSimpleVar)->GetUInt() + static_cast<UInt*>(secondSimpleVar)->GetUInt());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::LongType) {
			resVar = new Long(static_cast<Long*>(firstSimpleVar)->GetLong() + static_cast<Long*>(secondSimpleVar)->GetLong());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::ULongType) {
			resVar = new ULong(static_cast<ULong*>(firstSimpleVar)->GetULong() + static_cast<ULong*>(secondSimpleVar)->GetULong());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::DoubleType) {
			resVar = new Double(static_cast<Double*>(firstSimpleVar)->GetDouble() + static_cast<Double*>(secondSimpleVar)->GetDouble());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::StringType) {
			resVar = new String_t(static_cast<String_t*>(firstSimpleVar)->GetString() + static_cast<String_t*>(secondSimpleVar)->GetString());
		}
		else {
			throw "Not exists that math action";
		}
	}
	else if (mathActionType == VariableType::MinesActionType) {
		if (firstSimpleVar->GetVariableType() == VariableType::ByteType) {
			resVar = new Byte(static_cast<Byte*>(firstSimpleVar)->GetByte() - static_cast<Byte*>(secondSimpleVar)->GetByte());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::UByteType) {
			resVar = new UByte(static_cast<UByte*>(firstSimpleVar)->GetUByte() - static_cast<UByte*>(secondSimpleVar)->GetUByte());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::IntType) {
			resVar = new Int(static_cast<Int*>(firstSimpleVar)->GetInt() - static_cast<Int*>(secondSimpleVar)->GetInt());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::UIntType) {
			resVar = new UInt(static_cast<UInt*>(firstSimpleVar)->GetUInt() - static_cast<UInt*>(secondSimpleVar)->GetUInt());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::LongType) {
			resVar = new Long(static_cast<Long*>(firstSimpleVar)->GetLong() - static_cast<Long*>(secondSimpleVar)->GetLong());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::ULongType) {
			resVar = new ULong(static_cast<ULong*>(firstSimpleVar)->GetULong() - static_cast<ULong*>(secondSimpleVar)->GetULong());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::DoubleType) {
			resVar = new Double(static_cast<Double*>(firstSimpleVar)->GetDouble() - static_cast<Double*>(secondSimpleVar)->GetDouble());
		}
		else {
			throw "Not exists that math action";
		}
	}
	else if (mathActionType == VariableType::TimesActionType) {
		if (firstSimpleVar->GetVariableType() == VariableType::ByteType) {
			resVar = new Byte(static_cast<Byte*>(firstSimpleVar)->GetByte() * static_cast<Byte*>(secondSimpleVar)->GetByte());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::UByteType) {
			resVar = new UByte(static_cast<UByte*>(firstSimpleVar)->GetUByte() * static_cast<UByte*>(secondSimpleVar)->GetUByte());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::IntType) {
			resVar = new Int(static_cast<Int*>(firstSimpleVar)->GetInt() * static_cast<Int*>(secondSimpleVar)->GetInt());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::UIntType) {
			resVar = new UInt(static_cast<UInt*>(firstSimpleVar)->GetUInt() * static_cast<UInt*>(secondSimpleVar)->GetUInt());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::LongType) {
			resVar = new Long(static_cast<Long*>(firstSimpleVar)->GetLong() * static_cast<Long*>(secondSimpleVar)->GetLong());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::ULongType) {
			resVar = new ULong(static_cast<ULong*>(firstSimpleVar)->GetULong() * static_cast<ULong*>(secondSimpleVar)->GetULong());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::DoubleType) {
			resVar = new Double(static_cast<Double*>(firstSimpleVar)->GetDouble() * static_cast<Double*>(secondSimpleVar)->GetDouble());
		}
		else {
			throw "Not exists that math action";
		}
	}
	else if (mathActionType == VariableType::DivideActionType) {
		if (firstSimpleVar->GetVariableType() == VariableType::ByteType) {
			resVar = new Byte(static_cast<Byte*>(firstSimpleVar)->GetByte() / static_cast<Byte*>(secondSimpleVar)->GetByte());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::UByteType) {
			resVar = new UByte(static_cast<UByte*>(firstSimpleVar)->GetUByte() / static_cast<UByte*>(secondSimpleVar)->GetUByte());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::IntType) {
			resVar = new Int(static_cast<Int*>(firstSimpleVar)->GetInt() / static_cast<Int*>(secondSimpleVar)->GetInt());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::UIntType) {
			resVar = new UInt(static_cast<UInt*>(firstSimpleVar)->GetUInt() / static_cast<UInt*>(secondSimpleVar)->GetUInt());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::LongType) {
			resVar = new Long(static_cast<Long*>(firstSimpleVar)->GetLong() / static_cast<Long*>(secondSimpleVar)->GetLong());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::ULongType) {
			resVar = new ULong(static_cast<ULong*>(firstSimpleVar)->GetULong() / static_cast<ULong*>(secondSimpleVar)->GetULong());
		}
		else if (firstSimpleVar->GetVariableType() == VariableType::DoubleType) {
			resVar = new Double(static_cast<Double*>(firstSimpleVar)->GetDouble() / static_cast<Double*>(secondSimpleVar)->GetDouble());
		}
		else {
			throw "Not exists that math action";
		}
	}
	else {
	throw "Undefined math action";
	}

	return resVar;
}

Variable* Interpretator::PerfGoToMethod(GoToMethod* act, vector<Variable*>& methodLocalVars, ContainerTable* containerFromSearch) {
	Variable* returnVar = nullptr; // Variable will contains return variable from method if it will be
	GoToMethod* calculatedGoToMeth = new GoToMethod(act->GetMethodName()); // Copy of declaration
	
	for (Variable* arg : act->GetArguments()) {
		calculatedGoToMeth->AddArgument(CalculateVariable(arg, methodLocalVars)); // Get need args for method
	}
	
	if (!PerfReservedMethod(calculatedGoToMeth, &returnVar)) {
		MethodTable* methodTableForRun = nullptr;

		{
			methodTableForRun = FindMethodByAdditions(calculatedGoToMeth, containerFromSearch);

			if (methodTableForRun == nullptr) {
				throw "Not found method with name " + act->GetMethodName();
			}
		}

		for (auto iterator = calculatedGoToMeth->GetArguments().rbegin(); iterator != calculatedGoToMeth->GetArguments().rend(); iterator++) {
			PushToStack(*iterator);
		}

		ContainerTable* containerFrom = container;
		container = methodTableForRun->GetParent();

		RunGoToMethod(methodTableForRun);

		container = containerFrom;
		currModuleId = GetParentModuleTable(container)->GetModuleId();

		if (methodTableForRun->GetReturnVariableType()->GetVariableType() != VariableType::VoidType) {
			returnVar = GetFromStack();
		}
	}
	
	// Clear calculated GoToMethod
	for (size_t index = 0; index < calculatedGoToMeth->GetArguments().size(); index++) { // If arg is entity struct or array
		const VariableType argType = calculatedGoToMeth->GetArguments().at(index)->GetVariableType();

		if (argType != VariableType::BoolType && argType != VariableType::UByteType && argType != VariableType::DoubleType
			&& argType != VariableType::IntType && argType != VariableType::VoidType && argType != VariableType::StringType) {
			calculatedGoToMeth->GetArguments().at(index) = nullptr;
		}
	}

	delete calculatedGoToMeth;

	return returnVar;
}


bool Interpretator::PerfReservedMethod(GoToMethod* act, Variable** returnVar) {
	bool isPerformed = false;
	
	if (currModuleId == DbControllerModuleIdInSW) {
		isPerformed = PerfReserverDbControllerMethods(act, returnVar);
	}
	else if (currModuleId == IISModuleIdInSW) {
		isPerformed = PerfReservedIISMethods(act, returnVar);
	}
	else if (currModuleId == NetModuleIdInSW) {
		isPerformed = PerfReservedNetMethods(act, returnVar);
	}

	if (!isPerformed) {
		isPerformed = PerfReservedGeneralMethods(act, returnVar);
	}

	return isPerformed;
}

bool Interpretator::PerfReserverDbControllerMethods(GoToMethod* act, Variable** returnVar) {
	bool isPerformed = true; // If it is not basic instruction

	// Выгрузка изменений в файл базы данных
	if (act->GetMethodName() == "UnloadDbChangesToDbFile" && act->GetArguments().size() == 0) {
		try {
#ifdef MULTIUSERS
			std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

			m_dbController->UnloadChangesIntoDBFile();

			*returnVar = new Bool(true);
		}
		catch (const char*) {
			*returnVar = new Bool(false);
		}
	}

	// Действия с вершинами
	//Получение списка свободных вершин
	else if (act->GetMethodName() == "GetListOfFreeVertexIds"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		vector<vertexIdType> freeVertexIds = m_dbController->ReadNode(SWObjectFreeVertexIds)->GetVertexIdsVector();
		Array* returnArr = new Array(new ULong(), freeVertexIds.size());

		for (size_t index = 0; index < freeVertexIds.size(); index++) {
			static_cast<ULong*>(returnArr->GetAt(index))->SetULong(freeVertexIds.at(index));
		}

		*returnVar = returnArr;
	}

	// Получение Id последней записанной вершины
	else if (act->GetMethodName() == "GetLastWroteVertexId" && act->GetArguments().size() == 0) {
		*returnVar = new ULong(m_dbController->GetLastWroteVertexId());
	}

	// Получение Id последнего записанного узла
	else if (act->GetMethodName() == "GetLastWroteNodeId" && act->GetArguments().size() == 0) {
		*returnVar = new ULong(m_dbController->GetLastWroteNodeId());
	}

	// Создание вершины
	else if (act->GetMethodName() == "CreateVertexWithData"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&ULong(), & String_t()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* intVar = static_cast<ULong*>(act->GetArguments()[0]);
		String_t* stringVar = static_cast<String_t*>(act->GetArguments()[1]);

		m_dbController->OperationVertexInBuffer(&Vertex(intVar->GetULong(), stringVar->GetString()), OperationParametrs::Adding);
	}

	// Получение текста вершины по Id
	else if (act->GetMethodName() == "GetStringFromVertexWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&ULong()},
			act->GetArguments())) {
		ULong* intVar = static_cast<ULong*>(act->GetArguments().at(0));
		Vertex* foundVertex = nullptr;
		String_t* result = new String_t();

		{
#ifdef MULTIUSERS
			std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

			foundVertex = m_dbController->ReadVertex((intVar)->GetULong());
		}

		if (foundVertex == nullptr) {
			result->SetString("Vertex with "
				+ std::to_string(intVar->GetULong())
				+ " not exists in db");
		}
		else {
			result->SetString(foundVertex->GetText());
		}

		*returnVar = result;
	}

	// Установка текста в вешину с Id
	else if (act->GetMethodName() == "SetStringToVertexWithId"
		&& CheckEqualArgumentsParametrs(
			vector<Variable*> {&ULong(), & String_t()},
			act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* varInt = static_cast<ULong*>(act->GetArguments().at(0));
		String_t* varString = static_cast<String_t*>(act->GetArguments().at(1));

		m_dbController->OperationVertexInBuffer(&Vertex(varInt->GetULong(), varString->GetString()),
			OperationParametrs::Changing);
	}

	// Удаление вершины по Id
	else if (act->GetMethodName() == "DeleteVertexWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS
		ULong* vertexId = static_cast<ULong*>(act->GetArguments()[0]);

		m_dbController->OperationVertexInBuffer(&Vertex(vertexId->GetULong()), OperationParametrs::Deleting);
	}

	// Действия с узлами
	// Получение массива идентификационных номеров существующих узлов
	else if (act->GetMethodName() == "GetExistsNodeIds" && act->GetArguments().size() == 0) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS
		vector<vertexIdType> nodeIds = m_dbController->ReadNode(SWObjectNodeId)->GetVertexIdsVector();
		Array* returnArr = new Array(new ULong, nodeIds.size());

		for (size_t index = 0; index < nodeIds.size(); index++) {
			static_cast<ULong*>(returnArr->GetAt(index))->SetULong(nodeIds.at(index));
		}

		*returnVar = returnArr;
	}

	// Создание узла с Id
	else if (act->GetMethodName() == "CreateNodeWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS
		ULong* nodeId = static_cast<ULong*>(act->GetArguments()[0]);

		m_dbController->OperationNodeInBuffer(&Node(nodeId->GetULong()), OperationParametrs::Adding);
	}

	// Удаление узла по Id
	else if (act->GetMethodName() == "DeleteNodeWithId"
		&& CheckEqualArgumentsParametrs(vector < Variable* >  {&ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* nodeId = static_cast<ULong*>(act->GetArguments()[0]);

		m_dbController->OperationNodeInBuffer(&Node(nodeId->GetULong()), OperationParametrs::Deleting);
	}

	//Действия с подузлами узлов
	// Создать подузел в узле по Id
	else if (act->GetMethodName() == "CreateSubNodeWithDataInNodeWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&ULong(), & Struct("", vector<Variable*> {&ULong(), & ULong(), & ULong(), & UByte()})}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* nodeId = static_cast<ULong*>(act->GetArgAt(0));
		Struct* subNodeStruct = static_cast<Struct*>(act->GetArgAt(1));

		ULong* subNodeId = static_cast<ULong*>(subNodeStruct->GetAt(0));
		ULong* firstVertexId = static_cast<ULong*>(subNodeStruct->GetAt(1));
		ULong* secondVertexId = static_cast<ULong*>(subNodeStruct->GetAt(2));
		UByte* own = static_cast<UByte*>(subNodeStruct->GetAt(3));

		m_dbController->OperationSubNodesInBuffer(nodeId->GetULong(), vector<SubNode> {
			SubNode(subNodeId->GetULong(), firstVertexId->GetULong(), secondVertexId->GetULong(), own->GetUByte())
		}, OperationParametrs::Adding);
	}

	// Получить массив Id, существующих подузлов узла
	else if (act->GetMethodName() == "GetSubNodeIdsOfNodeWithId"
		&& CheckEqualArgumentsParametrs(vector < Variable* >  {&ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* nodeId = static_cast<ULong*>(act->GetArguments()[0]);
		Array* retArray = new Array(new ULong());

		vector<SubNode>* subNodes = m_dbController->ReadNode(nodeId->GetULong())->GetSubNodes();

		retArray->DefineArray(subNodes->size());

		for (size_t index = 0; index < subNodes->size(); index++) {
			static_cast<ULong*>(retArray->GetAt(index))->SetULong(subNodes->at(index).GetId());
		}

		*returnVar = retArray;
	}

	// Получить подузел по Id узла с Id
	else if (act->GetMethodName() == "GetSubNodeWithIdFromNodeWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&ULong(), & ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* nodeId = static_cast<ULong*>(act->GetArguments()[0]);
		ULong* subNodeId = static_cast<ULong*>(act->GetArguments()[1]);

		SubNode subNode = m_dbController->ReadNode(nodeId->GetULong())->GetSubNode(subNodeId->GetULong());

		*returnVar = new Struct(std::string(""), vector<Variable*>{
			new ULong(subNode.GetId()), new ULong(subNode.GetFirstVertexId()),
				new ULong(subNode.GetSecondVertexId()), new UByte(subNode.GetOwn())
		});
	}

	// Получить подузлы узла с Id
	else if (act->GetMethodName() == "GetSubNodesOfNodeWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* nodeId = static_cast<ULong*>(act->GetArgAt(0));

		vector<SubNode>* subNodes = m_dbController->ReadNode(nodeId->GetULong())->GetSubNodes();
		Array* retArr = new Array(new Struct("", vector<Variable*>{new ULong(), new ULong(), new ULong(), new UByte()}), subNodes->size());

		for (size_t index = 0; index < subNodes->size(); index++) {
			Struct* arrElemStruct = static_cast<Struct*>(static_cast<Struct*>(retArr->GetAt(index)));
			SubNode subNode = subNodes->at(index);

			static_cast<ULong*>(arrElemStruct->GetAt(0))->SetULong(subNode.GetId());
			static_cast<ULong*>(arrElemStruct->GetAt(1))->SetULong(subNode.GetFirstVertexId());
			static_cast<ULong*>(arrElemStruct->GetAt(2))->SetULong(subNode.GetSecondVertexId());
			static_cast<UByte*>(arrElemStruct->GetAt(3))->SetUByte(subNode.GetOwn());
		}

		*returnVar = retArr;
	}

	else if (act->GetMethodName() == "FindByVertexIdAnyWhereInNodeWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&ULong(), & ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* nodeId = static_cast<ULong*>(act->GetArgAt(0));
		ULong* vertexId = static_cast<ULong*>(act->GetArgAt(1));

		vector<SubNode> foundSubNodes = m_dbController->ReadNode(nodeId->GetULong())->FindByVertexIdAnyWhere(vertexId->GetULong());
		Array* retArr = new Array(GetFromSubNode(SubNode()), foundSubNodes.size());

		for (size_t index = 0; index < foundSubNodes.size(); index++) {
			retArr->SetAt(index, GetFromSubNode(foundSubNodes.at(index)));
		}

		*returnVar = retArr;
	}

	else if (act->GetMethodName() == "FindByFirstVertexIdInNodeWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&ULong(), & ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* nodeId = static_cast<ULong*>(act->GetArgAt(0));
		ULong* firstVertexId = static_cast<ULong*>(act->GetArgAt(1));

		vector<SubNode> foundSubNodes = m_dbController->ReadNode(nodeId->GetULong())->FindByFirstVertexId(firstVertexId->GetULong());
		Array* retArr = new Array(GetFromSubNode(SubNode()), foundSubNodes.size());

		for (size_t index = 0; index < foundSubNodes.size(); index++) {
			retArr->SetAt(index, GetFromSubNode(foundSubNodes.at(index)));
		}

		*returnVar = retArr;
	}

	else if (act->GetMethodName() == "FindBySecondVertexIdInNodeWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&ULong(), & ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* nodeId = static_cast<ULong*>(act->GetArgAt(0));
		ULong* secondVertexId = static_cast<ULong*>(act->GetArgAt(1));

		vector<SubNode> foundSubNodes = m_dbController->ReadNode(nodeId->GetULong())->FindBySecondVertexId(secondVertexId->GetULong());
		Array* retArr = new Array(GetFromSubNode(SubNode()), foundSubNodes.size());

		for (size_t index = 0; index < foundSubNodes.size(); index++) {
			retArr->SetAt(index, GetFromSubNode(foundSubNodes.at(index)));
		}

		*returnVar = retArr;
	}

	else if (act->GetMethodName() == "FindByBothVertexIdsInNodeWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&ULong(), & ULong(), & ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* nodeId = static_cast<ULong*>(act->GetArgAt(0));
		ULong* firstVertexId = static_cast<ULong*>(act->GetArgAt(1));
		ULong* secondVertexId = static_cast<ULong*>(act->GetArgAt(2));

		vector<SubNode> foundSubNodes = m_dbController->ReadNode(nodeId->GetULong())->FindByBothVertexIds(firstVertexId->GetULong(), secondVertexId->GetULong());
		Array* retArr = new Array(GetFromSubNode(SubNode()), foundSubNodes.size());

		for (size_t index = 0; index < foundSubNodes.size(); index++) {
			retArr->SetAt(index, GetFromSubNode(foundSubNodes.at(index)));
		}

		*returnVar = retArr;
	}

	// Установка значения подузла с Id в узле с Id
	else if (act->GetMethodName() == "SetSubNodeInNodeWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&ULong(), & Struct("", vector<Variable*> {&ULong(), & ULong(), & ULong(), & UByte()})}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* nodeId = static_cast<ULong*>(act->GetArgAt(0));
		Struct* subNodeStruct = static_cast<Struct*>(act->GetArgAt(1));

		SubNode subNode(static_cast<ULong*>(subNodeStruct->GetAt(0))->GetULong(),
			static_cast<ULong*>(subNodeStruct->GetAt(1))->GetULong(),
			static_cast<ULong*>(subNodeStruct->GetAt(2))->GetULong(),
			static_cast<UByte*>(subNodeStruct->GetAt(3))->GetUByte());

		m_dbController->OperationSubNodesInBuffer(nodeId->GetULong(), vector<SubNode> {
			subNode
		}, OperationParametrs::Changing);
	}

	// Удалить подузел с Id узла с Id
	else if (act->GetMethodName() == "DeleteSubNodeWithIdInNodeWithId"
		&& CheckEqualArgumentsParametrs(vector < Variable* >  {&ULong(), & ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		ULong* nodeId = static_cast<ULong*>(act->GetArguments()[0]);
		ULong* subNodeId = static_cast<ULong*>(act->GetArguments()[1]);

		m_dbController->ReadNode(nodeId->GetULong())->RemoveSubNode(subNodeId->GetULong());
	}
	else {
		isPerformed = false;
	}

	return isPerformed;
}

bool Interpretator::PerfReservedIISMethods(GoToMethod* act, Variable** returnVar) {
	if (act->GetMethodName() == "RestartInterpretator" && act->GetArguments().size() == 0) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		// TODO: tt
	}
	else if (act->GetMethodName() == "UsingModuleWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&ULong()}, act->GetArguments())) {
		ULong* moduleId = static_cast<ULong*>(act->GetArgAt(0));

		{
			RecursionParser parser(m_moduleTables, m_dbController);

			vector<vertexIdType> parsedModuleTableIds = parser.MakeRecourseReparsing(moduleId->GetULong());

			for (vertexIdType parsedModuleTableId : parsedModuleTableIds) {
				DefineModuleContainerVariables(GetModuleTableByItsId(parsedModuleTableId));
			}
		}

		GetParentModuleTable(container)->AddNeedModule(moduleId->GetULong());
	}
	else if (act->GetMethodName() == "DisuseModuleWithId"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS
		ULong* moduleId = static_cast<ULong*>(act->GetArgAt(0));

		// TODO: Define unusing module tables and delete them

		GetParentModuleTable(container)->RemoveNeedModule(moduleId->GetULong());
	}
	else if (act->GetMethodName() == "PerformSyntaxAnalizyModule"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&ULong()}, act->GetArguments())) {
#ifdef MULTIUSERS
		std::lock_guard<std::mutex> lg(globalMutex);
#endif // MULTIUSERS

		// TODO: tt
	}
	else {
		return false;
	}

	return true;
}

bool Interpretator::PerfReservedNetMethods(GoToMethod* act, Variable** returnVar) {
	if (act->GetMethodName() == "IsOpenConnectionWithClient"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {},
			act->GetArguments())) {
		*returnVar = new Bool(m_acceptedClient->IsOpen());
	}
	else if (act->GetMethodName() == "DisconectConnectionWithClient"
		&& act->GetArguments().size() == 0) {
		m_acceptedClient->DisconnectClient();
	}
	else if (act->GetMethodName() == "SendStringToClient"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&String_t()},
			act->GetArguments())) {
		m_acceptedClient->SendMsg(static_cast<String_t*>(act->GetArguments()[0])->GetString());
	}
	else if (act->GetMethodName() == "WaitGetStringFromClient"
		&& act->GetArguments().size() == 0) {
		*returnVar = new String_t(m_acceptedClient->ReceiveMsg());
	}
	else {
		return false;
	}

	return true;
}

bool Interpretator::PerfReservedGeneralMethods(GoToMethod* act, Variable** returnVar) {
	// For all store variables
	if (act->GetMethodName() == "Copy"
		&& act->GetArguments().size() == 1) {
		*returnVar = act->GetArgAt(0)->Clone();
	}

	// Convert methods
	else if (act->GetMethodName() == "IntToUInt"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Int()}, act->GetArguments())) {
		Int* intVar = static_cast<Int*>(act->GetArguments()[0]);

		*returnVar = new UInt(intVar->GetInt());
	}
	else if (act->GetMethodName() == "IntToLong"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Int()}, act->GetArguments())) {
		Int* intVar = static_cast<Int*>(act->GetArguments()[0]);

		*returnVar = new Long(intVar->GetInt());
	}
	else if (act->GetMethodName() == "IntToULong"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Int()}, act->GetArguments())) {
		Int* intVar = static_cast<Int*>(act->GetArguments()[0]);

		*returnVar = new ULong(intVar->GetInt());
	}
	else if (act->GetMethodName() == "IntToString"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Int()}, act->GetArguments())) {
		Int* intVar = static_cast<Int*>(act->GetArguments()[0]);

		*returnVar = new String_t(std::to_string(intVar->GetInt()));
	}

	else if (act->GetMethodName() == "ULongToInt"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&ULong()}, act->GetArguments())) {
		ULong* intVar = static_cast<ULong*>(act->GetArguments()[0]);

		*returnVar = new Int((int)intVar->GetULong());
	}
	else if (act->GetMethodName() == "ULongToUInt"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&ULong()}, act->GetArguments())) {
		ULong* intVar = static_cast<ULong*>(act->GetArguments()[0]);

		*returnVar = new UInt((int)intVar->GetULong());
	}
	else if (act->GetMethodName() == "ULongToString"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&ULong()}, act->GetArguments())) {
		ULong* vId_t = static_cast<ULong*>(act->GetArguments()[0]);

		*returnVar = new String_t(std::to_string(vId_t->GetULong()));
	}

	else if (act->GetMethodName() == "DoubleToInt"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Double()}, act->GetArguments())) {
		Double* doubleVar = static_cast<Double*>(act->GetArguments()[0]);

		*returnVar = new Int((int32_t)doubleVar->GetDouble());
	}
	else if (act->GetMethodName() == "DoubleToUInt"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Double()}, act->GetArguments())) {
		Double* doubleVar = static_cast<Double*>(act->GetArguments()[0]);

		*returnVar = new UInt((uint32_t)doubleVar->GetDouble());
	}
	else if (act->GetMethodName() == "DoubleToLong"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Double()}, act->GetArguments())) {
		Double* doubleVar = static_cast<Double*>(act->GetArguments()[0]);

		*returnVar = new Long((int64_t)doubleVar->GetDouble());
	}
	else if (act->GetMethodName() == "DoubleToULong"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Double()}, act->GetArguments())) {
		Double* doubleVar = static_cast<Double*>(act->GetArguments()[0]);

		*returnVar = new ULong((uint64_t)doubleVar->GetDouble());
	}
	else if(act->GetMethodName() == "DoubleToString"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Double()}, act->GetArguments())) {
		Double* doubleVar = static_cast<Double*>(act->GetArguments()[0]);

		*returnVar = new String_t(std::to_string(doubleVar->GetDouble()));
	}

	else if (act->GetMethodName() == "StringToInt"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&String_t()}, act->GetArguments())) {
		String_t* stringVar = static_cast<String_t*>(act->GetArguments()[0]);

		*returnVar = new Int(stringVar->GetString() != "" ? stoi(stringVar->GetString()) : 0.0);
	}
	else if (act->GetMethodName() == "StringToUInt"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&String_t()}, act->GetArguments())) {
		String_t* stringVar = static_cast<String_t*>(act->GetArguments()[0]);

		*returnVar = new UInt(stringVar->GetString() != "" ? stoul(stringVar->GetString()) : 0.0);
	}
	else if (act->GetMethodName() == "StringToLong"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&String_t()}, act->GetArguments())) {
		String_t* stringVar = static_cast<String_t*>(act->GetArguments()[0]);

		*returnVar = new Long(stringVar->GetString() != "" ? stoll(stringVar->GetString()) : 0.0);
	}
	else if (act->GetMethodName() == "StringToULong"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&String_t()}, act->GetArguments())) {
		String_t* stringVar = static_cast<String_t*>(act->GetArguments()[0]);

		*returnVar = new ULong(stringVar->GetString() != "" ? stoull(stringVar->GetString()) : 0.0);
	}
	else if (act->GetMethodName() == "StringToDouble"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&String_t()}, act->GetArguments())) {
		String_t* stringVar = static_cast<String_t*>(act->GetArgAt(0));

		*returnVar = new Double(stringVar->GetString() != "" ? stod(stringVar->GetString()) : 0.0);
	}

	// Method for modify string
	else if (act->GetMethodName() == "Substring"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&String_t(), & ULong(), & ULong()}, act->GetArguments())) {
		String_t* stringVar = static_cast<String_t*>(act->GetArgAt(0));
		ULong* startIndex = static_cast<ULong*>(act->GetArgAt(1));
		ULong* subCount = static_cast<ULong*>(act->GetArgAt(2));
		
		string subStr = stringVar->GetString().substr(
			startIndex->GetULong(), subCount->GetULong()
		);

		*returnVar = new String_t(subStr);
	}
	else if (act->GetMethodName() == "Substring"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&String_t(), & ULong()}, act->GetArguments())) {
		String_t* stringVar = static_cast<String_t*>(act->GetArgAt(0));
		ULong* startIndex = static_cast<ULong*>(act->GetArgAt(1));

		string subStr = stringVar->GetString().substr(
			startIndex->GetULong()
		);

		*returnVar = new String_t(subStr);
	}

	// Tree methods
	else if (act->GetMethodName() == "GetStringJSONFormatFromTree"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree()}, act->GetArguments())) {
		string ret = static_cast<Tree*>(act->GetArguments()[0])->GetStringJSONFormat();

		*returnVar = new String_t(ret);
	}
	else if (act->GetMethodName() == "GetTreeFromStringJSONFormat"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&String_t()}, act->GetArguments())) {
		Tree* tree = new Tree();

		tree->SetTreeFromJSONStringFormat(static_cast<String_t*>(act->GetArgAt(0))->GetString());

		*returnVar = tree;
	}

	else if (act->GetMethodName() == "GetChildByKeyFromTree"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree(), &String_t()}, act->GetArguments())) {
		Tree* tree = static_cast<Tree*>(act->GetArgAt(0));
		String_t* key = static_cast<String_t*>(act->GetArgAt(1));

		*returnVar = new Tree(tree->GetChild(key->GetString()));
	}
	else if (act->GetMethodName() == "AddChildWithKeyToTree"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree(), & String_t(), & Tree()}, act->GetArguments())) {
		Tree* tree = static_cast<Tree*>(act->GetArgAt(0));
		String_t* key = static_cast<String_t*>(act->GetArgAt(1));
		Tree* value = static_cast<Tree*>(act->GetArgAt(2));

		tree->AddChild(key->GetString(), value->GetPtree());
	}

	else if (act->GetMethodName() == "GetBoolByKeyFromTree"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree(), & String_t()}, act->GetArguments())) {
		Tree* tree = static_cast<Tree*>(act->GetArgAt(0));
		String_t* key = static_cast<String_t*>(act->GetArgAt(1));

		*returnVar = new Bool(tree->GetVar<bool>(key->GetString()));
	}
	else if (act->GetMethodName() == "AddBoolWithKeyToTree"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree(), & String_t(), & Bool()}, act->GetArguments())) {
		Tree* tree = static_cast<Tree*>(act->GetArgAt(0));
		String_t* key = static_cast<String_t*>(act->GetArgAt(1));
		Bool* value = static_cast<Bool*>(act->GetArgAt(2));

		tree->PutVar<uint64_t>(key->GetString(), value->GetBool());
	}

	else if (act->GetMethodName() == "GetULongByKeyFromTree"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree(), & String_t()}, act->GetArguments())) {
		Tree* tree = static_cast<Tree*>(act->GetArgAt(0));
		String_t* key = static_cast<String_t*>(act->GetArgAt(1));

		*returnVar = new ULong(tree->GetVar<uint64_t>(key->GetString()));
	}
	else if (act->GetMethodName() == "AddULongWithKeyToTree"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree(), & String_t(), &ULong()}, act->GetArguments())) {
		Tree* tree = static_cast<Tree*>(act->GetArgAt(0));
		String_t* key = static_cast<String_t*>(act->GetArgAt(1));
		ULong* value = static_cast<ULong*>(act->GetArgAt(2));

		tree->PutVar<uint64_t>(key->GetString(), value->GetULong());
	}

	else if (act->GetMethodName() == "GetStringByKeyFromTree"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree(), & String_t()}, act->GetArguments())) {
		Tree* tree = static_cast<Tree*>(act->GetArgAt(0));
		String_t* key = static_cast<String_t*>(act->GetArgAt(1));

		string retStr = utf2cp(tree->GetVar<string>(key->GetString()));

		*returnVar = new String_t(retStr);
	}
	else if (act->GetMethodName() == "AddStringWithKeyToTree"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree(), & String_t(), & String_t()}, act->GetArguments())) {
		Tree* tree = static_cast<Tree*>(act->GetArgAt(0));
		String_t* key = static_cast<String_t*>(act->GetArgAt(1));
		String_t* value = static_cast<String_t*>(act->GetArgAt(2));

		tree->PutVar<string>(key->GetString(), value->GetString());
	}

	else if (act->GetMethodName() == "GetTreeAsIntArray"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree()}, act->GetArguments())) {
		ptree tree = static_cast<Tree*>(act->GetArgAt(0))->GetPtree();
		vector<int32_t> vertexArr;

		for (auto& item : tree) {
			vertexArr.push_back(item.second.get_value<int32_t>());
		}

		Array* retArr = new Array(new Int(), vertexArr.size());

		for (size_t index = 0; index < vertexArr.size(); index++) {
			retArr->SetAt(index, new Int(vertexArr.at(index)));
		}

		*returnVar = retArr;
	}
	else if (act->GetMethodName() == "AddIntToTreeArray"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&Tree(), & Int()}, act->GetArguments())) {
		Tree* tree = static_cast<Tree*>(act->GetArgAt(0));
		Int* var = static_cast<Int*>(act->GetArgAt(1));

		tree->PushBackVar(var->GetInt());
	}
	else if (act->GetMethodName() == "GetTreeAsULongArray"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree()}, act->GetArguments())) {
		ptree tree = static_cast<Tree*>(act->GetArgAt(0))->GetPtree();
		vector<uint64_t> vertexArr;

		for (auto& item : tree) {
			vertexArr.push_back(item.second.get_value<uint64_t>());
		}

		Array* retArr = new Array(new ULong(), vertexArr.size());

		for (size_t index = 0; index < vertexArr.size(); index++) {
			retArr->SetAt(index, new ULong(vertexArr.at(index)));
		}

		*returnVar = retArr;
	}
	else if(act->GetMethodName() == "AddULongToTreeArray"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&Tree(), &ULong()}, act->GetArguments())) {
		Tree* tree = static_cast<Tree*>(act->GetArgAt(0));
		ULong* var = static_cast<ULong*>(act->GetArgAt(1));

		tree->PushBackVar(var->GetULong());
	}
	else if (act->GetMethodName() == "AddDoubleToTreeArray"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&Tree(), & Double()}, act->GetArguments())) {
		Tree* tree = static_cast<Tree*>(act->GetArgAt(0));
		Double* var = static_cast<Double*>(act->GetArgAt(1));

		tree->PushBackVar(var->GetDouble());
	}
	else if (act->GetMethodName() == "GetTreeAsDoubleArray"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree()}, act->GetArguments())) {
		ptree tree = static_cast<Tree*>(act->GetArgAt(0))->GetPtree();
		vector<double> vertexArr;

		for (auto& item : tree) {
			vertexArr.push_back(item.second.get_value<double>());
		}

		Array* retArr = new Array(new Double(), vertexArr.size());

		for (size_t index = 0; index < vertexArr.size(); index++) {
			retArr->SetAt(index, new Double(vertexArr.at(index)));
		}

		*returnVar = retArr;
	}
	else if (act->GetMethodName() == "AddStringToTreeArray"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&Tree(), & String_t()}, act->GetArguments())) {
		static_cast<Tree*>(act->GetArgAt(0))->PushBackVar(static_cast<String_t*>(act->GetArgAt(1))->GetString());
	}
	else if (act->GetMethodName() == "GetTreeAsStringArray"
		&& CheckEqualArgumentsParametrs(vector<Variable*> {&Tree()}, act->GetArguments())) {
		ptree tree = static_cast<Tree*>(act->GetArgAt(0))->GetPtree();
		vector<string> vertexArr;

		for (auto& item : tree) {
			string second = utf2cp(item.second.get_value<string>());

			vertexArr.push_back(second);
		}

		Array* retArr = new Array(new String_t(), vertexArr.size());

		for (size_t index = 0; index < vertexArr.size(); index++) {
			retArr->SetAt(index, new String_t(vertexArr.at(index)));
		}

		*returnVar = retArr;
	}

	// Array methods
	else if (act->GetMethodName() == "len"
		&& act->GetArgAt(0)->GetVariableType() == VariableType::ArrayType) {
		Array* arr = static_cast<Array*>(act->GetArgAt(0));

		*returnVar = new ULong(arr->GetSize());
	}
	else if (act->GetMethodName() == "CopyArrayFromTo"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{ &Array(), & ULong(), & ULong(), & Array(), & ULong() }, act->GetArguments())) {
		Array* from = static_cast<Array*>(act->GetArgAt(0));
		ULong* frStartIndex = static_cast<ULong*>(act->GetArgAt(1));
		ULong* frCopyCount = static_cast<ULong*>(act->GetArgAt(2));
		Array* to = static_cast<Array*>(act->GetArgAt(3));
		ULong* toIndex = static_cast<ULong*>(act->GetArgAt(4));

		for (size_t index = 0; index < frCopyCount->GetULong(); index++) {
			to->SetAt(index, from->GetCopyAt(index+frStartIndex->GetULong()));
		}
	}
	else if (act->GetMethodName() == "ResizeArray"
		&& CheckEqualArgumentsParametrs(vector<Variable*>{&Array(), & ULong()}, act->GetArguments())) {
		Array* arrForResize = static_cast<Array*>(act->GetArgAt(0));
		ULong* newSize = static_cast<ULong*>(act->GetArgAt(1));

		arrForResize->ResizeArray(newSize->GetULong());
	}

	else {
		return false;
	}

	return true;
}


Variable* Interpretator::PerfCastAction(CastAction* act, vector<Variable*>& methodLocalVars) {
	Variable* rValue = CalculateVariable(act->GetFromCast(), methodLocalVars);

	return CastFromTo(rValue, act->GetToCast());
}

void Interpretator::AssertFromTo(Variable* to, Variable* from) {
	if (to->GetVariableType() != VariableType::AnyType && !to->IsEqualVarStructures(from)) {
		throw "lValue and rValue have different data type";
	}

	switch (to->GetVariableType()) {
	case VariableType::AnyType:
		static_cast<Any*>(to)->DeleteAnyVar();
		static_cast<Any*>(to)->SetAnyVar(from->Clone());
		break;
	case VariableType::BoolType:
		static_cast<Bool*>(to)->SetBool(static_cast<Bool*>(from)->GetBool());
		break;
	case VariableType::ByteType:
		static_cast<Byte*>(to)->SetByte(static_cast<Byte*>(from)->GetByte());
		break;
	case VariableType::UByteType:
		static_cast<UByte*>(to)->SetUByte(static_cast<UByte*>(from)->GetUByte());
		break;
	case VariableType::IntType:
		static_cast<Int*>(to)->SetInt(static_cast<Int*>(from)->GetInt());
		break;
	case VariableType::UIntType:
		static_cast<UInt*>(to)->SetUInt(static_cast<UInt*>(from)->GetUInt());
		break;
	case VariableType::LongType:
		static_cast<Long*>(to)->SetLong(static_cast<Long*>(from)->GetLong());
		break;
	case VariableType::ULongType:
		static_cast<ULong*>(to)->SetULong(static_cast<ULong*>(from)->GetULong());
		break;
	case VariableType::DoubleType:
		static_cast<Double*>(to)->SetDouble(static_cast<Double*>(from)->GetDouble());
		break;
	case VariableType::StringType:
		static_cast<String_t*>(to)->SetString(static_cast<String_t*>(from)->GetString());
		break;
	case VariableType::ArrayType:
	{
		Array* fromArr = static_cast<Array*>(from);

		static_cast<Array*>(to)->SetArray(fromArr->GetArray(), fromArr->GetBasicVar());
	}
		break;
	case VariableType::TreeType:
		static_cast<Tree*>(to)->SetPtree(static_cast<Tree*>(from)->GetPtree());
		break;
	case VariableType::StructType:
	{
		Struct* toStruct = static_cast<Struct*>(to);
		Struct* fromStruct = static_cast<Struct*>(from);

		if (!(
				(fromStruct->GetStructName() == "" || fromStruct->GetStructName() == toStruct->GetStructName())
				/* && CheckEqualArgumentsParametrs(toStruct->GetVariables(), fromStruct->GetVariables())*/
			)) {
			throw "Cannot convert struct " + fromStruct->GetStructName() + " to struct " + toStruct->GetStructName();
		}

		toStruct->SetStructCopyFrom(fromStruct);
	}
	break;
	default:
		throw "Not programmed into interpretator assert to that variable type";
		break;
	}
}

bool Interpretator::CheckEqualArgumentsParametrs(const vector<Variable*>& parametrs, const vector<Variable*>& arguments) const {
	if (parametrs.size() != arguments.size()) {
		return false;
	}

	for (size_t index = 0; index < parametrs.size(); index++) {
		VariableType paramType = parametrs[index]->GetVariableType();
		VariableType argType = arguments[index]->GetVariableType();

		if (paramType != VariableType::AnyType) {
			if (!parametrs[index]->IsEqualVarStructures(arguments[index])) {
				return false;
			}
		}
	}

	return true;
}

Variable* Interpretator::GetDefinedVariable(LinkToVariable* specVar, vector<Variable*>& methodLocalVars) {
	const VariableType specVarType = specVar->GetVariableType();
	Variable* varLink = nullptr;

	if (specVarType == VariableType::MethodVariableLinkType) {
		varLink = GetMethodVariableLink(methodLocalVars, static_cast<MethodVariable*>(specVar)->GetVariableIndex());
	}
	else if (specVarType == VariableType::NamedObjectType) {
		// Get variable name
		NamedObject* containerVariable = static_cast<NamedObject*>(specVar);

		if (container->GetContainerTableType() == ContainerTableType::StructTableType) {
			Struct* structCurrentContainer = static_cast<Struct*>(GetFromStack());
			
			StructTable* currentStructTable = static_cast<StructTable*>(container);

			size_t index = currentStructTable->GetGlobalVariableIndex(containerVariable->GetVariableName());

			if (index < currentStructTable->GetGlobalVariablesCount()) {
				varLink = structCurrentContainer->GetAt(index);
			}

			PushToStack(structCurrentContainer);
		}

		if (varLink == nullptr) {
			varLink = FindNamedVariableByAdditions(containerVariable->GetVariableName(), container);

			if (varLink != nullptr) {
				varLink = static_cast<ContainerVariable*>(varLink)->GetVariable();
			}
		}

		if (varLink == nullptr) {
			throw "Not found global variable with name " + containerVariable->GetVariableName();
		}
	}
	else if (specVarType == VariableType::VariablePathType) {
		VariablePath* path = (VariablePath*)specVar;
		Variable* pathElement = nullptr;
		ContainerTable* containerOfVarLink = container;

		for (size_t index = 0; index < path->GetVarPathSize(); index++) {
			pathElement = path->GetVarPathAt(index);
			VariableType pathElementType = pathElement->GetVariableType();

			if (pathElementType == VariableType::NamedObjectType) {
				NamedObject* containerVariable = static_cast<NamedObject*>(pathElement);
				bool found = false;

				// Search struct table variable and get index for get variable from struct of struct table
				if (containerOfVarLink->GetContainerTableType() == ContainerTableType::StructTableType) {
					bool fromStruct = varLink == nullptr;

					if (fromStruct) {
						varLink = static_cast<Struct*>(GetFromStack());
					}

					size_t index = containerOfVarLink->GetGlobalVariableIndex(containerVariable->GetVariableName());

					if (index < containerOfVarLink->GetGlobalVariablesCount()) {
						found = true;

						varLink = static_cast<Struct*>(varLink)->GetAt(index);
					}

					if (fromStruct) {
						PushToStack(varLink);
					}
				}

				if (!found) {
					varLink = FindNamedVariableByAdditions(containerVariable->GetVariableName(), containerOfVarLink);

					if (varLink != nullptr) {
						varLink = static_cast<ContainerVariable*>(varLink)->GetVariable();
					}
				}

				if (varLink == nullptr) {
					throw "Not found global variable or struct variable with name " + containerVariable->GetVariableName();
				}
			}
			else if (pathElementType == VariableType::ArrayIndexType) {
				Variable* calculatedIndexVar = CalculateVariable(static_cast<ArrayIndex*>(pathElement)->GetIndex(), methodLocalVars);
				Array* arrayVar = static_cast<Array*>(varLink);

				if (calculatedIndexVar->GetVariableType() == VariableType::UByteType) {
					varLink = arrayVar->GetAt(static_cast<UByte*>(calculatedIndexVar)->GetUByte());
				}
				else if (calculatedIndexVar->GetVariableType() == VariableType::IntType) {
					varLink = arrayVar->GetAt(static_cast<Int*>(calculatedIndexVar)->GetInt());
				}
				else if (calculatedIndexVar->GetVariableType() == VariableType::ULongType) {
					varLink = arrayVar->GetAt(static_cast<ULong*>(calculatedIndexVar)->GetULong());
				}
				else {
					throw "Not indexical type for array";
				}

				delete calculatedIndexVar;
			}
			else if (pathElementType == VariableType::GoToMethodType) {
				if (containerOfVarLink->GetContainerTableType() == ContainerTableType::StructTableType) {
					PushToStack(varLink);
				}

				varLink = PerfGoToMethod(static_cast<GoToMethod*>(pathElement), methodLocalVars, containerOfVarLink);

				if (containerOfVarLink->GetContainerTableType() == ContainerTableType::StructTableType) {
					GetFromStack();
				}

				if (varLink == nullptr && index < path->GetVarPathSize() - 1) {
					throw static_cast<GoToMethod*>(pathElement)->GetMethodName() + " is not return  variable";
				}
			}
			else if (pathElementType == VariableType::MethodVariableLinkType) { // Can be perform in first time
				varLink = GetMethodVariableLink(methodLocalVars, static_cast<MethodVariable*>(pathElement)->GetVariableIndex());
			}
			else {
				throw "Variable path cannot contains that path element type";
			}

			if (varLink != nullptr && varLink->GetVariableType() == VariableType::StructType && static_cast<Struct*>(varLink)->GetStructName() != "") {
				containerOfVarLink = FindStructTableByAdditions(static_cast<Struct*>(varLink)->GetStructName(), containerOfVarLink);
			}
		}
	}
	else {
		throw "Undefined defined variable type";
	}

	return varLink;
}

ModuleTable* Interpretator::GetModuleTableByItsId(const vertexIdType moduleId) const {
	ModuleTable* fModuleTable = nullptr;

	for (auto moduleTable : m_moduleTables) {
		if (moduleTable->GetModuleId() == moduleId) {
			fModuleTable = moduleTable;

			break;
		}
	}

	return fModuleTable;
}


StructTable* Interpretator::FindStructTableByAdditions(const string& structName, ContainerTable* containerFromSearch) {
	ContainerTable* containerForSearch = containerFromSearch;

	if (container->GetContainerTableType() == ContainerTableType::StructTableType) {
		while (containerForSearch->GetContainerTableType() != ContainerTableType::ModuleTableType) {
			StructTable* structTable = (StructTable*)containerForSearch;

			ContainerTable* foundStructTable = structTable->GetSubContainerTable(structName);

			if (foundStructTable == nullptr) {
				containerForSearch = structTable->GetParent();
			}
			else {
				return static_cast<StructTable*>(foundStructTable);
			}
		}
	}

	vector<vertexIdType> forSearch = {static_cast<ModuleTable*>(containerForSearch)->GetModuleId()};

	return static_cast<StructTable*>(
		FindTByModules<ContainerTable*>(static_cast<ModuleTable*>(containerFromSearch)->GetModuleId(),
			[&structName](ModuleTable* modTab) {
				return modTab->GetSubContainerTable(structName);
			})
		);
}

MethodTable* Interpretator::FindMethodByAdditions(GoToMethod* goToMethod, ContainerTable* containerFromSearch) {
	ContainerTable* containerForSearch = containerFromSearch;

	if (containerFromSearch->GetContainerTableType() == ContainerTableType::StructTableType) {
		while (containerForSearch->GetContainerTableType() != ContainerTableType::ModuleTableType) {
			MethodTable* contMethodTable = FindMethodInContainerTable(goToMethod, containerForSearch);

			if (contMethodTable != nullptr) {
				return contMethodTable;
			}
			
			containerForSearch = static_cast<StructTable*>(containerForSearch)->GetParent();
		}
	}
	
	return FindTByModules<MethodTable*>(static_cast<ModuleTable*>(containerForSearch)->GetModuleId(),
		[&goToMethod](ModuleTable* modTab) {
			return modTab->GetMethodTable(goToMethod->GetMethodName(), goToMethod->GetArguments());
		});
}


MethodTable* Interpretator::FindMethodInContainerTable(GoToMethod* goToMethod, ContainerTable* containerForSearch) {
	return containerForSearch->GetMethodTable(goToMethod->GetMethodName(), goToMethod->GetArguments());
}


ContainerVariable* Interpretator::FindNamedVariableByAdditions(const string& globVarName, ContainerTable* containerFromSearch) {
	if(container->GetContainerTableType() == ContainerTableType::StructTableType) {
		ContainerVariable* foundGlNamVar = container->GetGlobalVariable(globVarName);

		if (foundGlNamVar != nullptr) {
			return foundGlNamVar;
		}
	}

	return FindTByModules<ContainerVariable*>(GetParentModuleTable(containerFromSearch)->GetModuleId(), [&globVarName](ModuleTable* modTab) {
		return modTab->GetGlobalVariable(globVarName);
		});
}

Variable* Interpretator::GetMethodVariableLink(vector<Variable*>& methodVars, size_t variableIndex) {
	return methodVars.at(methodVars.size() - variableIndex - 1);
}

void Interpretator::PerfDefineAction(DefineAction* act, vector<Variable*>& localVars) {
	const VariableType setToType = act->GetSetToType()->GetVariableType();
	Variable* var = CreateVariableForDefine(act->GetVariableLink(), container, localVars);

	switch (setToType) {
	case VariableType::MethodVariableLinkType:
		localVars.push_back(var);
		break;
	case VariableType::StackElementType:
		PushToStack(var);
		break;
	default:
		throw "Error";
		break;
	}
}

void Interpretator::PerfAssertAction(AssertAction* act, vector<Variable*>& localVars) {
	LinkToVariable* lSpValue = act->GetLValue();
	Variable* rValue = CalculateVariable(act->GetRightValue(), localVars);

	if (lSpValue->GetVariableType() == VariableType::MethodVariableLinkType) {
		Variable* lValue = GetMethodVariableLink(localVars, static_cast<MethodVariable*>(lSpValue)->GetVariableIndex());

		rValue = CalculateVariable(rValue, localVars);

		if (!lValue->IsEqualVarStructures(rValue)) {
			throw "lValue and rValue is different data type";
		}

		AssertFromTo(lValue, rValue);
	}
	else if (lSpValue->GetVariableType() == VariableType::VariablePathType) {
		Variable* defVar = GetDefinedVariable(lSpValue, localVars);

		AssertFromTo(defVar, CastFromTo(rValue, defVar));
	}
	else if (lSpValue->GetVariableType() == VariableType::NamedObjectType) {
		NamedObject* moduleVar = static_cast<NamedObject*>(lSpValue);
		Variable* globVar = GetDefinedVariable(moduleVar, localVars);

		if (globVar == nullptr) {
			throw "Undefined global variable with name " + moduleVar->GetVariableName();
		}

		CastAction* castAct = new CastAction(CalculateVariable(rValue, localVars), globVar);
		Variable* calcVar = PerfCastAction(castAct, localVars);

		AssertFromTo(globVar, calcVar);
	}
	else {
		throw "Cannot assert into lValue type";
	}
}

void Interpretator::PerfRemoveSpecificVariable(RemoveSpecificVariable* act, vector<Variable*>& localVars) {
	const size_t countToRemove = act->GetCountToRemove();
	const VariableType typeToRemove = act->GetSpecificTypeToRemove();

	switch (typeToRemove) {
	case VariableType::MethodVariableLinkType:
		// Remove from method local variables
		for (size_t number = 1; number <= countToRemove; number++) {
			if (localVars.back() != nullptr) {
				delete localVars.back();
			}
			localVars.pop_back();
		}

		break;
	case VariableType::StackElementType:
		// Remove from stack
		for (size_t number = 1; number <= countToRemove; number++) {
			delete GetFromStack();
		}

		break;
	default:
		throw "Such specific type not exists for remove";
		break;
	}
}

void Interpretator::PerfSkipRemoveMethLocalCariable(vector<Variable*>& localVars) noexcept {
	localVars.pop_back();
}


Variable* Interpretator::CreateVariableForDefine(Variable* var, ContainerTable* containerForSearch, vector<Variable*> methodLocalVars) {
	switch (var->GetVariableType()) {
	case VariableType::NamedObjectType: // Struct from modules
	{
		StructTable* structTable = FindStructTableByAdditions(static_cast<NamedObject*>(var)->GetVariableName(), container);
		Struct* returnStruct = new Struct(structTable->GetContainerName());

		for (size_t index = 0; index < structTable->GetGlobalVariablesCount(); index++) {
			returnStruct->AddVarToBack(CreateVariableForDefine(structTable->GetGlobalVariable(index), structTable, methodLocalVars));
		}

		return returnStruct;
	}
		break;
	case VariableType::ArrayType:
	{
		return new Array(CreateVariableForDefine(CreateVariableForDefine(static_cast<Array*>(var)->GetBasicVar(), containerForSearch, methodLocalVars), containerForSearch, methodLocalVars), static_cast<Array*>(var)->GetArraySizeVar());
	}
		break;
	case VariableType::NamedVariableType:
		return CalculateVariable(static_cast<ContainerVariable*>(var)->GetVariable(), methodLocalVars);
		break;
	case VariableType::CastActionType:
	case VariableType::StackElementType:
		return CalculateVariable(var, methodLocalVars);
		break;
	case VariableType::VariablePathType: // For array with specific size
	{
		Variable* arrayType = CreateVariableForDefine(static_cast<VariablePath*>(var)->GetVarPathAt(0), containerForSearch, methodLocalVars);
		Variable* arraySizeVar = CalculateVariable(static_cast<ArrayIndex*>(static_cast<VariablePath*>(var)->GetVarPathAt(1))->GetIndex(), methodLocalVars);

		return new Array(arrayType, static_cast<ULong*>(arraySizeVar)->GetULong());
	}
		break;
	default: // Simple store variable or stack
		return CalculateVariable(var, methodLocalVars)->Clone();
		break;
	}
}

Variable* Interpretator::CastFromTo(Variable* from, Variable* to) const {
	Variable* lValue = nullptr;

	switch (to->GetVariableType()) {
	case VariableType::AnyType:
		if (from->GetVariableType() == VariableType::AnyType) {
			lValue = static_cast<Any*>(from);
		}
		else {
			lValue = new Any(from);
		}
		break;
	case VariableType::BoolType:
		switch (from->GetVariableType()) {
		case VariableType::BoolType:
			lValue = from;
			break;
		case VariableType::ByteType:
			lValue = new Bool(static_cast<Byte*>(from)->GetByte());
			break;
		case VariableType::UByteType:
			lValue = new Bool(static_cast<UByte*>(from)->GetUByte());
			break;
		case VariableType::IntType:
			lValue = new Bool(static_cast<Int*>(from)->GetInt());
			break;
		case VariableType::UIntType:
			lValue = new Bool(static_cast<UInt*>(from)->GetUInt());
			break;
		case VariableType::LongType:
			lValue = new Bool(static_cast<Long*>(from)->GetLong());
			break;
		case VariableType::ULongType:
			lValue = new Bool(static_cast<ULong*>(from)->GetULong());
			break;
		case VariableType::DoubleType:
			lValue = new Bool(static_cast<Double*>(from)->GetDouble());
			break;
		case VariableType::StringType:
			lValue = new Bool(static_cast<String_t*>(from)->GetString() != "");
			break;
		default:
			throw "Cannot cast from that type to bool";
			break;
		}
		break;
	case VariableType::ByteType:
		switch (from->GetVariableType()) {
		case VariableType::ByteType:
			lValue = from;
			break;
		case VariableType::UByteType:
			lValue = new Byte((int8_t)static_cast<UByte*>(from)->GetUByte());
			break;
		case VariableType::IntType:
			lValue = new Byte((int8_t)static_cast<Int*>(from)->GetInt());
			break;
		case VariableType::UIntType:
			lValue = new Byte((int8_t)static_cast<UInt*>(from)->GetUInt());
			break;
		case VariableType::LongType:
			lValue = new Byte((int8_t)static_cast<Long*>(from)->GetLong());
			break;
		case VariableType::ULongType:
			lValue = new Byte((int8_t)static_cast<ULong*>(from)->GetULong());
			break;
		case VariableType::DoubleType:
			lValue = new Byte((int8_t)static_cast<Double*>(from)->GetDouble());
			break;
		default:
			throw "Cannot cast from that type to byte";
			break;
		}
		break;
	case VariableType::UByteType:
		switch (from->GetVariableType()) {
		case VariableType::ByteType:
			lValue = new UByte((uint8_t)static_cast<Byte*>(from)->GetByte());
			break;
		case VariableType::UByteType:
			lValue = from;
			break;
		case VariableType::IntType:
			lValue = new UByte((uint8_t)static_cast<Int*>(from)->GetInt());
			break;
		case VariableType::UIntType:
			lValue = new UByte((uint8_t)static_cast<UInt*>(from)->GetUInt());
			break;
		case VariableType::LongType:
			lValue = new UByte((uint8_t)static_cast<Long*>(from)->GetLong());
			break;
		case VariableType::ULongType:
			lValue = new UByte((uint8_t)static_cast<ULong*>(from)->GetULong());
			break;
		case VariableType::DoubleType:
			lValue = new UByte((uint8_t)static_cast<Double*>(from)->GetDouble());
			break;
		default:
			throw "Cannot cast from that type to ubyte";
			break;
		}
		break;
	case VariableType::IntType:
		switch (from->GetVariableType()) {
		case VariableType::ByteType:
			lValue = new Int((int32_t)static_cast<Byte*>(from)->GetByte());
			break;
		case VariableType::UByteType:
			lValue = new Int((int32_t)static_cast<UByte*>(from)->GetUByte());
			break;
		case VariableType::IntType:
			lValue = from;
			break;
		case VariableType::UIntType:
			lValue = new Int((int32_t)static_cast<UInt*>(from)->GetUInt());
			break;
		case VariableType::LongType:
			lValue = new Int((int32_t)static_cast<Long*>(from)->GetLong());
			break;
		case VariableType::ULongType:
			lValue = new Int((int32_t)static_cast<ULong*>(from)->GetULong());
			break;
		case VariableType::DoubleType:
			lValue = new Int((int32_t)static_cast<Double*>(from)->GetDouble());
			break;
		default:
			throw "Cannot cast from that type to int";
			break;
		}
		break;
	case VariableType::UIntType:
		switch (from->GetVariableType()) {
		case VariableType::ByteType:
			lValue = new UInt((uint32_t)static_cast<Byte*>(from)->GetByte());
			break;
		case VariableType::UByteType:
			lValue = new UInt((uint32_t)static_cast<UByte*>(from)->GetUByte());
			break;
		case VariableType::IntType:
			lValue = new UInt((uint32_t)static_cast<Int*>(from)->GetInt());
			break;
		case VariableType::UIntType:
			lValue = from;
			break;
		case VariableType::LongType:
			lValue = new UInt((uint32_t)static_cast<Long*>(from)->GetLong());
			break;
		case VariableType::ULongType:
			lValue = new UInt((uint32_t)static_cast<ULong*>(from)->GetULong());
			break;
		case VariableType::DoubleType:
			lValue = new UInt((uint32_t)static_cast<Double*>(from)->GetDouble());
			break;
		default:
			throw "Cannot cast from that type to uint";
			break;
		}
		break;
	case VariableType::LongType:
		switch (from->GetVariableType()) {
		case VariableType::ByteType:
			lValue = new Long((int64_t)static_cast<Byte*>(from)->GetByte());
			break;
		case VariableType::UByteType:
			lValue = new Long((int64_t)static_cast<UByte*>(from)->GetUByte());
			break;
		case VariableType::IntType:
			lValue = new Long((int64_t)static_cast<Int*>(from)->GetInt());
			break;
		case VariableType::UIntType:
			lValue = new Long((int64_t)static_cast<UInt*>(from)->GetUInt());
			break;
		case VariableType::LongType:
			lValue = from;
			break;
		case VariableType::ULongType:
			lValue = new Long((int64_t)static_cast<ULong*>(from)->GetULong());
			break;
		case VariableType::DoubleType:
			lValue = new Long((int64_t)static_cast<Double*>(from)->GetDouble());
			break;
		default:
			throw "Cannot cast from that type to vertexId_t";
			break;
		}
		break;
	case VariableType::ULongType:
		switch (from->GetVariableType()) {
		case VariableType::ByteType:
			lValue = new ULong((uint64_t)static_cast<Byte*>(from)->GetByte());
			break;
		case VariableType::UByteType:
			lValue = new ULong((uint64_t)static_cast<UByte*>(from)->GetUByte());
			break;
		case VariableType::IntType:
			lValue = new ULong((uint64_t)static_cast<Int*>(from)->GetInt());
			break;
		case VariableType::UIntType:
			lValue = new ULong((uint64_t)static_cast<UInt*>(from)->GetUInt());
			break;
		case VariableType::LongType:
			lValue = new ULong((uint64_t)static_cast<Long*>(from)->GetLong());
			break;
		case VariableType::ULongType:
			lValue = from;
			break;
		case VariableType::DoubleType:
			lValue = new ULong((uint64_t)static_cast<Double*>(from)->GetDouble());
			break;
		default:
			throw "Cannot cast from that type to vertexId_t";
			break;
		}
		break;
	case VariableType::DoubleType:
		switch (from->GetVariableType()) {
		case VariableType::ByteType:
			lValue = new Double((double)static_cast<Byte*>(from)->GetByte());
			break;
		case VariableType::UByteType:
			lValue = new Double((double)static_cast<UByte*>(from)->GetUByte());
			break;
		case VariableType::IntType:
			lValue = new Double((double)static_cast<Int*>(from)->GetInt());
			break;
		case VariableType::UIntType:
			lValue = new Double((double)static_cast<UInt*>(from)->GetUInt());
			break;
		case VariableType::LongType:
			lValue = new Double((double)static_cast<Long*>(from)->GetLong());
			break;
		case VariableType::ULongType:
			lValue = new Double((double)static_cast<ULong*>(from)->GetULong());
			break;
		case VariableType::DoubleType:
			lValue = from;
			break;
		default:
			throw "Cannot cast from that type to vertexId_t";
			break;
		}
		break;
	case VariableType::StringType:
		switch (from->GetVariableType()) {
		case VariableType::UByteType:
			lValue = new String_t(std::to_string(static_cast<UByte*>(from)->GetUByte()));
			break;
		case VariableType::StringType:
			lValue = static_cast<String_t*>(from);
			break;
		default:
			throw "Cannot cast from that type to string";
			break;
		}
		break;
	case VariableType::ArrayTemplateType:
		switch (from->GetVariableType()) {
		case VariableType::ArrayType:
		{
			ArrayTemplate* toArTemp = static_cast<ArrayTemplate*>(to);
			Array* fromArr = static_cast<Array*>(from);

			if (!toArTemp->GetObjectTemplate()->IsEqualVarStructures(fromArr->GetBasicVar())) {
				throw "Cannot convert array to array (template) that have different basic var type";
			}

			if (toArTemp->GetArraySizeVar() != 0 && toArTemp->GetArraySizeVar() < fromArr->GetArraySizeVar()) {
				throw "Cannot fit array with size " + std::to_string(fromArr->GetArraySizeVar()) + " to array template with size " + std::to_string(toArTemp->GetArraySizeVar());
			}

			if (fromArr->GetBasicVar()->GetVariableType() == VariableType::StructType) {
				string arrStructName = static_cast<NamedObject*>(toArTemp->GetObjectTemplate())->GetVariableName();

				static_cast<Struct*>(fromArr->GetBasicVar())->SetStructName(arrStructName);

				for (size_t index = 0; index < fromArr->GetArraySizeVar(); index++) {
					static_cast<Struct*>(fromArr->GetAt(index))->SetStructName(arrStructName);
				}
			}

			lValue = fromArr;
		}
			break;
		default:
			throw "Cannot cast from that to array (template)";
			break;
		}
		break;
	case VariableType::ArrayType:
		switch (from->GetVariableType()) {
		case VariableType::ArrayType:
			if (to->IsEqualVarStructures(from)) {
				lValue = static_cast<Array*>(from);
			}
			else {
				lValue = new Array();
				static_cast<Array*>(lValue)->SetArrayCopyFrom(static_cast<Array*>(from));
			}
			break;
		default:
			throw "Cannot cast from that type to array";
			break;
		}
		break;
	case VariableType::StructType:
		switch (from->GetVariableType()) {
		case VariableType::ArrayType: {
			if (!CheckEqualArgumentsParametrs(static_cast<Array*>(from)->GetArray(), static_cast<Struct*>(to)->GetVariables())) {
				throw "Cannot convert array to struct. Their structures is different";
			}

			lValue = new Struct(static_cast<Struct*>(to)->GetStructName(), static_cast<Array*>(from)->GetArray());
		}
		break;
		case VariableType::StructType: {
			Struct* toCastStruct = static_cast<Struct*>(to);
			Struct* fromStruct = static_cast<Struct*>(from);

			if (fromStruct->GetStructName() != ""
				&& fromStruct->GetStructName() != toCastStruct->GetStructName()/*!CheckEqualArgumentsParametrs(toCastStruct->GetVariables(), fromStruct->GetVariables())*/) {
				throw "Cannot convert struct " + fromStruct->GetStructName() + " to struct " + toCastStruct->GetStructName();
			}
			else if (fromStruct->GetStructName() == "") {
				fromStruct->SetStructName(toCastStruct->GetStructName());
			}

			lValue = static_cast<Struct*>(from);
		}
		break;
		default:
			throw "Cannot cast from that type to struct";
			break;
		}
		break;
	case VariableType::NamedObjectType:
		switch (from->GetVariableType()) {
		case VariableType::StructType:
		{
			NamedObject* toNameObj = static_cast<NamedObject*>(to);
			Struct* fromStruct = static_cast<Struct*>(from);

			if (fromStruct->GetStructName() == "") {
				fromStruct->SetStructName(toNameObj->GetVariableName());
			}

			if (toNameObj->GetVariableName() != fromStruct->GetStructName()) {
				throw "Cannot cast named object " + toNameObj->GetVariableName() + " with struct with name " + fromStruct->GetStructName();
			}

			lValue = static_cast<Struct*>(from);
		}
		break;
		default:
			throw "Cannot cast from named object with name " + static_cast<NamedObject*>(to)->GetVariableName() + " with that type";
			break;
		}
		break;
	case VariableType::TreeType:
		switch (from->GetVariableType()) {
		case VariableType::TreeType:
			lValue = static_cast<Tree*>(from);
			break;
		default:
			throw "Cannot cast from that to tree";
			break;
		}
		break;
	default:
		throw "Undefined cast from first variable to second";
		break;
	}

	return lValue;
}


Struct* Interpretator::GetFromSubNode(const SubNode subNode) {
	return new Struct("", vector<Variable*>{
		new ULong(subNode.GetId()),
			new ULong(subNode.GetFirstVertexId()),
			new ULong(subNode.GetSecondVertexId()),
			new UByte(subNode.GetOwn())
	});
}

Variable* Interpretator::GetFromStack() noexcept {
	Variable* var = nullptr;

	var = m_stack.top();
	m_stack.pop();

	return var;
}

void Interpretator::PushToStack(Variable* var) {
	m_stack.push(var);
}
