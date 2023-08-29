#pragma once
#include "LineAction.h"
#include "../ExpressionAndVariables/Variable.h"

namespace Interpretator {
	namespace LineActions {
		class RemoveSpecificVariable final : public LineAction {
		private:
			VariableType my_remove_the = VariableType::Null; // Contains only specific types
			size_t my_count_to_remove = 0;

		public:
			RemoveSpecificVariable(VariableType remove_the, size_t count_to_remove) : LineAction(LineActionType::RemoveSpecificVariableType) {
				my_remove_the = remove_the;
				my_count_to_remove = count_to_remove;
			}

			VariableType get_specific_type_to_remove() {
				return my_remove_the;
			}

			size_t get_count_to_remove() {
				return my_count_to_remove;
			}
		};
	}
}
