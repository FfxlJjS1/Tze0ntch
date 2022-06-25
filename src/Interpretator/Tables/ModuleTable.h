#pragma once
#include <vector>

#include "ContainerTable.h"

#include "CommonTypeDef.h"

using std::vector;

class ModuleTable final : public ContainerTable {
private:
	vertexIdType m_moduleId = 0;
	vector<vertexIdType> m_needModules; // TODO: Change to links to modules

public:
	ModuleTable(vertexIdType moduleId, string moduleName) : ContainerTable(moduleName, ContainerTableType::ModuleTableType){
		m_moduleId = moduleId;
	};

	vertexIdType GetModuleId() const {
		return m_moduleId;
	}

	vector<vertexIdType> GetNeedModules() const {
		return m_needModules;
	}

	void AddNeedModule(vertexIdType moduleId) {
		if (find(m_needModules.begin(), m_needModules.end(), moduleId) == m_needModules.end()) {
			m_needModules.push_back(moduleId);
		}
	}

	void RemoveNeedModule(vertexIdType moduleId) {
		auto iterator = find(m_needModules.begin(), m_needModules.end(), moduleId);

		if (iterator != m_needModules.end()) {
			m_needModules.erase(iterator);
		}
	}

	bool operator==(const vertexIdType& moduleId) const {
		return m_moduleId == moduleId;
	}
};