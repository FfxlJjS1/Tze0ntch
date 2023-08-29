#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <functional>

#include "Vertex.h"
#include "SubNode.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		using std::vector;

		class Node final {
		private:
			vertexIdType my_node_id;

			mutable std::set<SubNode> my_sub_nodes;

			vector<SubNode> find_by_predicate(std::function<bool(SubNode& sub_node)> predicate) const;

		public:
			Node(const vertexIdType node_id);


			vertexIdType get_node_id() const;

			void set_node_id(const vertexIdType new_node_id);


			void set_sub_node(const SubNode& sub_node);

			SubNode get_sub_node(const vertexIdType sub_node_id) const;

			void remove_sub_node(const vertexIdType sub_node_id);


			vector<SubNode> find_by_vertex_id_any_where(const vertexIdType vertex_id) const;

			vector<SubNode> find_by_vertex_ids_any_where(const vector<vertexIdType>& vertex_ids) const;


			vector<SubNode> find_by_first_vertex_id(const vertexIdType first_vertex_id) const;

			vector<SubNode> find_by_first_vertex_ids(const vector<vertexIdType>& first_vertex_ids) const;


			vector<SubNode> find_by_second_vertex_id(const vertexIdType second_vertex_id) const;

			vector<SubNode> find_by_second_vertex_ids(const vector<vertexIdType>& second_vertex_ids) const;


			vector<SubNode> find_by_vertex_ids_any_where(const vertexIdType first_vertex_id, const vertexIdType second_vertex_id) const;


			friend std::ostream& operator<<(std::ostream& ostream, const Node& node);

			friend std::istream& operator>>(std::istream& istream, Node*& vertex);
		};
	}
}