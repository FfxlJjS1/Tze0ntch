#pragma once
#include <cstdint>

#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class UInt final : public ElementaryVariable {
			private:
				uint32_t my_unsigned_int32 = 0;

			public:
				UInt() noexcept : ElementaryVariable(VariableType::UIntType) {};

				UInt(uint32_t unsigned_int32) noexcept : UInt() {
					my_unsigned_int32 = unsigned_int32;
				}

				uint32_t GetUInt() noexcept {
					return my_unsigned_int32;
				}

				void SetUInt(uint32_t unsigned_int32) noexcept {
					my_unsigned_int32 = unsigned_int32;
				}

				Variable* clone() const override {
					return new UInt(my_unsigned_int32);
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
