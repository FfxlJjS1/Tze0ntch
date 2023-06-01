#pragma once

#include <string>

#include "Vertex.h"
#include "Node.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		class SemanticWeb final {
		private:
			vector<Vertex> my_vertexes;

			vector<Node*> my_nodes;

		public:
			void set_vertex(const Vertex& vertex);

			Vertex& get_vertex(const vertexIdType vertex_id);

			void remove_vertex(const vertexIdType vertex_id);


			void set_node(Node* node);

			Node* get_node(const vertexIdType node_id);

			void remove_node(const vertexIdType node_id);


			vector<vertexIdType> find_vertexes_equal_text(const string& text) const;

			vector<vertexIdType> find_vertexes_like_text(const string& text) const;


			~SemanticWeb() {
				for (Node* node : my_nodes) {
					delete node;
				}
			}
		};
	}
}