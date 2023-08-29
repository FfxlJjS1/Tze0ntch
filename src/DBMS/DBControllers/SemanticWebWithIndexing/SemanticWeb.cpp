#include "../../../../include/DBMS/DbControllers/SemanticWebWithIndexing/SemanticWeb.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		void SemanticWeb::set_vertex(const Vertex& vertex_from) {
			Vertex vertex = vertex_from;

			if (vertex_from.my_id == 0) {
				vertex.my_id = my_vertexes.rbegin()->my_id + 1;

				my_vertexes.push_back(vertex);
			}
			else {
				auto iterator = my_vertexes.begin();

				for (;
					iterator != my_vertexes.end()
					|| vertex_from.my_id <= iterator->my_id;
					iterator++);

				if (vertex_from.my_id == iterator->my_id) {
					iterator->my_text = vertex_from.my_text;
				}
				else {
					my_vertexes.insert(iterator, vertex_from);
				}
			}
		}

		Vertex SemanticWeb::get_vertex(const vertexIdType vertex_id) {
			auto iterator = std::find(
				my_vertexes.begin(), my_vertexes.end(),
				[vertex_id](const Vertex& vertex) {
					return vertex.my_id == vertex_id;
				}
			);

			if (iterator == my_vertexes.end()) {
				return Vertex();
			}

			return *iterator;
		}

		void SemanticWeb::remove_vertex(const vertexIdType vertex_id) {
			auto iterator = std::find(
				my_vertexes.begin(), my_vertexes.end(),
				[vertex_id](const Vertex& vertex) {
					return vertex.my_id == vertex_id;
				}
			);

			if (iterator != my_vertexes.end()) {
				my_vertexes.erase(iterator);
			}
		}


		void SemanticWeb::set_node(Node* node_from) {
			if (node_from->get_node_id() == 0) {
				throw "...";
			}

			auto iterator = my_nodes.begin();

			for (;
				iterator != my_nodes.end()
				|| node_from->get_node_id() <= (*iterator)->get_node_id();
				iterator++);

			if (iterator == my_nodes.end()) {
				my_nodes.push_back(node_from);
			}
			else if ((*iterator)->get_node_id() > node_from->get_node_id()) {
				my_nodes.insert(iterator, node_from);
			}
			else {
				throw "...";
			}
		}

		Node* SemanticWeb::get_node(const vertexIdType node_id) {
			for (auto iterator = my_nodes.begin();
				(*iterator)->get_node_id() <= node_id && iterator != my_nodes.end();
				iterator++) {
				if ((*iterator)->get_node_id() == node_id) {
					return *iterator;
				}
			}

			return nullptr;
		}

		void SemanticWeb::remove_node(const vertexIdType node_id) {
			for (auto iterator = my_nodes.begin();
				(*iterator)->get_node_id() <= node_id && iterator != my_nodes.end();
				iterator++) {
				if ((*iterator)->get_node_id() == node_id) {
					my_nodes.erase(iterator);

					break;
				}
			}
		}


		SemanticWeb::~SemanticWeb() {
			for (Node* node : my_nodes) {
				delete node;
			}
		}
	}
}