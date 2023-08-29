#pragma once
#include <vector>

#include "../LinkToVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			using std::vector;

			class VariablePath final : public LinkToVariable {
			protected:
				// Can be varLink and ArrayIndex (array), string (subfield struct), GoToMethod (method)
				vector<Variable*> my_var_path;

			public:
				VariablePath() : LinkToVariable(VariableType::VariablePathType) {}

				void add_var_path(Variable* varPath) {
					my_var_path.push_back(varPath);
				}

				Variable* get_var_path_at(const size_t index) {
					return my_var_path.at(index);
				}

				Variable* get_var_path_front() {
					return my_var_path.front();
				}

				Variable* get_var_path_back() {
					return my_var_path.back();
				}

				size_t get_var_path_size() const {
					return my_var_path.size();
				}

				VariablePath* clone() const {
					VariablePath* clone = new VariablePath();

					for (Variable* var : my_var_path) {
						clone->add_var_path(var->clone());
					}

					return clone;
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}

				~VariablePath() {
					for (Variable* var : my_var_path) {
						delete var;
					}
				}
			};
		}
	}
}
