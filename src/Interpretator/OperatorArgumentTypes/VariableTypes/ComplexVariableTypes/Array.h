#pragma once
#include <vector>

#include "Variable.h"

using std::vector;

class Array final : public Variable {
private:
	vector<Variable*> m_array;
	Variable* m_basicVar = nullptr;

public:
	Array() noexcept : Variable(VariableType::ArrayType) {};
	
	Array(Variable* basicVar) noexcept : Array() {
		m_basicVar = basicVar;
	};

	Array(Variable* basicVar, const size_t size) : Array() {
		MakeNewArray(basicVar, size);
	}

	void DefineArray(const size_t size) {
		Variable* basicVar = m_basicVar;
		m_basicVar = nullptr;

		MakeNewArray(basicVar, size);
	}

	void ResizeArray(const size_t newSize) {
		size_t oldCount = m_array.size();

		m_array.resize(newSize);

		for (; oldCount < newSize; oldCount++) {
			m_array.at(oldCount) = static_cast<Variable*>(m_basicVar->Clone());
		}
	}

	void SetArray(vector<Variable*>& varArray, Variable* basicVar) {
		m_array = varArray;
		m_basicVar = basicVar;
	}

	void SetArrayCopyFrom(const Array* fromArray) {
		for (Variable* var : m_array) {
			delete var;
		}

		delete m_basicVar;

		m_basicVar = static_cast<Variable*>(fromArray->GetBasicVar()->Clone());

		for (size_t index = 0; index < fromArray->GetSize(); index++) {
			m_array.push_back(fromArray->GetCopyAt(index));
		}
	}

	Variable* GetBasicVar() const noexcept {
		return m_basicVar;
	}

	size_t GetSize() const noexcept {
		return m_array.size();
	}

	vector<Variable*>& GetArray() noexcept {
		return m_array;
	}

	size_t GetArraySizeVar() const {
		return m_array.size();
	}

	Variable* GetAt(size_t index) {
		return m_array.at(index);
	}

	Variable* GetCopyAt(const size_t index) const {
		return static_cast<Variable*>(m_array.at(index)->Clone());
	}

	void SetAt(size_t index, Variable* var) {
		delete m_array.at(index);

		m_array.at(index) = var;
	}

	Variable* Clone() const override {
		Array* clone = new Array();

		clone->SetArrayCopyFrom(this);

		return clone;
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		if (stVar->GetVariableType() != VariableType::ArrayType) {
			return false;
		}

		Array* varAr = static_cast<Array*>(stVar);

		if (varAr->m_array.size() != 0 && m_array.size() != 0) {
			if (varAr->GetArraySizeVar() != m_array.size()) {
				return false;
			}

			for (size_t index = 0; index < m_array.size(); index++) {
				if (!m_array[index]->IsEqualVarStructures(varAr->GetAt(index))) {
					return false;
				}
			}
		}

		return true;
	}

	~Array() {
		for (Variable* var : m_array) {
			delete var;
		}

		delete m_basicVar;
	}

private:
	void MakeNewArray(Variable* basicVar, const size_t size) {
		for (Variable* var : m_array) {
			delete var;
		}

		delete m_basicVar;

		m_basicVar = basicVar;

		for (size_t index = 0; index < size; index++) {
			m_array.push_back(static_cast<Variable*>(basicVar->Clone()));
		}
	}
};
