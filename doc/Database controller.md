# Contents
- [[Database]];
- [[Database cache]].

**Concrete database controllers:**
- [[System database controller]];
- [[Key-value database controller]];
- [[Relational database controller]];
- [[Customizable database controller]].

[[Database management system]]

# Info
**Database controller (DB Controller)** is module for controlling one [[Database|database file]].

## Principles of database controller
- To **constructor send database file name**, for open database, **and id**, for identification;
- Database **controller have cache of database** for temp store of data for more fast access to data;
- **Database can be opened by only one database controller in the moment**;
- OpenDatabase() and CloseDatabase() are **private methods for DBMS**;
- Structures of database **can be relation, key-value and other**;
- Can use **SQL for a database with the structures** example relation;
- Database **may have one of the several database structures**;
- DB Controller **have abstract class and concrete classes** for more abstract code.

## Abstract DB Controller

### Variables
- DatabaseId is a unique identificatory integer of database. Id 1 is reserved for system database;
- DatabaseStream is a file stream by which can execute reading and writing data in database file;
- DatabaseCache is a specific class of database cache. For every concrete DB Controller class realized for DB structure. May be template (C++).

### Methods
**Private:**
- OpenDatabase();
- CloseDatabase().

**Public:**
- GetDatabaseId();
- flushCache(bool wtireToFile) - except customizable database controller;