#pragma once
#include "CommonTypeDef.h"

enum class VariableType : char {
	Null,
	
	CreateComplexVarType,
	
	CastActionType,

	// MathActionTypes
	PlusActionType,
	MinesActionType,
	TimesActionType,
	DivideActionType,
	RemainderActionType,


	// LogicalAction
	NotLogicalActionType,
	AndLogicalActionType,
	OrLogicalActionType,


	// ComparibleAction
	ComparibleEqualType,
	ComparibleNEqualType,
	ComparibleLessType,
	ComparibleGreaterType,
	ComparibleLessEqualType,
	ComparibleGreaterEqualType,


	/// Variables
	/// Simple types
	VoidType,
	AnyType,
	BoolType,
	ByteType,
	UByteType,
	IntType,
	UIntType,
	LongType,
	ULongType,
	DoubleType,

	/// Complex types
	NullObjectType,
	ArrayType,
	ArrayTemplateType,
	StringType,
	StructType,
	TreeType,

	/// Specific types
	StackElementType,
	ArrayIndexType,
	NamedVariableType,
	MethodVariableLinkType, // For method variable without struct
	NamedObjectType, // For global struct or global variable undefined in local method
	VariablePathType, // For struct or array path
	GoToMethodType
};

class Variable {
protected:
	VariableType m_variableType = VariableType::Null;

public:
	Variable(VariableType varType) noexcept : m_variableType(varType) {};

	VariableType GetVariableType() noexcept {
		return m_variableType;
	}

	virtual Variable* Clone() const = 0;

	virtual bool IsEqualVarStructures(Variable* stVar) const = 0;
};
