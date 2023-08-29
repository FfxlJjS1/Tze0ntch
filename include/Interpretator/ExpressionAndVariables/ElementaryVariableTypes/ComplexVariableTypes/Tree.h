#pragma once
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../ElementaryVariable.h"

#include "../../../../../../libruaries/EncodingConvertorActions/EncodingConvertor.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			using std::string;
			using boost::property_tree::ptree;

			class Tree : public ElementaryVariable {
			private:
				ptree my_pt;

			public:
				Tree() : ElementaryVariable(VariableType::TreeType) {};

				Tree(string json) : Tree() {
					SetTreeFromJSONStringFormat(json);
				}

				Tree(ptree& pt) : Tree() {
					SetTreeFromJSONStringFormat(GetStringJSONFormatFromPtree(pt));
				}

				ptree get_ptree() {
					return my_pt;
				}

				void set_ptree(ptree& pt) {
					my_pt = pt;
				}

				// Get and set child

				ptree get_child(const string& key) {
					return my_pt.get_child(key);
				}

				void add_child(const string& key, ptree pt) {
					my_pt.put_child(key, pt);
				}

				void add_child(const string& key, Tree& tree) {
					my_pt.put_child(key, tree.get_ptree());
				}

				// Get and set vars (not ptree)

				template<typename T>
				T get_var(const string& key) const {
					return my_pt.get<T>(key);
				}

				template<typename T>
				T get_opt_var(const string& key) const {
					return my_pt;
				}

				template<typename T>
				void put_var(const string& key, T var) {
					my_pt.put(key, var);
				}

				void push_back_var(ptree var) {
					my_pt.push_back(std::make_pair("", var));
				}

				template<typename T>
				void push_back_var(T var) {
					ptree child;

					child.put("", var);

					my_pt.push_back(std::make_pair("", child));
				}

				string get_string_json_format() const {
					std::stringstream str_stream;

					boost::property_tree::json_parser::write_json(str_stream, my_pt);

					return str_stream.str();
				}

				static string get_string_json_format_from_ptree(ptree& json_pt) {
					std::stringstream str_stream;

					boost::property_tree::json_parser::write_json(str_stream, json_pt);

					string str = str_stream.str();

					str = utf2cp(str);

					return str;
				}

				void set_tree_from_json_string_format(const string& json) {
					string g = cp2utf(json);

					std::stringstream buffer(g.c_str());

					boost::property_tree::json_parser::read_json(buffer, my_pt);
				}

				Variable* clone() const override {
					return new Tree(get_string_json_format());
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
