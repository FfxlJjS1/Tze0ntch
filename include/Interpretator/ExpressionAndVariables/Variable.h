#pragma once

namespace Interpretator {
	namespace ExpressionAndVariables {
		enum class VariableType : unsigned char {
			Null =							0b00000000, // 1-st and 2-nd byte is't true

			// Expression variables
			ExpressionVariableType =		0b10000000, // 1-st is true but 2-nd is false

			/// Logical or comparible actions
			LogicalOrComparibleAction =		0b10100000, // 3-rd is true

			//// ComparibleAction
			ComparibleActionType =			0b10110000, // 4-th is true
			ComparibleEqualType =			0b10110001,
			ComparibleNEqualType =			0b10110010,
			ComparibleLessType =			0b10110011,
			ComparibleGreaterType =			0b10110100,
			ComparibleLessEqualType =		0b10110101,
			ComparibleGreaterEqualType =	0b10110110,
			
			//// LogicalAction
			LogicalActionType =				0b10100000, // 4-th is false
			NotLogicalActionType =			0b10100001,
			AndLogicalActionType =			0b10100010,
			OrLogicalActionType =			0b10100011,


			/// Math or other actions
			MathOrOtherActions =			0b10000000, // 3-rd is false

			//// MathActionTypes
			MathActionType =				0b10010000, // 4-th is true
			PlusActionType =				0b10010001,
			MinesActionType =				0b10010010,
			TimesActionType =				0b10010011,
			DivideActionType =				0b10010100,
			RemainderActionType =			0b10010101,

			//// OtherAction
			OtherActionType =				0b10000000, // 4-th is false
			CreateComplexVarType =			0b10000001,
			CastActionType =				0b10000010,



			// Elementary variables
			ElementaryVariableType =		0b01000000, // 1-st is false but 2-nd is true
			
			/// Simple data types
			SimpleDataType =				0b01100000, // 3-rd is true

			//// Simple types
			SimpleType =					0b01110000, // 4-th is true
			VoidType =						0b01110001,
			AnyType =						0b01110010,
			BoolType =						0b01110011,
			ByteType =						0b01110100,
			UByteType =						0b01110101,
			IntType =						0b01110110,
			UIntType =						0b01110111,
			LongType =						0b01111000,
			ULongType =						0b01111001,
			DoubleType =					0b01111010,

			//// Complex types
			ComplexType =					0b01100000, // 4-th is false
			ArrayType =						0b01100001,
			StringType =					0b01100011,
			StructType =					0b01100100,
			TreeType =						0b01100101,


			/// Specific types
			SpecificDataType =				0b01000000, // 3-rd is false
			NullObjectType =				0b01100001,
			StackElementType =				0b01000010,
			ArrayIndexType =				0b01000011,
			ArrayTemplateType =				0b01100100,
			NamedVariableType =				0b01000101,
			MethodVariableLinkType =		0b01000110, // For method variable without struct
			NamedObjectType =				0b01000111, // For global struct or global variable undefined in local method
			VariablePathType =				0b01001000, // For struct or array path
			GoToMethodType =				0b01001001
		};

		class Variable {
		protected:
			VariableType my_var_type = VariableType::Null;

		public:
			Variable(VariableType var_type) noexcept : my_var_type(var_type) {};

			VariableType get_var_type() noexcept {
				return my_var_type;
			}

			virtual Variable* clone() const = 0;

			virtual bool is_equal_var_structures(Variable* st_var) const = 0;
		};
	}
}
