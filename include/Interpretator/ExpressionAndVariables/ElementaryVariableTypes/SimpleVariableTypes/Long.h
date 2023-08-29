#pragma once
#include <cstdint>

#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class Long final : public ElementaryVariable {
			private:
				int64_t my_signed_long64 = 0;

			public:
				Long() : ElementaryVariable(VariableType::LongType) {}

				Long(int64_t signed_long64) : Long() {
					my_signed_long64 = signed_long64;
				}

				int64_t get_long() {
					return my_signed_long64;
				}

				void set_long(int64_t signed_long64) {
					my_signed_long64 = signed_long64;
				}

				Variable* clone() const override {
					return new Long(my_signed_long64);
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
