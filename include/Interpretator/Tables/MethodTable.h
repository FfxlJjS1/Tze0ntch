#pragma once
#include <vector>
#include <string>

#include "ContainerElementTable.h"

#include "LineAction.h"

#include "Variable.h"

namespace Interpretator {
	namespace Tables {
		using std::vector, std::string;

		using ExpressionAndVariables::Variable, ExpressionAndVariables::VariableType;
		using LineActions::LineAction;

		class MethodTable final : public ContainerElementTable {
		private:
			string my_method_name = "";
			Variable* my_return_variable_type = nullptr;
			vector <Variable*> my_parametrs;
			vector <LineAction*> my_line_actions;

		public:
			MethodTable(const string& method_name) : my_method_name(method_name) {};

			MethodTable(const string& method_name,
				Variable* return_variable_type,
				vector<Variable*> parametrs)
				: m_methodName(method_name),
				my_return_variable_type(return_variable_type),
				my_parametrs(parametrs) {};

			string get_method_name() const {
				return my_method_name;
			}

			void set_return_variable_type(Variable*& return_var_type) {
				my_return_variable_type = return_var_type;
			}

			Variable*& get_return_variable_type() {
				return my_return_variable_type;
			}

			void add_parametr(Variable*& parametr_type) {
				my_parametrs.push_back(parametr_type);
			}

			vector<Variable*>& get_parametrs() {
				return my_parametrs;
			}

			size_t get_parametrs_count() const {
				return m_parametrs.size();
			}

			bool is_parametrs_equal_arguments(const vector<Variable*>& arguments) {
				if (my_parametrs.size() != arguments.size()) {
					return false;
				}

				for (size_t index = 0; index < my_parametrs.size(); index++) {
					VariableType param_type = my_parametrs[index]->get_variable_type();

					if (param_type != VariableType::AnyType) {
						if (!my_parametrs[index]->is_equal_var_structures(arguments[index])) {
							return false;
						}
					}
				}

				return true;
			}

			vector<LineAction*>& get_line_action_vector() {
				return my_line_actions;
			}

			vector<LineAction*>* get_line_actions_link() {
				return &my_line_actions;
			}

			void add_line_action(LineAction*& line_action) {
				my_line_actions.push_back(line_action);
			}

			void remove_last_line_actions(int count_to_remove) {
				for (size_t index = my_line_actions.size() - 1;
					index >= my_line_actions.size() - count_to_remove - 1;
					index--) {
					delete my_line_actions[index];
					my_line_actions.pop_back();
				}
			}


			~MethodTable() {
				delete my_return_variable_type;

				for (Variable* parametr : my_parametrs) {
					delete parametr;
				}

				for (LineAction* line_act : my_line_actions) {
					delete line_act;
				}
			}
		};
	}
}
