#pragma once
class ContainerTable;

class ContainerElementTable {
protected:
	ContainerTable* m_parent = nullptr;

public:
	ContainerTable*& GetParent() noexcept {
		return m_parent;
	}

	void SetParent(ContainerTable*& parent) noexcept {
		m_parent = parent;
	}
};

#include "ContainerTable.h"
