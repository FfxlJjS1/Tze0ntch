#pragma once
#include <string>

#include "LinkToVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace OperationTypes {
			using std::string;

			class GoToMethod final : public LinkToVariable {
			private:
				string my_method_name = "";
				vector<Variable*> my_arguments;

			public:
				GoToMethod(string method_name)
					: LinkToVariable(VariableType::GoToMethodType) {
					my_method_name = method_name;
				};

				string get_method_name() const {
					return my_method_name;
				}

				vector<Variable*>& get_arguments() noexcept {
					return my_arguments;
				}

				void add_argument(Variable* argument) {
					my_arguments.push_back(argument);
				}

				Variable*& get_arg_at(size_t index) {
					return my_arguments.at(index);
				}

				void set_arguments(vector<Variable*>& arguments) noexcept {
					my_arguments = arguments;
				}

				Variable* clone() const override {
					GoToMethod* clone = new GoToMethod(my_method_name);

					for (Variable* var : my_arguments) {
						clone->add_argument(var->clone());
					}

					return clone;
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}

				~GoToMethod() {
					for (size_t index = 0; index < my_arguments.size(); index++) {
						delete my_arguments.at(index);
					}
				}
			};
		}
	}
}
