# Contents
- [[Database structure]].

**Concrete database controllers:**
- [[System database controller]];
- [[Module database controller]];
- [[Semantic web database with indexing controller]];
- [[Relational database controller]];
- [[Customizable database controller]].

[[Database management system]]

# Info
**Database controller (DB Controller)** is module for controlling one [[Database|database file]].

## Principles of database controller
- **Send to constructor database file name**, for open database, **and id**, for identification;
- Database **controller have cache of database** for temp store of data for more fast access to data;
- **Database can be opened by only one database controller in the moment**;
- OpenDatabase() and CloseDatabase() are **private methods for DBMS**;
- Concrete realization of database controller may have a [[Database cache|database cache]] class for temp storing data from database file for more faster access to data.
- DB Controller **have abstract class and concrete classes** for more abstract code;
- With one database controller **can work only one \[client\] interpreter stream**.

## Abstract DB Controller

### Variables
- DatabaseId is a unique identificatory integer of database. Id 1 is reserved for [[System database controller|system database controller]];
- DatabaseStream is a file stream by which can be executed reading and writing data in database file.

### Methods
**Private:**
- OpenDatabase() - only open database stream;
- CloseDatabase() - only close database stream.

**Public:**
- GetDatabaseId();
- FlushCache(bool wtireToFile) - except customizable database controller;