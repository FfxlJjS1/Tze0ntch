#pragma once
#include "LogicalAction.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ExpressionTypes {
			class LogicalTwoArgs final : public LogicalAction {
			private:
				Variable* my_first_logical_var = nullptr;
				Variable* my_second_logical_var = nullptr;

			public:
				LogicalTwoArgs(VariableType logical_two_args_type) noexcept
					: LogicalAction(logical_two_args_type) {};

				LogicalTwoArgs(VariableType logical_two_args_type,
					Variable* first_var, Variable* second_var)
					: LogicalTwoArgs(logical_two_args_type) {
					my_first_logical_var = first_var;
					my_second_logical_var = second_var;
				}

				void set_first_logical_var(Variable* first_logical_ar) noexcept {
					my_first_logical_var = first_logical_ar;
				}

				Variable*& get_first_logical_var() noexcept {
					return my_first_logical_var;
				}

				void set_second_logical_var(Variable* second_logical_var) noexcept {
					my_second_logical_var = second_logical_var;
				}

				Variable*& get_second_logical_var() noexcept {
					return my_second_logical_var;
				}

				Variable* clone() const override {
					return new LogicalTwoArgs(my_var_type, my_first_logical_var->clone(), my_second_logical_var->clone());
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					if (st_var->get_var_type() != my_var_type) {
						return false;
					}

					LogicalTwoArgs* logical_two_arg = static_cast<LogicalTwoArgs*>(st_var);

					return my_first_logical_var->is_equal_var_structures(logical_two_arg->get_first_logical_var())
						&& my_second_logical_var->is_equal_var_structures(logical_two_arg->get_second_logical_var());
				}

				~LogicalTwoArgs() {
					delete my_first_logical_var;
					delete my_second_logical_var;
				}
			};
		}
	}
}
