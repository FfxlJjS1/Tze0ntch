#pragma once
#include "LogicalAction.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ExpressionTypes {
			class LogicalNOT final : public LogicalAction {
			private:
				Variable* my_logical_var = nullptr;

			public:
				LogicalNOT(Variable* logical_var)
					: LogicalAction(VariableType::NotLogicalActionType) {
					my_logical_var = logical_var;
				};

				void set_logical_var(Variable*& log_var) noexcept {
					my_logical_var = log_var;
				}

				Variable* get_logical_var() const noexcept {
					return my_logical_var;
				}

				Variable* clone() const override {
					return new LogicalNOT(my_logical_var->clone());
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type
						&& my_logical_var->is_equal_var_structures(static_cast<LogicalNOT*>(st_var)->get_logical_var());
				}

				~LogicalNOT() {
					delete my_logical_var;
				}
			};
		}
	}
}
