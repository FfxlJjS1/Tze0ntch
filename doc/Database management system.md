# Contents
**Modules:**
1. [[Database controller]];
2. [[Database structure]].

[[Home]]

# Info
**Database management system** is system (module) for controlling database controllers which by create, read, write and delete database and it data.

Has the [[System database|specific database]], which contains information about exist databases in folder.
The database always has the same name.

# Abstraction levels
1. **Controllers**
DBMS has controllers for read, write data from file (binary data stream) by using database structure, create and delete new database files.

2. **Database structure**
Specific structure of database file in memory. By using a specific structure a controller can work with binary data stream.

3. **Binary data stream (\[Database\] file)**
Stream of binary data in computer memory. Can be created, wrote, read and deleted;

# Principles of work
- DBMS has path for **databases folder** in [[System ini file|system.ini]];
- DBMS has **[[System database|system database]]** in which contents data about another databases that exists and has their **id and name**;
- DBMS has database controller that controlling **only one database**;
- DBMS can **create and delete database**;
- In DBMS may exists **only one database for only algorithms**. It is indicated in [[System database|system database]];
- Can be made **only one object of this module**.

## Class variables
- [[System database controller]].  System database have id 0;
- List of database controllers.

## Methods
**Private:**
- GetDatabaseId(string databaseName);
- GetDatabaseName(int databaseId).

**Public:**
- CreateDatabase(string databaseName);
- GetDatabaseController:
	- (int id);
	- (string databaseName).
- IncludeDatabase:
	- (string databaseName);
	- (int id, string databaseName).
- ExcludeDatabase:
	- (string databaseName);
	- (int id).
- DeleteDatabase:
	- (int id);
	- (string databaseName).