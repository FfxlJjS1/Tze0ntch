#pragma once
#include <string>

#include "ContainerTable.h"
#include "ContainerElementTable.h"

using std::string;

class StructTable final : public ContainerTable, public ContainerElementTable {
public:
	StructTable(string structName) : ContainerTable(structName, ContainerTableType::StructTableType) {};

	StructTable(string structName, vector<ContainerVariable*>& structFields)
		: StructTable(structName) {
		m_globalVariables = structFields;
	};
};