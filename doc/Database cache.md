# Contents
- [[Database structure class]]

[[Database controller]]

# Info
**Database cache** is class for keeping data between DB Controller and database file for more faster work than direct access and work with database file.

## Princip of database cache
- Using flags for determine what have made with data:
	- Put flag: A flag that marks new record as putted to cache but haven't confirmed or saved at the moment;
	- Change flag: A flag that marks record as changed in cache but haven't unloaded to database file at the moment;
	- Block flag: A flag that marks record as blocked for other streams of system or transaction until current transaction have finished;
	- Delete flag: A flag that marks record as deleted instead of actually removing it from the database. This allows for the record to be restored later if needed.
- Using mutex for realizing a transaction;
- Is template for concrete DB Controller classes.

## Variables


## Methods
**Public:**
- Put();
- Get();
- Delete().
- flushCache(bool writeToFile)