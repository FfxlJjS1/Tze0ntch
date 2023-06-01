#include "../../../../include/DBMS/DbControllers/SemanticWebWithIndexing/SubNode.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		bool SubNode::operator==(const SubNode& sub_node) const {
			return sub_node.my_id == my_id;
		}

		bool SubNode::operator==(const vertexIdType& sub_node_id) const {
			return my_id == sub_node_id;
		}

		bool SubNode::operator<(const SubNode& sub_node) const {
			return my_id < sub_node.my_id;
		}
	}
}
