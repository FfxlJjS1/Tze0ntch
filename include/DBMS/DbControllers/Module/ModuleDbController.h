#pragma once

#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <string>

#include "../DbController.h"
#include "Representing.h"
#include "Module.h"

namespace DBMS {
	namespace ModuleDbController {
		typedef __int64 stream_position;

		using std::vector, std::string, std::set, std::map;

		class ModuleDbController final : public DbController {
		private:
			struct FileMetadata {
				stream_position database_metadata_start_position;
				stream_position representings_position_list_start_position;
				stream_position modules_position_list_start_position;
			} my_file_metadata;

			struct FileDatabaseMetadata {
				map<objectIdType, stream_position> representing_ids_positions_list;

				map<objectIdType, stream_position> module_ids_positions_list;
			} my_file_database_metadata;

			struct ChangeListForFileDatabase {
				set<objectIdType> representing_ids_for_set;
				set<objectIdType> representing_ids_for_remove;

				set<objectIdType> module_ids_for_set;
				set<objectIdType> module_ids_for_remove;

				set<std::pair<objectIdType, objectIdType>> representing_module_ids_add;
				set<std::pair<objectIdType, objectIdType>> representing_module_ids_remove;
			} my_change_list_for_file_database;

			set<Module> my_modules;
			set<Representing> my_representings;


			void read_metadata_from_file();

			void read_modules_from_database_file(const vector<objectIdType>& module_ids);

			void read_representings_from_database_file(const vector<objectIdType>& representing_ids);

		public:
			ModuleDbController(const dbIdType database_id, const string& database_file_name);


			static void init_database_file(const string& database_file_path);


			Module read_module(const objectIdType module_id_for_read);

			vector<Module> read_modules(vector<objectIdType> module_ids_for_read);

			void write_module(const Module& module_for_write);

			void write_modules(const vector<Module>& modules_for_write);

			void remove_module(const objectIdType module_id_for_remove);

			void remove_modules(const vector<objectIdType>& module_ids_for_remove);

			void add_module_to_representing(const objectIdType module_id_for_add, const objectIdType representing_id);


			Representing read_representing(const objectIdType representing_id);

			vector<Representing> read_representings(vector<objectIdType> representing_ids);

			void write_representing(const Representing& representing_for_write);

			void write_representings(const vector<Representing>& representings_for_write);

			void remove_representing(const objectIdType& representing_id_for_remove);

			void remove_representings(const vector<objectIdType>& representing_ids_for_remove);


			void flush_cache() override;

		private:
			std::pair<stream_position, objectIdType> write_representings_with_metadata_at_the_position(std::fstream& new_db_file_stream);

			std::pair<stream_position, objectIdType> write_modules_with_metadata_at_the_position(std::fstream& new_db_file_stream);
		};
	}
}