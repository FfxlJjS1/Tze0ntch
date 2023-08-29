#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <string>

#include "../DbController.h"
#include "SemanticWeb.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		typedef __int64 stream_position;

		class SemanticWebWithIndexingDbController final : public DbController {
		private:
			struct FileMetadata {
				stream_position database_metadata_start_position;
				stream_position vertexes_position_list_start_position;
				stream_position nodes_position_list_start_position;
			} my_file_metadata;

			struct FileDatabaseMetadata {
				vertexIdType last_wrote_vertex_id;
				std::map<vertexIdType, stream_position> vertex_ids_positions_list;

				vertexIdType last_wrote_node_id;
				std::map<vertexIdType, stream_position> node_ids_positions_list;
			} my_file_database_metadata;

			struct ChangeListForFileDatabase {
				std::set<vertexIdType> vertex_ids_for_set;
				std::set<vertexIdType> vertex_ids_for_remove;

				std::set<vertexIdType> node_ids_for_set;
				std::set<vertexIdType> node_ids_for_remove;

				std::set<std::pair<vertexIdType, vertexIdType>> sub_node_ids_for_set;
				std::set<std::pair<vertexIdType, vertexIdType>> sub_node_ids_for_remove;
			} my_change_list_for_file_database;

			SemanticWeb my_buffered_semantic_web;


			void read_metadata_from_db_file();

			void read_vertexes_from_database_file(const vector<vertexIdType>& vertex_ids);

			void read_nodes_from_database_file(const vector<vertexIdType>& node_ids);

		public:
			SemanticWebWithIndexingDbController(const dbIdType database_id, const string& database_file_name);


			static void init_database_file(const string& database_file_path);


			vertexIdType get_last_wrote_vertex_id() const;

			vertexIdType get_last_wrote_node_id() const;


			Vertex read_vertex(const vertexIdType vertex_id_for_read);

			vector<Vertex> read_vertexes(vector<vertexIdType> vertex_ids_for_read);

			void write_vertex(const Vertex& vertex_id_for_write);

			void write_vertexes(const vector<Vertex>& vertex_ids_for_write);

			void remove_vertex(const vertexIdType vertex_id_for_remove);

			void remove_vertexes(const vector<vertexIdType>& vertex_ids_for_remove);


			Node* read_node(const vertexIdType node_id);

			vector<Node*> read_nodes(vector<vertexIdType> node_ids);

			void write_node(Node* node);

			void write_nodes(const vector<Node*>& nodes);

			void remove_node(const vertexIdType node_id);

			void remove_nodes(const vector<vertexIdType>& node_ids);


			// Working correctly with read nodes with sub_nodes which have sub_node_id in SemanticWeb
			SubNode read_sub_node(const vertexIdType node_id, const vertexIdType sub_node_id) const;

			// Working correctly with read nodes with sub_nodes which have sub_node_id in SemanticWeb
			vector<SubNode> read_sub_nodes(const vertexIdType node_id, const vector<vertexIdType>& sub_node_ids) const;

			void write_sub_node(const vertexIdType node_id, const SubNode& sub_node);

			void write_sub_nodes(const vertexIdType node_id, const vector<SubNode>& sub_nodes);

			// Working correctly with read nodes with sub_nodes which have sub_node_id in SemanticWeb
			void remove_sub_node(const vertexIdType node_id, const vertexIdType sub_node_id);

			// Working correctly with read nodes with sub_nodes which have sub_node_id in SemanticWeb
			void remove_sub_nodes(const vertexIdType node_id, const vector<vertexIdType>& sub_node_ids);


			void flush_cache() override;

		private:
			template<typename Obj>
			std::pair<stream_position, vertexIdType> write_objects_with_metadata_at_the_position(std::fstream& new_db_file_stream,
				const std::set<vertexIdType>& object_ids_for_set,
				const std::map<vertexIdType, stream_position>& object_ids_positions_list,
				const std::set<vertexIdType>& object_ids_for_remove,
				std::vector<Obj>(*MethodForRead)(vector<vertexIdType>));


			std::pair<stream_position, vertexIdType> write_vertexes_with_metadata_at_the_position(std::fstream& new_db_file_stream);

			std::pair<stream_position, vertexIdType> write_nodes_with_metadata_at_the_position(std::fstream& new_db_file_stream);
		};
	}
}