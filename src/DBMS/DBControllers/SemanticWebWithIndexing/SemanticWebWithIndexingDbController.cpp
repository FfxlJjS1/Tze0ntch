#include "../../../../include/DBMS/DbControllers/SemanticWebWithIndexing/SemanticWebWithIndexingDbController.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		void SemanticWebWithIndexingDbController::read_metadata_from_db_file() {
			{
				stream_position& database_metadata_start_position = my_file_metadata.database_metadata_start_position;
				stream_position& vertexes_position_list_start_position = my_file_metadata.vertexes_position_list_start_position;
				stream_position& vertexes_list_start_position = my_file_metadata.vertexes_list_start_position;
				stream_position& nodes_position_list_strart_position = my_file_metadata.nodes_position_list_strart_position;
				stream_position& nodes_list_start_position = my_file_metadata.nodes_list_start_position;

				my_database_stream.seekg(ios_base::beg);

				my_database_stream.read((char*)&database_metadata_start_position, sizeof(stream_position));
				my_database_stream.read((char*)&vertexes_position_list_start_position, sizeof(stream_position));
				my_database_stream.read((char*)&vertexes_list_start_position, sizeof(stream_position));
				my_database_stream.read((char*)&nodes_position_list_strart_position, sizeof(stream_position));
				my_database_stream.read((char*)&nodes_list_start_position, sizeof(stream_position));
			}

			{
				my_database_stream.seekg(ios_base::beg, my_file_metadata.database_metadata_start_position);

				my_database_stream.read((char*)&(my_file_database_metadata.last_wrote_vertex_id), sizeof(vertexIdType));
				my_database_stream.read((char*)&(my_file_database_metadata.last_wrote_node_id), sizeof(vertexIdType));


				my_database_stream.seekg(ios_base::beg, my_file_metadata.vertexes_position_list_start_position);

				auto& vertexes_id_positions_list = my_file_database_metadata.vertexes_id_positions_list;

				do {
					vertexIdType vertex_id = 0;
					stream_position vertex_start_position = 0;

					my_database_stream.read((char*)&vertex_id, sizeof(vertexIdType));
					my_database_stream.read((char*)&vertex_start_position, sizeof(stream_position));

					vertexes_id_positions_list.insert(vertex_id, vertex_start_position);
				} while (vertex_id != my_file_database_metadata.last_wrote_vertex_id);


				my_database_stream.seekg(ios_base::beg, my_file_metadata.nodes_position_list_start_position);

				auto& nodes_id_positions_list = my_file_database_metadata.nodes_id_positions_list;

				do {
					vertexIdType node_id = 0;
					stream_position node_start_position = 0;

					my_database_stream.read((char*)&node_id, sizeof(vertexIdType));
					my_database_stream.read((char*)&node_start_position, sizeof(stream_position));

					nodes_id_positions_list.insert(vertex_id, vertex_start_position);
				} while (vertex_id != my_file_database_metadata.last_wrote_vertex_id);
			}
		}

		void SemanticWebWithIndexingDbController::read_vertexes_from_database_file(const vector<vertexIdType>& vertex_ids) {
			auto& vertexes_id_positions_list = my_file_database_metadata.vertexes_id_positions_list;

			for (vertexIdType vertex_id_index = 0, vertex_id_index < vertex_ids.size(); vertex_id_index++) {
				vertexIdType vertex_id = vertex_ids[vertex_id_index];
				auto vertex_position_iterator = vertexes_id_positions_list.find(vertex_id);

				if (vertex_position_iterator == vertexes_id_positions_list.end()) {
					continue;
				}


				my_database_stream.seekg(ios_base::beg, my_file_metadata.vertexes_position_list_start_position + (*vertex_position_iterator).second);

				string vertex_text = "";

				size_t count_for_read = 0;

				if (vertex_position_iterator + 1 == vertexes_id_positions_list.end()) {
					count_for_read = my_file_metadata.nodes_position_list_strart_position - (*vertex_position_iterator).second;
				}
				else {
					count_for_read = (*(vertex_position_iterator + 1)).second - (*vertex_position_iterator).second;
				}

				my_database_stream.read((char*)vertex_text, count_for_read);

				my_buffered_semantic_web.set_vertex(Vertex(vertex_id, vertex_text));
			}
		}

		void SemanticWebWithIndexingDbController::read_nodes_from_database_file(const vector<vertexIdType>& node_ids) {
			auto& nodes_id_positions_list = my_file_database_metadata.nodes_id_positions_list;

			for (vertexIdType node_id_index = 0, node_id_index < node_ids.size(); node_id_index++) {
				vertexIdType node_id = node_ids[node_id_index];
				auto node_position_iterator = nodes_id_positions_list.find(node_id);

				if (node_position_iterator == nodes_id_positions_list.end()) {
					continue;
				}


				Node* node = new Node(node_id);


				my_database_stream.seekg(ios_base::beg, my_file_metadata.nodes_position_list_start_position + (*node_position_iterator).second);

				__int32 sub_nodes_count_for_read = 0;

				if (node_position_iterator + 1 == nodes_id_positions_list.end()) {
					sub_nodes_count_for_read = ((my_database_stream.end()) - (*node_position_iterator).second) / (2 * sizeof(vertexIdType));
				}
				else {
					sub_nodes_count_for_read = ((*(node_position_iterator + 1)).second - (*node_position_iterator).second) / (2 * sizeof(vertexIdType));
				}

				while (sub_nodes_count_for_read > 0) {
					SubNode sub_node;

					my_database_stream.read((char*)&(sub_node.my_vertex_id_1), sizeof(vertexIdType));
					my_database_stream.read((char*)&(sub_node.my_vertex_id_2), sizeof(vertexIdType));

					node->set_sub_node(sub_node);

					sub_nodes_count_for_read--;
				}

				my_buffered_semantic_web.set_node(node);
			}
		}


		SemanticWebWithIndexingDbController::SemanticWebWithIndexingDbController(const dbIdType database_id, const std::string& database_file_name) : DbController(database_id, database_file_name) {
			open_database_file(ios_base::binary | ios::in | ios::out);

			read_metadata_from_db_file();
		};


		vertexIdType SemanticWebWithIndexingDbController::get_last_wrote_vertex_id() const {
			return my_file_database_metadata.last_wrote_vertex_id;
		}

		vertexIdType SemanticWebWithIndexingDbController::get_last_wrote_node_id() const {
			return my_file_database_metadata.last_wrote_node_id;
		}


	}
}
