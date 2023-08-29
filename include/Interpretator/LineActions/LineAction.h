#pragma once

namespace Interpretator {
	namespace LineActions {
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
			LineActionType my_line_act_type = LineActionType::Null;

		public:
			LineAction(LineActionType line_act_type) noexcept : my_line_act_type(line_act_type) {};

			LineActionType get_line_action_type() noexcept {
				return my_line_act_type;
			}
		};
	}
}
