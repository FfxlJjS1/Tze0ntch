# Contents

[[Database structure]]

# Info
**Key-value database structure** is database structure with key-value structure, where a key is a integer and a value is a text.

## Princip of this database structure
- Structure have four lists with specific data type and itself goal:
	- First, list contains three integer, which point start of id-point list, start of record list, start of node list;
	- Second, integer-integer, where first integer is id of text in a record, second integer is start byte of a record in file. List ordered by first integer. Called id-point list;
	- Third, text which is a second pair of id in id-point list. Ordered by id in id-point list. After text end start next record without null-character that can indicate the end of the record, because it is defined in id-point list and . Called record list.
	- Fourth, integer-pair of integer-integer, where first integer is id of record, that describe node between id of records. Called node list.