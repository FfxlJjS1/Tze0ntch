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

		std::ostream& operator<<(std::ostream& ostream, const SubNode& sub_node) {
			ostream.write((char*)(&(sub_node.my_vertex_id_1)), sizeof(sub_node.my_vertex_id_1));
			ostream.write((char*)(&(sub_node.my_vertex_id_2)), sizeof(sub_node.my_vertex_id_2));

			return ostream;
		}

		std::istream& operator>>(std::istream& istream, SubNode& sub_node) {
			auto& sub_node_vertex_id_1 = sub_node.my_vertex_id_1;
			auto& sub_node_vertex_id_2 = sub_node.my_vertex_id_2;

			istream.read((char*)&sub_node_vertex_id_1, sizeof(sub_node_vertex_id_1));
			istream.read((char*)&sub_node_vertex_id_2, sizeof(sub_node_vertex_id_2));

			return istream;
		}
	}
}
