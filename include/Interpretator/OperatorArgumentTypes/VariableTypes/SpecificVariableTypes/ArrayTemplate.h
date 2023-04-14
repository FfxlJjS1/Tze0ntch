#pragma once
#include "Variable.h"

#include "NamedObject.h"

// For define of method method return type
class ArrayTemplate final : public Variable {
private:
	Variable* m_objectTemplate = nullptr;
	size_t m_arraySize = 0; // If it zero array with undefined size

public:
	ArrayTemplate() : Variable(VariableType::ArrayTemplateType) {}

	ArrayTemplate(size_t arraySize) : ArrayTemplate() {
		m_arraySize = arraySize;
	}

	ArrayTemplate(Variable* objectTemplate) : ArrayTemplate() {
		m_objectTemplate = objectTemplate;
	}

	ArrayTemplate(Variable* objectTemplate, size_t arraySize) : ArrayTemplate() {
		m_objectTemplate = objectTemplate;
		m_arraySize = arraySize;
	}

	Variable* GetObjectTemplate() {
		return m_objectTemplate;
	}

	void SetObjectTemplate(Variable* newObjectTemplate) {
		m_objectTemplate = newObjectTemplate;
	}

	size_t GetArraySizeVar() {
		return m_arraySize;
	}

	void SetArraySize(size_t arraySize) {
		m_arraySize = arraySize;
	}

	Variable* Clone() const override {
		return new ArrayTemplate(m_objectTemplate->Clone(), m_arraySize);
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		if (stVar->GetVariableType() != VariableType::ArrayType) {
			return false;
		}

		Array* stArr = static_cast<Array*>(stVar);

		// Check data type equal
		if (m_objectTemplate->GetVariableType() == VariableType::NamedObjectType
			&& static_cast<NamedObject*>(m_objectTemplate)->GetVariableName() != static_cast<Struct*>(stArr->GetBasicVar())->GetStructName()) {
			return false;
		}
		else if (!m_objectTemplate->IsEqualVarStructures(stArr->GetBasicVar())) {
			return false;
		}

		// Array size
		if(m_arraySize != 0 && stArr->GetArraySizeVar() != m_arraySize) {
			return false;
		}

		return true;
	}

	~ArrayTemplate() {
		delete m_objectTemplate;
	}
};
