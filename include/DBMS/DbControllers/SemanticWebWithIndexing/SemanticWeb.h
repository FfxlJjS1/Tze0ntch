#pragma once

#include <vector>
#include <string>

#include "Vertex.h"
#include "Node.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		class SemanticWeb final {
		private:
			std::vector<Vertex> my_vertexes;
			std::vector<Node*> my_nodes;

		public:
			void set_vertex(const Vertex& vertex);

			Vertex get_vertex(const vertexIdType vertex_id);

			void remove_vertex(const vertexIdType vertex_id);


			void set_node(Node* node);

			Node* get_node(const vertexIdType node_id);

			void remove_node(const vertexIdType node_id);


			~SemanticWeb();
		};
	}
}