#include "../../../../include/DBMS/DbControllers/SemanticWebWithIndexing/SemanticWebWithIndexingDbController.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		void SemanticWebWithIndexingDbController::read_metadata_from_db_file() {
			{
				// Read general database file metadata
				auto& database_metadata_start_position = my_file_metadata.database_metadata_start_position;
				auto& vertexes_position_list_start_position = my_file_metadata.vertexes_position_list_start_position;
				auto& nodes_position_list_start_position = my_file_metadata.nodes_position_list_start_position;

				my_database_stream.seekg(sizeof(dbStructTypes), ios_base::beg);

				my_database_stream.read((char*)&database_metadata_start_position, sizeof(database_metadata_start_position));
				my_database_stream.read((char*)&vertexes_position_list_start_position, sizeof(vertexes_position_list_start_position));
				my_database_stream.read((char*)&nodes_position_list_start_position, sizeof(nodes_position_list_start_position));
			}

			{
				my_database_stream.seekg(my_file_metadata.database_metadata_start_position, ios_base::beg);

				auto& last_wrote_vertex_id = my_file_database_metadata.last_wrote_vertex_id;
				auto& last_wrote_node_id = my_file_database_metadata.last_wrote_node_id;

				my_database_stream.read((char*)&last_wrote_vertex_id, sizeof(last_wrote_vertex_id));
				my_database_stream.read((char*)&last_wrote_node_id, sizeof(last_wrote_node_id));


				my_database_stream.seekg(my_file_metadata.vertexes_position_list_start_position, ios_base::beg);

				auto& vertex_ids_positions_list = my_file_database_metadata.vertex_ids_positions_list;

				for (vertexIdType vertex_id = 0; vertex_id != last_wrote_vertex_id;) {
					stream_position vertex_start_position = 0;

					my_database_stream.read((char*)&vertex_id, sizeof(vertexIdType));
					my_database_stream.read((char*)&vertex_start_position, sizeof(stream_position));

					vertex_ids_positions_list.insert(std::pair(vertex_id, vertex_start_position));
				}


				my_database_stream.seekg(my_file_metadata.nodes_position_list_start_position, ios_base::beg);

				auto& node_ids_positions_list = my_file_database_metadata.node_ids_positions_list;

				for (vertexIdType node_id = 0; node_id != last_wrote_node_id;) {
					stream_position node_start_position = 0;

					my_database_stream.read((char*)&node_id, sizeof(vertexIdType));
					my_database_stream.read((char*)&node_start_position, sizeof(stream_position));

					node_ids_positions_list.insert(std::pair(node_id, node_start_position));
				}
			}
		}

		void SemanticWebWithIndexingDbController::read_vertexes_from_database_file(const vector<vertexIdType>& vertex_ids) {
			auto& vertex_ids_positions_list = my_file_database_metadata.vertex_ids_positions_list;

			for (vertexIdType vertex_id_index = 0; vertex_id_index < vertex_ids.size(); vertex_id_index++) {
				vertexIdType vertex_id = vertex_ids[vertex_id_index];
				auto vertex_position_iterator = vertex_ids_positions_list.find(vertex_id);

				if (vertex_position_iterator == vertex_ids_positions_list.end()) {
					continue;
				}


				my_database_stream.seekg(my_file_metadata.vertexes_position_list_start_position + vertex_position_iterator->second, ios_base::beg);

				string vertex_text = "";

				size_t count_for_read = vertex_position_iterator->second;

				if (++vertex_position_iterator == vertex_ids_positions_list.end()) {
					count_for_read = my_file_metadata.nodes_position_list_start_position - count_for_read;
				}
				else {
					count_for_read = vertex_position_iterator->second - count_for_read;
				}

				my_database_stream.read((char*)&vertex_text, count_for_read);

				my_buffered_semantic_web.set_vertex(Vertex(vertex_id, vertex_text));
			}
		}

		void SemanticWebWithIndexingDbController::read_nodes_from_database_file(const vector<vertexIdType>& node_ids) {
			auto& node_ids_positions_list = my_file_database_metadata.node_ids_positions_list;

			for (vertexIdType node_id_index = 0; node_id_index < node_ids.size(); node_id_index++) {
				vertexIdType node_id = node_ids[node_id_index];
				auto node_position_iterator = node_ids_positions_list.find(node_id);

				if (node_position_iterator == node_ids_positions_list.end()) {
					continue;
				}


				Node* node = new Node(node_id);


				my_database_stream.seekg(my_file_metadata.nodes_position_list_start_position + node_position_iterator->second, ios_base::beg);

				size_t sub_nodes_count_for_read = node_position_iterator->second / (2 * sizeof(vertexIdType));

				if (++node_position_iterator == node_ids_positions_list.end()) {
					sub_nodes_count_for_read = my_database_stream.end - sub_nodes_count_for_read;
				}
				else {
					sub_nodes_count_for_read = node_position_iterator->second - sub_nodes_count_for_read;
				}

				SubNode sub_node;
				auto& my_vertex_id_1 = sub_node.my_vertex_id_1;
				auto& my_vertex_id_2 = sub_node.my_vertex_id_2;

				while (sub_nodes_count_for_read > 0) {
					my_database_stream.read((char*)&my_vertex_id_1, sizeof(my_vertex_id_1));
					my_database_stream.read((char*)&my_vertex_id_2, sizeof(my_vertex_id_2));

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


		Vertex SemanticWebWithIndexingDbController::read_vertex(const vertexIdType vertex_id_for_read) {
			vector<Vertex> vertexes = read_vertexes(vector<vertexIdType>{vertex_id_for_read});
			Vertex read_vertex;

			if (vertexes.size() > 0) {
				read_vertex = vertexes[0];
			}

			return read_vertex;
		}

		vector<Vertex> SemanticWebWithIndexingDbController::read_vertexes(vector<vertexIdType> vertex_ids_for_read) {
			vector<Vertex> read_vertexes;

			std::sort(vertex_ids_for_read.begin(), vertex_ids_for_read.end());

			for (vertexIdType vertex_id_for_read : vertex_ids_for_read) {
				if (my_change_list_for_file_database.vertex_ids_for_remove.find(vertex_id_for_read) != my_change_list_for_file_database.vertex_ids_for_remove.end()) {
					continue;
				}

				auto vertex_position_iterator = my_file_database_metadata.vertex_ids_positions_list.find(vertex_id_for_read);
				if (vertex_position_iterator
					!= my_file_database_metadata.vertex_ids_positions_list.end()) {
					Vertex reading_vertex;

					my_database_stream.seekg(vertex_position_iterator->second, ios::beg);

					my_database_stream >> reading_vertex;

					read_vertexes.push_back(reading_vertex);
				}
				else {
					read_vertexes.push_back(my_buffered_semantic_web.get_vertex(vertex_id_for_read));
				}
			}

			return read_vertexes;
		}

		void SemanticWebWithIndexingDbController::write_vertex(const Vertex& vertex_id_for_write) {
			write_vertexes(vector<Vertex>{vertex_id_for_write});
		}

		void SemanticWebWithIndexingDbController::write_vertexes(const vector<Vertex>& vertexes_for_write) {
			for (const auto& vertex : vertexes_for_write) {
				const auto iterator_removed_mark = my_change_list_for_file_database.vertex_ids_for_remove.find(vertex.my_id);

				if (iterator_removed_mark != my_change_list_for_file_database.vertex_ids_for_remove.end()) {
					my_change_list_for_file_database.vertex_ids_for_remove.erase(iterator_removed_mark);
				}

				my_change_list_for_file_database.vertex_ids_for_set.insert(vertex.my_id);

				my_buffered_semantic_web.set_vertex(vertex);
			}
		}

		void SemanticWebWithIndexingDbController::remove_vertex(const vertexIdType vertex_id_for_remove) {
			remove_vertexes(vector<vertexIdType> {vertex_id_for_remove});
		}

		void SemanticWebWithIndexingDbController::remove_vertexes(const vector<vertexIdType>& vertex_ids_for_remove) {
			for (const auto vertex_id_for_remove : vertex_ids_for_remove) {
				const auto iterator_set_mark = my_change_list_for_file_database.vertex_ids_for_set.find(vertex_id_for_remove);

				if (iterator_set_mark != my_change_list_for_file_database.vertex_ids_for_set.end()) {
					my_change_list_for_file_database.vertex_ids_for_set.erase(iterator_set_mark);
				}

				if (my_file_database_metadata.vertex_ids_positions_list.find(vertex_id_for_remove) != my_file_database_metadata.vertex_ids_positions_list.end()) {
					my_change_list_for_file_database.vertex_ids_for_remove.insert(vertex_id_for_remove);
				}
			}
		}


		Node* SemanticWebWithIndexingDbController::read_node(const vertexIdType node_id) {
			vector<Node*> nodes = read_nodes(vector<vertexIdType>{node_id});
			Node* read_node;

			if (nodes.size() > 0) {
				read_node = nodes[0];
			}

			return read_node;
		}

		vector<Node*> SemanticWebWithIndexingDbController::read_nodes(vector<vertexIdType> node_ids_for_read) {
			vector<Node*> read_nodes;

			std::sort(node_ids_for_read.begin(), node_ids_for_read.end());

			for (vertexIdType node_id_for_read : node_ids_for_read) {
				if (my_change_list_for_file_database.node_ids_for_remove.find(node_id_for_read) != my_change_list_for_file_database.node_ids_for_remove.end()) {
					continue;
				}

				auto node_position_iterator = my_file_database_metadata.node_ids_positions_list.find(node_id_for_read);
				if (node_position_iterator
					!= my_file_database_metadata.node_ids_positions_list.end()) {
					Node* reading_node;

					my_database_stream.seekg(node_position_iterator->second, ios::beg);

					my_database_stream >> reading_node;

					read_nodes.push_back(reading_node);
				}
				else {
					read_nodes.push_back(my_buffered_semantic_web.get_node(node_id_for_read));
				}
			}
		}

		void SemanticWebWithIndexingDbController::write_node(Node* node) {
			write_nodes(vector<Node*>{node});
		}

		void SemanticWebWithIndexingDbController::write_nodes(const vector<Node*>& nodes_for_write) {
			for (auto& node : nodes_for_write) {
				const auto iterator_removed_mark = my_change_list_for_file_database.node_ids_for_remove.find(node->get_node_id());

				if (iterator_removed_mark != my_change_list_for_file_database.node_ids_for_remove.end()) {
					my_change_list_for_file_database.node_ids_for_remove.erase(iterator_removed_mark);
				}

				my_change_list_for_file_database.node_ids_for_set.insert(node->get_node_id());

				my_buffered_semantic_web.set_node(node);
			}
		}

		void SemanticWebWithIndexingDbController::remove_node(const vertexIdType node_id) {
			remove_nodes(vector<vertexIdType> {node_id});
		}

		void SemanticWebWithIndexingDbController::remove_nodes(const vector<vertexIdType>& node_ids_for_remove) {
			for (const auto node_id_for_remove : node_ids_for_remove) {
				const auto iterator_set_mark = my_change_list_for_file_database.node_ids_for_set.find(node_id_for_remove);

				if (iterator_set_mark != my_change_list_for_file_database.node_ids_for_set.end()) {
					my_change_list_for_file_database.node_ids_for_set.erase(iterator_set_mark);
				}

				if (my_file_database_metadata.node_ids_positions_list.find(node_id_for_remove) != my_file_database_metadata.node_ids_positions_list.end()) {
					my_change_list_for_file_database.node_ids_for_remove.insert(node_id_for_remove);
				}
			}
		}


		void SemanticWebWithIndexingDbController::flush_cache() {
			const string new_db_file_path = my_database_file_name + ".tmp";

			std::fstream new_db_file_stream(new_db_file_path, ios_base::binary | ios_base::in | ios_base::out | ios_base::trunc);

			new_db_file_stream.seekp(0, ios::beg);

			// TODO: May be here will be error with pointer to undefined object because it's literal
			new_db_file_stream.write((char*)dbStructTypes::SemanticWebWithIndexingDbController, sizeof(dbStructTypes));


			auto database_metadata_start_position = my_file_metadata.database_metadata_start_position;
			auto vertexes_position_list_start_position = my_file_metadata.vertexes_position_list_start_position;
			auto nodes_position_list_start_position = my_file_metadata.nodes_position_list_start_position;

			auto last_wrote_vertex_id = my_file_database_metadata.last_wrote_vertex_id;
			auto last_wrote_node_id = my_file_database_metadata.last_wrote_node_id;


			new_db_file_stream.write((char*)&database_metadata_start_position, sizeof(database_metadata_start_position));
			new_db_file_stream.write((char*)&vertexes_position_list_start_position, sizeof(vertexes_position_list_start_position));
			new_db_file_stream.write((char*)&nodes_position_list_start_position, sizeof(nodes_position_list_start_position));

			
			database_metadata_start_position = new_db_file_stream.tellp();


			new_db_file_stream.write((char*)&last_wrote_vertex_id, sizeof(last_wrote_vertex_id));
			new_db_file_stream.write((char*)&last_wrote_node_id, sizeof(last_wrote_node_id));


			/*
			std::pair<stream_position, vertexIdType> vertexes_metadata = write_objects_with_metadata_at_the_position<Vertex>(
				new_db_file_stream,
				my_change_list_for_file_database.vertex_ids_for_set,
				my_file_database_metadata.vertex_ids_positions_list,
				my_change_list_for_file_database.vertex_ids_for_remove,
				read_vertexes);
			*/
			std::pair<stream_position, vertexIdType> vertexes_metadata = write_vertexes_with_metadata_at_the_position(new_db_file_stream);

			vertexes_position_list_start_position = vertexes_metadata.first;
			last_wrote_vertex_id = vertexes_metadata.second;


			/*
			std::pair<stream_position, vertexIdType> nodes_metadata = write_objects_with_metadata_at_the_position<Node*>(new_db_file_stream,
				my_change_list_for_file_database.node_ids_for_set,
				my_file_database_metadata.node_ids_positions_list,
				my_change_list_for_file_database.node_ids_for_remove,
				read_nodes);
			*/
			std::pair<stream_position, vertexIdType> nodes_metadata = write_vertexes_with_metadata_at_the_position(new_db_file_stream);

			nodes_position_list_start_position = nodes_metadata.first;
			last_wrote_node_id = nodes_metadata.second;


			new_db_file_stream.seekp(sizeof(dbStructTypes), ios::beg);

			new_db_file_stream.write((char*)&database_metadata_start_position, sizeof(database_metadata_start_position));
			new_db_file_stream.write((char*)&vertexes_position_list_start_position, sizeof(vertexes_position_list_start_position));
			new_db_file_stream.write((char*)&nodes_position_list_start_position, sizeof(nodes_position_list_start_position));

			new_db_file_stream.write((char*)&last_wrote_vertex_id, sizeof(last_wrote_vertex_id));
			new_db_file_stream.write((char*)&last_wrote_node_id, sizeof(last_wrote_node_id));

			
			my_database_stream.close();
			new_db_file_stream.close();

			int perf_state = -1;

			if ((perf_state = rename(my_database_file_name.c_str(), (my_database_file_name + ".backup").c_str())) == 0
				&& rename(new_db_file_path.c_str(), my_database_file_name.c_str()) == 0) {
				remove((my_database_file_name + ".backup").c_str());

				my_database_stream.open(my_database_file_name);
			}
			else {
				std::cout << "Unsucssesful unloading of data to db" << std::endl;

				if (perf_state == 0
					&& rename((my_database_file_name + ".backup").c_str(), my_database_file_name.c_str()) == 0) {
					std::cout << "Old db file have restored. Please check happened situation and try unload updates to db again" << std::endl;

					my_database_stream.open(my_database_file_name);
				}
				else {
					std::cout << "Cannot be restore old db file from backup. Please will be rename {db}.backup by self" << std::endl;
				}
			}
		}

		template<typename Obj>
		std::pair<stream_position, vertexIdType> SemanticWebWithIndexingDbController::write_objects_with_metadata_at_the_position(
			std::fstream& new_db_file_stream,
			const std::set<vertexIdType>& object_ids_for_set,
			const std::map<vertexIdType, stream_position>& object_ids_positions_list,
			const std::set<vertexIdType>& object_ids_for_remove,
			std::vector<Obj>(*MethodForRead)(vector<vertexIdType>)) {
			std::pair<stream_position, vertexIdType> result;
			result.first = new_db_file_stream.tellp();


			std::set<vertexIdType> all_object_ids_for_write = object_ids_for_set;

			const auto& object_ids_positions_list = object_ids_positions_list;
			const auto& object_ids_for_remove = object_ids_for_remove;

			for (const auto& object_id_position : object_ids_positions_list) {
				if (object_ids_for_remove.count(object_id_position.first) == 0) {
					all_object_ids_for_write.insert(object_id_position.first);
				}
			}

			result.second = *(all_object_ids_for_write.rbegin());


			const size_t objects_id_position_list_elements_count = all_object_ids_for_write.size();

			new_db_file_stream.seekp(objects_id_position_list_elements_count
				* (sizeof(vertexIdType) + sizeof(stream_position)), ios::cur);


			std::set<vertexIdType>::iterator iterator = all_object_ids_for_write.begin();
			vector<std::pair<vertexIdType, stream_position>> wrote_object_ids_positions;
			const int count_for_one_iteration = 20;

			for (size_t wrote_objects_count = 0;
				wrote_objects_count < objects_id_position_list_elements_count;
				wrote_objects_count += count_for_one_iteration) {
				const int count_for_read = wrote_objects_count + count_for_one_iteration <= objects_id_position_list_elements_count
					? count_for_one_iteration
					: objects_id_position_list_elements_count - wrote_objects_count;

				vector<vertexIdType> object_ids_for_read(count_for_read);

				std::copy_n(iterator, count_for_read, object_ids_for_read.begin());

				std::advance(iterator, count_for_read);


				vector<Obj> objects_for_write = MethodForRead(object_ids_for_read);
				size_t object_for_write_index = 0;

				for (size_t object_for_write_index = 0;
					object_for_write_index < objects_for_write.size();
					object_for_write_index++) {
					const Obj& vertex_for_write = objects_for_write[object_for_write_index];

					wrote_object_ids_positions.push_back(std::pair(vertex_for_write.my_id, new_db_file_stream.tellp()));

					new_db_file_stream << vertex_for_write;
				}
			}

			stream_position position_after_wrote_objects_block_relate_begin = new_db_file_stream.tellp();


			new_db_file_stream.seekp(result.first, ios::beg);

			for (const auto& wrote_object_id_position : wrote_object_ids_positions) {
				auto& object_id = wrote_object_id_position.first;
				auto& object_position = wrote_object_id_position.second;

				new_db_file_stream.write((char*)&object_id, sizeof(object_id));
				new_db_file_stream.write((char*)&object_position, sizeof(object_position));
			}


			new_db_file_stream.seekp(position_after_wrote_objects_block_relate_begin, ios::beg);

			return result;
		}


		std::pair<stream_position, vertexIdType> SemanticWebWithIndexingDbController::write_vertexes_with_metadata_at_the_position(std::fstream& new_db_file_stream) {
			std::pair<stream_position, vertexIdType> result;
			result.first = new_db_file_stream.tellp();


			std::set<vertexIdType> all_vertex_ids_for_write = my_change_list_for_file_database.vertex_ids_for_set;

			const auto& vertex_ids_positions_list = my_file_database_metadata.vertex_ids_positions_list;
			const auto& vertex_ids_for_remove = my_change_list_for_file_database.vertex_ids_for_remove;

			for (const auto& vertex_id_position : vertex_ids_positions_list) {
				if (vertex_ids_for_remove.count(vertex_id_position.first) == 0) {
					all_vertex_ids_for_write.insert(vertex_id_position.first);
				}
			}

			result.second = *(all_vertex_ids_for_write.rbegin());


			const size_t vertexes_id_position_list_elements_count = all_vertex_ids_for_write.size();

			new_db_file_stream.seekp(vertexes_id_position_list_elements_count
				* (sizeof(vertexIdType) + sizeof(stream_position)), ios::cur);


			std::set<vertexIdType>::iterator iterator = all_vertex_ids_for_write.begin();
			vector<std::pair<vertexIdType, stream_position>> wrote_vertex_ids_positions;
			const int count_for_one_iteration = 20;

			for (size_t wrote_vertexes_count = 0;
				wrote_vertexes_count < vertexes_id_position_list_elements_count;
				wrote_vertexes_count += count_for_one_iteration) {
				const int count_for_read = wrote_vertexes_count + count_for_one_iteration <= vertexes_id_position_list_elements_count
					? count_for_one_iteration
					: vertexes_id_position_list_elements_count - wrote_vertexes_count;

				vector<vertexIdType> vertex_ids_for_read(count_for_read);

				std::copy_n(iterator, count_for_read, vertex_ids_for_read.begin());

				std::advance(iterator, count_for_read);


				vector<Vertex> vertexes_for_write = read_vertexes(vertex_ids_for_read);
				size_t vertex_for_write_index = 0;

				for (size_t vertex_for_write_index = 0;
					vertex_for_write_index < vertexes_for_write.size();
					vertex_for_write_index++) {
					const Vertex& vertex_for_write = vertexes_for_write[vertex_for_write_index];

					wrote_vertex_ids_positions.push_back(std::pair(vertex_for_write.my_id, new_db_file_stream.tellp()));

					new_db_file_stream << vertex_for_write;
				}
			}

			stream_position position_after_wrote_vertexes_block_relate_begin = new_db_file_stream.tellp();


			new_db_file_stream.seekp(result.first, ios::beg);

			for (const auto& wrote_vertex_id_position : wrote_vertex_ids_positions) {
				auto& vertex_id = wrote_vertex_id_position.first;
				auto& vertex_position = wrote_vertex_id_position.second;

				new_db_file_stream.write((char*)&vertex_id, sizeof(vertex_id));
				new_db_file_stream.write((char*)&vertex_position, sizeof(vertex_position));
			}


			new_db_file_stream.seekp(position_after_wrote_vertexes_block_relate_begin, ios::beg);

			return result;
		}

		std::pair<stream_position, vertexIdType> SemanticWebWithIndexingDbController::write_nodes_with_metadata_at_the_position(std::fstream& new_db_file_stream) {
			std::pair<stream_position, vertexIdType> result;
			result.first = new_db_file_stream.tellp();


			std::set<vertexIdType> all_node_ids_for_write = my_change_list_for_file_database.node_ids_for_set;

			const auto& node_ids_positions_list = my_file_database_metadata.node_ids_positions_list;
			const auto& node_ids_for_remove = my_change_list_for_file_database.node_ids_for_remove;

			for (const auto& node_id_position : node_ids_positions_list) {
				if (node_ids_for_remove.count(node_id_position.first) == 0) {
					all_node_ids_for_write.insert(node_id_position.first);
				}
			}

			result.second = *(all_node_ids_for_write.rbegin());


			const size_t nodes_id_position_list_elements_count = all_node_ids_for_write.size();

			new_db_file_stream.seekp(nodes_id_position_list_elements_count
				* (sizeof(vertexIdType) + sizeof(stream_position)), ios::cur);


			std::set<vertexIdType>::iterator iterator = all_node_ids_for_write.begin();
			vector<std::pair<vertexIdType, stream_position>> wrote_node_ids_positions;
			const int count_for_one_iteration = 20;

			for (size_t wrote_nodes_count = 0;
				wrote_nodes_count < nodes_id_position_list_elements_count;
				wrote_nodes_count += count_for_one_iteration) {
				const int count_for_read = wrote_nodes_count + count_for_one_iteration <= nodes_id_position_list_elements_count
					? count_for_one_iteration
					: nodes_id_position_list_elements_count - wrote_nodes_count;

				vector<vertexIdType> node_ids_for_read(count_for_read);

				std::copy_n(iterator, count_for_read, node_ids_for_read.begin());

				std::advance(iterator, count_for_read);


				vector<Node*> nodes_for_write = read_nodes(node_ids_for_read);
				size_t node_for_write_index = 0;

				for (size_t node_for_write_index = 0;
					node_for_write_index < nodes_for_write.size();
					node_for_write_index++) {
					const Node* node_for_write = nodes_for_write[node_for_write_index];

					wrote_node_ids_positions.push_back(std::pair(node_for_write->get_node_id(), new_db_file_stream.tellp()));

					new_db_file_stream << node_for_write;
				}
			}

			stream_position position_after_wrote_nodes_block_relate_begin = new_db_file_stream.tellp();


			new_db_file_stream.seekp(result.first, ios::beg);

			for (const auto& wrote_node_id_position : wrote_node_ids_positions) {
				auto& node_id = wrote_node_id_position.first;
				auto& node_position = wrote_node_id_position.second;

				new_db_file_stream.write((char*)&node_id, sizeof(node_id));
				new_db_file_stream.write((char*)&node_position, sizeof(node_position));
			}


			new_db_file_stream.seekp(position_after_wrote_nodes_block_relate_begin, ios::beg);

			return result;
		}
	}
}
