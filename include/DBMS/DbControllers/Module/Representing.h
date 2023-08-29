#pragma once

#include <string>
#include <set>

namespace DBMS {
	namespace ModuleDbController {
		typedef __int32 objectIdType;

		class Representing final {
		private:
			objectIdType my_id = 0;
			mutable std::string my_name = "";
			mutable std::set<objectIdType> my_sub_representing_ids;
			mutable std::set<objectIdType> my_module_ids;

		public:
			objectIdType get_id() const;

			void set_id(objectIdType new_id);

			std::string get_name() const;

			void set_name(std::string new_name);


			std::set<objectIdType> get_sub_representing_ids() const;

			bool is_content_representing(objectIdType representing_id) const;

			void add_representing(objectIdType representing_id);

			void remove_representing(objectIdType representing_id);

			
			std::set<objectIdType> get_module_ids() const;

			bool is_content_module(objectIdType module_id) const;

			void add_module(objectIdType module_id);

			void remove_module(objectIdType module_id);
		};
	}
}