#include "DBMS/DbControllers/Module/Representing.h"

namespace DBMS {
    namespace ModuleDbController {
        Representing::get_id() const {
            return my_id;
        }

        Representing::set_id(objectIdType new_id) {
            my_id = new_id;
        }

        Representing::get_name() const {
            return my_name;
        }

        Representing::set_name(std::string name) {
            my_name = name;
        }


        std::set<objectIdType> Representing::get_sub_representing_ids() const {
            return my_sub_representing_ids;
        }

        bool Representing::is_content_representing(objectIdType representing_id) const {
            return my_sub_representing_ids.count(representing_id);
        }

        void Representing::add_representing(objectIdType module_id) {
            my_sub_representing_ids.insert(module_id);
        }

        void Representing::remove_representing(objectIdType module_id) {
            my_sub_representing_ids.erase(module_id);
        }


        std::set<objectIdType> Representing::get_module_ids() const {
            return my_module_ids;
        }

        bool Representing::is_content_module(objectIdType module_id) const {
            return my_module_ids.count(module_id);
        }

        void Representing::add_module(objectIdType module_id) {
            my_module_ids.insert(module_id);
        }

        void Representing::remove_module(objectIdType module_id) {
            my_module_ids.erase(module_id);
        }
    }
}

