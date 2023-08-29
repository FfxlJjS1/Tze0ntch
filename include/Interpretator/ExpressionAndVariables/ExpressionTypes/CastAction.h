#pragma once
#include "ExpressionAction.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ExpressionTypes {
			class CastAction : public ExpressionAction {
			private:
				Variable* my_from_cast = nullptr;
				Variable* my_to_cast = nullptr;

			public:
				CastAction(Variable* from_cast, Variable* to_cast)
					: ExpressionAction(VariableType::CastActionType) {
					my_from_cast = from_cast;
					my_to_cast = to_cast;
				}

				Variable* get_from_cast() {
					return my_from_cast;
				}

				Variable* get_to_cast() {
					return my_to_cast;
				}

				CastAction* clone() const override {
					return new CastAction(my_from_cast->clone(), my_to_cast->clone());
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					if (st_var->get_var_type() != my_var_type) {
						return false;
					}

					CastAction* cast_act = static_cast<CastAction*>(st_var);

					return my_from_cast->is_equal_var_structures(cast_act->get_from_cast())
						&& my_to_cast->is_equal_var_structures(cast_act->get_to_cast());
				}

				~CastAction() {
					delete my_from_cast;
					delete my_to_cast;
				}
			};
		}
	}
}
