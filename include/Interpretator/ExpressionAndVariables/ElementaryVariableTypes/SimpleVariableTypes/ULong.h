#pragma once
#include <cstdint>

#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class ULong final : public ElementaryVariable {
			private:
				uint64_t my_unsigned_long64 = 0;

			public:
				ULong() : ElementaryVariable(VariableType::ULongType) {}

				ULong(uint64_t unsigned_long64) : ULong() {
					my_unsigned_long64 = unsigned_long64;
				}

				uint64_t get_ulong() {
					return my_unsigned_long64;
				}

				void set_ulong(uint64_t unsigned_long64) {
					my_unsigned_long64 = unsigned_long64;
				}

				Variable* clone() const override {
					return new ULong(my_unsigned_long64);
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
