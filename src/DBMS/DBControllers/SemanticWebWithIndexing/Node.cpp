#include "../../../../include/DBMS/DbControllers/SemanticWebWithIndexing/Node.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		vector<SubNode> Node::find_by_predicate(std::function<bool(SubNode& sub_node)> predicate) const {
			vector<SubNode> sub_nodes;

			for (SubNode subNode : my_sub_nodes) {
				if (predicate(subNode)) {
					sub_nodes.push_back(subNode);
				}
			}

			return sub_nodes;
		}


		Node::Node(const vertexIdType node_id) : my_node_id(node_id) {};


		vertexIdType Node::get_node_id() const {
			return my_node_id;
		}

		void Node::set_node_id(const vertexIdType new_node_id) {
			my_node_id = new_node_id;
		};


		void Node::set_sub_node(const SubNode& sub_node) {
			auto sub_nodes_iterator = my_sub_nodes.end();

			if (sub_node.my_id != 0) {
				sub_nodes_iterator = std::find(
					my_sub_nodes.begin(),
					my_sub_nodes.end(),
					[&sub_node](const SubNode& sub_node_from) {
						return sub_node.my_id == sub_node_from.my_id;
					}
				);
			}

			if (sub_nodes_iterator == my_sub_nodes.end()) {
				SubNode res_sub_node = sub_node;
			
				res_sub_node.my_id = my_sub_nodes.rbegin()->my_id + 1;

				my_sub_nodes.insert(res_sub_node);
			}
			else {
				sub_nodes_iterator->my_vertex_id_1 = sub_node.my_vertex_id_1;
				sub_nodes_iterator->my_vertex_id_2 = sub_node.my_vertex_id_2;
			}
		}

		SubNode Node::get_sub_node(const vertexIdType sub_node_id) const {
			auto sub_nodes_iterator = std::find(
				my_sub_nodes.begin(), my_sub_nodes.end(),
				[&sub_node_id](const SubNode& subNode) {
					return subNode.my_id == sub_node_id;
				}
			);
			
			if (sub_nodes_iterator == my_sub_nodes.end()) {
				return SubNode();
			}
			else {
				return *sub_nodes_iterator;
			}
		}

		void Node::remove_sub_node(const vertexIdType sub_node_id) {
			auto sub_nodes_iterator = std::find(
			my_sub_nodes.begin(),
				my_sub_nodes.end(),
				[&sub_node_id](const SubNode& subNode) {
					return subNode.my_id == sub_node_id;
				}
			);

			if (sub_nodes_iterator != my_sub_nodes.end()) {
				my_sub_nodes.erase(sub_nodes_iterator);
			}
		}


		vector<SubNode> Node::find_by_vertex_id_any_where(vertexIdType vertexId) const {
			return find_by_vertex_ids_any_where(vector<vertexIdType> { vertexId });
		}

		vector<SubNode> Node::find_by_vertex_ids_any_where(const vector<vertexIdType>& vertex_ids) const {
			std::function<bool(SubNode& in_arg)> predicate = [&vertex_ids](SubNode& sub_node) {
				for (vertexIdType vertex_id : vertex_ids) {
					if (sub_node.my_id == vertex_id
						|| sub_node.my_vertex_id_1 == vertex_id
						|| sub_node.my_vertex_id_2 == vertex_id) {
						return true;
					}
				}

				return false;
			};

			return find_by_predicate(predicate);
		}

		vector<SubNode> Node::find_by_first_vertex_id(const vertexIdType first_vertex_id) const {
			return find_by_first_vertex_ids(vector<vertexIdType> { first_vertex_id });
		}

		vector<SubNode> Node::find_by_first_vertex_ids(const vector<vertexIdType>& first_vertex_ids) const {
			std::function<bool(SubNode& in_arg)> predicate = [&first_vertex_ids](SubNode& sub_node) {
				return find(first_vertex_ids.begin(), first_vertex_ids.end(), sub_node.my_vertex_id_1)
					!= first_vertex_ids.end();
			};

			return find_by_predicate(predicate);
		}


		vector<SubNode> Node::find_by_second_vertex_id(const vertexIdType second_vertex_id) const {
			return find_by_second_vertex_ids(vector<vertexIdType> { second_vertex_id });
		}

		vector<SubNode> Node::find_by_second_vertex_ids(const vector<vertexIdType>& second_vertex_ids) const {
			std::function<bool(SubNode& in_arg)> predicate = [&second_vertex_ids](SubNode& sub_node) {
				return find(second_vertex_ids.begin(), second_vertex_ids.end(), sub_node.my_vertex_id_2)
					!= second_vertex_ids.end();
			};

			return find_by_predicate(predicate);
		}


		vector<SubNode> Node::find_by_vertex_ids_any_where(const vertexIdType first_vertex_id, const vertexIdType second_vertex_id) const {
			std::function<bool(SubNode& in_arg)> predicate = [&first_vertex_id, &second_vertex_id](SubNode& sub_node) {
				return sub_node.my_vertex_id_1 == first_vertex_id
					&& sub_node.my_vertex_id_2 == second_vertex_id;
			};

			return find_by_predicate(predicate);
		}


		std::ostream& operator<<(std::ostream& ostream, const Node& node) {
			ostream.write((char*)node.my_node_id, sizeof(node.my_node_id));

			for (auto iterator = node.my_sub_nodes.begin(); iterator != node.my_sub_nodes.end(); iterator++) {
				ostream << *iterator;
			}

			ostream.write((char*)0, sizeof(node.my_node_id));

			return ostream;
		}

		std::istream& operator>>(std::istream& istream, Node*& node)
		{
			vertexIdType read_node_id;

			istream.read((char*)&read_node_id, sizeof(read_node_id));

			node = new Node(read_node_id);
			
			vertexIdType flag = 0;

			istream.read((char*)&flag, sizeof(flag));

			while(flag != 0) {
				SubNode sub_node_for_read;
				
				istream.seekg(-sizeof(flag), std::ios::cur);
				istream >> sub_node_for_read;

				node->set_sub_node(sub_node_for_read);

				istream.read((char*)&flag, sizeof(flag));
			}

			return istream;
		}
	}
}