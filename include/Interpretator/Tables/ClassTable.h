#pragma once
#include <string>

#include "ContainerTable.h"
#include "ContainerElementTable.h"

namespace Interpretator {
	namespace Tables {
		using std::string;

		class ClassTable final : public ContainerTable, public ContainerElementTable {
		public:
			ClassTable(string struct_name) : ContainerTable(struct_name, ContainerTableType::ClassTableType) {};

			ClassTable(string struct_name, vector<ContainerVariable*>& struct_fields)
				: ClassTable(struct_name) {
				m_globalVariables = struct_fields;
			};
		};
	}
}
