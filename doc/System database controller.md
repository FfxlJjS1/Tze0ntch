# Content

- [[Database]];
- [[System database structure]].

[[Database controller]]

# Info
**System database controller** is a controller, which stores records about database files and their flags.

## Principles of this controller
- It's concrete realization of [[Database controller|abstract database controller]];
- Stores records about database files and flags of working setting with them.

## Variables and Methods
**Variables:**
- db_records is vector of database records, contains of database id in DBMS, database name, integer of database type.

**Methods:**
- get_database_id(string database_name);
- get_database_name(integer database_name);
- get_database_struct_type(integer database_id);
- add_database_record:
	- (integer database_id, string database_name, integer database_struct_type);
	- (string database_name, integer database_struct_type).
- remove_database_record(integer database_id).