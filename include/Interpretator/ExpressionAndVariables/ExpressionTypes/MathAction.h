#pragma once
#include "ExpressionAction.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ExpressionTypes {
			class MathAction final : public ExpressionAction {
			private:
				Variable* my_var1 = nullptr;
				Variable* my_var2 = nullptr;

			public:
				MathAction(VariableType math_option_type) noexcept : ExpressionAction(math_option_type) {};

				MathAction(VariableType math_option_type, Variable* var1, Variable* var2) noexcept
					: MathAction(math_option_type) {
					my_var1 = var1;
					my_var2 = var2;
				}

				void set_var1(Variable* var1) noexcept {
					my_var1 = var1;
				}

				Variable*& get_var1() noexcept {
					return my_var1;
				}

				void set_var2(Variable* var2) noexcept {
					my_var2 = var2;
				}

				Variable*& get_var2() noexcept {
					return my_var2;
				}

				Variable* clone() const override {
					return new MathAction(my_var_type, my_var1->clone(), my_var2->clone());
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					if (st_var->get_var_type() != my_var_type) {
						return false;
					}

					MathAction* math_Act = static_cast<MathAction*>(st_var);

					return my_var1->is_equal_var_structures(math_Act->get_var1())
						&& my_var2->is_equal_var_structures(math_Act->get_var2());
				}

				~MathAction() {
					delete my_var1;
					delete my_var2;
				}
			};
		}
	}
}
