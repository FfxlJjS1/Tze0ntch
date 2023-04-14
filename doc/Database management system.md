# Contents
**Modules:**
1. [[Database controller]].

[[Home]]

# Info
**Database management system** is system for controlling database controllers which by create, read, write and delete database and it data.

# Principles of work
- DBMS have path for **databases folder** in [[System ini file|system.ini]];
- DBMS have **[[System database|system database]]** in which contents data about another databases that exists and have their **id and name**;
- DBMS have database controller that controlling **only one database**;
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