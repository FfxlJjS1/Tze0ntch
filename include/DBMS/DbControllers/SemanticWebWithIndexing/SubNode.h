#pragma once

#include <iostream>

#include "Vertex.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		struct SubNode final {
			vertexIdType my_id = 0;
			mutable vertexIdType my_vertex_id_1 = 0;
			mutable vertexIdType my_vertex_id_2 = 0;

			bool operator==(const SubNode& sub_node) const;

			bool operator==(const vertexIdType& sub_node_id) const;

			bool operator<(const SubNode& sub_node) const;

			friend std::ostream& operator<<(std::ostream& ostream, const SubNode& subNode);

			friend std::istream& operator>>(std::istream& istream, SubNode& subNodw);
		};
	}
}