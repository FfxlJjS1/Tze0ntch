#pragma once
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Variable.h"

#include "EncodingConvertor.h"

using std::string;
using boost::property_tree::ptree;

class Tree : public Variable {
private:
	ptree m_pt;

public:
	Tree() : Variable(VariableType::TreeType) {};

	Tree(string json) : Tree() {
		SetTreeFromJSONStringFormat(json);
	}

	Tree(ptree& pt) : Tree() {
		SetTreeFromJSONStringFormat(GetStringJSONFormatFromPtree(pt));
	}

	ptree GetPtree() {
		return m_pt;
	}

	void SetPtree(ptree& pt) {
		m_pt = pt;
	}

	// Get and set child

	ptree GetChild(const string& key) {
		return m_pt.get_child(key);
	}

	void AddChild(const string& key, ptree pt) {
		m_pt.put_child(key, pt);
	}

	void AddChild(const string& key, Tree& tree) {
		m_pt.put_child(key, tree.GetPtree());
	}

	// Get and set vars (not ptree)

	template<typename T>
	T GetVar(const string& key) const {
		return m_pt.get<T>(key);
	}

	template<typename T>
	T GetOptVar(const string& key) const {
		return m_pt;
	}
	
	template<typename T>
	void PutVar(const string& key, T var) {
		m_pt.put(key, var);
	}

	void PushBackVar(ptree var) {
		m_pt.push_back(std::make_pair("", var));
	}

	template<typename T>
	void PushBackVar(T var) {
		ptree child;

		child.put("", var);

		m_pt.push_back(std::make_pair("", child));
	}

	string GetStringJSONFormat() const {
		std::stringstream strStream;

		boost::property_tree::json_parser::write_json(strStream, m_pt);

		return strStream.str();
	}

	static string GetStringJSONFormatFromPtree(ptree& jsonPt) {
		std::stringstream strStream;

		boost::property_tree::json_parser::write_json(strStream, jsonPt);

		string str = strStream.str();

		str = utf2cp(str);

		return str;
	}

	void SetTreeFromJSONStringFormat(const string& json) {
		string g = cp2utf(json);

		std::stringstream buffer(g.c_str());

		boost::property_tree::json_parser::read_json(buffer, m_pt);
	}

	Variable* Clone() const {
		return new Tree(GetStringJSONFormat());
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};