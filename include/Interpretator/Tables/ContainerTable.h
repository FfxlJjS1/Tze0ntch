#pragma once
#include <string>

#include "MethodTable.h"

#include "NamedVariable.h"

namespace Interpretator {
	namespace Tables {
		using std::string;

		using ExpressionAndVariables::ElementaryVariableTypes::ContainerVariable;

		enum class ContainerTableType : char {
			Null,
			ModuleTableType,
			ClassTableType
		};

		class ContainerTable {
		protected:
			string my_container_name = "";
			ContainerTableType my_cont_table_type = ContainerTableType::Null;
			vector<ContainerTable*> my_sub_container_tables;
			vector<MethodTable*> my_method_tables;
			vector<ContainerVariable*> my_global_variables;

		public:
			ContainerTable(const string& container_name, const ContainerTableType cont_table_Type) : my_container_name(container_name), my_cont_table_type(cont_table_type) {};

			string get_container_name() const {
				return my_container_name;
			}

			ContainerTableType get_container_table_type() const {
				return my_cont_table_type;
			}

			/*
			  Return link to NamedVariable if contains it with name as arg
			  If not found return nullptr
			*/
			ContainerVariable* get_global_variable(const string& gl_var_name) {
				for (ContainerVariable* gl_var : my_global_variables) {
					if (*gl_var == gl_var_name) {
						return gl_var;
					}
				}

				return nullptr;
			}

			ContainerVariable* get_global_variable(const size_t index) {
				return my_global_variables.at(index);
			}

			/*
			  Return index of global variable with name
			  If not found return index greater than size of global variables vector
			*/
			size_t get_global_variable_index(const string& gl_var_name) const {
				for (size_t index = 0; index < my_global_variables.size(); index++) {
					if (my_global_variables[index]->get_name() == gl_var_name) {
						return index;
					}
				}

				return my_global_variables.size() + 1;
			}

			void add_global_variable(ContainerVariable* gl_var) {
				my_global_variables.push_back(gl_var);
			}

			size_t get_global_variables_count() const {
				return my_global_variables.size();
			}

			/*
			  Return lint to SubContainerTable if contains it with name as arg
			  If not found return nullptr
			*/
			ContainerTable* get_sub_container_table(const string& struct_name) {
				for (ContainerTable* struct_table : my_sub_container_tables) {
					if (struct_table->get_container_name() == struct_name) {
						return structTable;
					}
				}

				return nullptr;
			}

			void add_struct_table(ContainerTable* struct_table) {
				m_subContainerTables.push_back(struct_table);
			}

			size_t get_struct_tables_count() const {
				return my_sub_container_tables.size();
			}

			/*
			  Return lint to MethodTable with name
			  If not found return nullptr
			*/
			MethodTable* get_method_table(const string& method_name) {
				for (MethodTable* method_table : my_method_tables) {
					if (*method_table == method_name) {
						return method_table;
					}
				}

				return nullptr;
			}

			/*
			  Return link to MethodTable with name and parametrs equal arguments
			  If not found return nullptr
			*/
			MethodTable* get_method_table(const string& method_name, const vector<Variable*>& arguments) {
				for (MethodTable* con_tab_method_table : my_method_tables) {
					if (con_tab_method_table->get_method_name() == method_name &&
						con_tab_method_table->is_parametrs_equal_arguments(arguments)) {
						return con_tab_method_table;
					}
				}

				return nullptr;
			}

			size_t get_method_tables_count() const {
				return my_method_tables.size();
			}

			void add_method_table(MethodTable* method_table) {
				my_method_tables.push_back(method_table);
			}


			~ContainerTable() {
				for (ContainerVariable* gl_var : my_global_variables) {
					delete gl_var;
				}

				for (ContainerTable* struct_table : my_sub_container_tables) {
					delete structTable;
				}

				for (MethodTable* method_table : my_method_tables) {
					delete method_table;
				}
			}
		};
	}
}
