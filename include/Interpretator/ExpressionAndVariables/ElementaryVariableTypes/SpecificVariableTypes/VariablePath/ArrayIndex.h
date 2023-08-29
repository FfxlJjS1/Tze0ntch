#pragma once
#include "../LinkToVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class ArrayIndex final : public LinkToVariable {
			private:
				Variable* my_index_var = nullptr;

			public:
				ArrayIndex() noexcept : LinkToVariable(VariableType::ArrayIndexType) {};

				ArrayIndex(Variable* index_var) : ArrayIndex() {
					my_index_var = index_var;
				}

				Variable* get_index() noexcept {
					return my_index_var;
				}

				void set_index(Variable* index_var) noexcept {
					my_index_var = index_var;
				}

				Variable* clone() const override {
					return new ArrayIndex(my_index_var->Clone());
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}

				~ArrayIndex() {
					delete my_index_var;
				}
			};
		}
	}
}
