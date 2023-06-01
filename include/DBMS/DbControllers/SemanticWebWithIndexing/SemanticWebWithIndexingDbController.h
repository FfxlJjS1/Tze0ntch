#pragma once

#include <map>
#include <set>

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
				stream_position vertexes_list_start_position;
				stream_position nodes_position_list_strart_position;
				stream_position nodes_list_start_position;
			} my_file_metadata;

			struct FileDatabaseMetadata {
				vertexIdType last_wrote_vertex_id;
				std::map<vertexIdType, stream_position> vertexes_id_positions_list;

				vertexIdType last_wrote_node_id;
				std::map<vertexIdType, stream_position> nodes_id_positions_list;
			} my_file_database_metadata;

			struct ChangeListForFileDatabase {
				std::set<vertexIdType> vertexes_id_for_set;
				std::set<vertexIdType> vertexes_id_for_remove;

				std::set<vertexIdType> nodes_id_for_set;
				std::set<vertexIdType> nodes_id_for_remove;

				std::set<std::pair<vertexIdType, vertexIdType>> sub_nodes_id_for_set;
				std::set<std::pair<vertexIdType, vertexIdType>> sub_nodes_id_for_remove;
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


			const Vertex& read_vertex(const vertexIdType vertex_id) const;

			vector<const Vertex&> read_vertexes(const vector<vertexIdType>& vertex_ids);

			vector<vertexIdType> read_vertexes_equal_text(const string& vertex_text);

			vector<vertexIdType> read_vertexes_like_text(const string& vertex_text);

			void write_vertex(const Vertex& vertex);

			void write_vertexes(const vector<Vertex>& vertexes);

			void remove_vertex(const vertexIdType vertex_id);

			void remove_vertexes(const vector<vertexIdType>& vertex_id);


			const Node const* read_node(const vertexIdType node_id) const;

			vector<const Node const*> read_nodes(const vector<vertexIdType>& node_ids) const;

			void write_node(Node* node);

			void write_nodes(vector<Node*>& nodes);

			void remove_node(const vertexIdType node_id);

			void remove_nodes(const vector<vertexIdType>& node_ids);


			SubNode read_sub_node(const vertexIdType node_id, const vertexIdType sub_node_id) const;

			vector<SubNode> read_sub_nodes(const vertexIdType node_id, const vector<vertexIdType>& sub_node_ids) const;

			void write_sub_node(const vertexIdType node_id, const SubNode& sub_node);

			void write_sub_nodes(const vertexIdType node_id, const vector<SubNode>& sub_nodes);

			void remove_sub_node(const vertexIdType node_id, const vertexIdType sub_node_id);

			void remove_sub_nodes(const vertexIdType node_id, const vector<vertexIdType>& sub_node_ids);


			void flush_cache() override;
		};
	}
}