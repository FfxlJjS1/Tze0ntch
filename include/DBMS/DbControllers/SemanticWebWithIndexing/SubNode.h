#pragma once

#include "Vertex.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		struct SubNode final {
		public:
			vertexIdType my_id = 0;
			vertexIdType my_vertex_id_1 = 0;
			vertexIdType my_vertex_id_2 = 0;

			bool operator==(const SubNode& sub_node) const;

			bool operator==(const vertexIdType& sub_node_id) const;

			bool operator<(const SubNode& sub_node) const;
		};
	}
}