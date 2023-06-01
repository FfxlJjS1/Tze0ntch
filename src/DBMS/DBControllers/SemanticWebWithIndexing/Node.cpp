#include "../../../../include/DBMS/DbControllers/SemanticWebWithIndexing/Node.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		vector<SubNode> Node::find_by_predicate(std::function<bool(SubNode& sub_node)> predicate) const {
			vector<SubNode> subNodes;

			for (SubNode subNode : my_sub_nodes) {
				if (predicate(subNode)) {
					subNodes.push_back(subNode);
				}
			}

			return subNodes;
		}


		Node::Node(const vertexIdType node_id) : my_node_id(node_id) {};


		vertexIdType Node::get_node_id() const {
			return my_node_id;
		}

		void Node::set_node_id(const vertexIdType new_node_id) {
			my_node_id = new_node_id;
		};


		SubNode Node::get_sub_node(const vertexIdType sub_node_id) const {
			auto sub_nodes_iterator = my_sub_nodes.find(sub_node_id);
			
			if (sub_nodes_iterator == my_sub_nodes.end()) {
				return SubNode();
			}
			else {
				return *sub_nodes_iterator;
			}
		}

		void Node::remove_sub_node(const vertexIdType sub_node_id) {
			auto sub_nodes_iterator = my_sub_nodes.find(sub_node_id);

			if (sub_nodes_iterator != my_sub_nodes.end()) {
				my_sub_nodes.erase(sub_nodes_iterator);
			}
		}
	}
}