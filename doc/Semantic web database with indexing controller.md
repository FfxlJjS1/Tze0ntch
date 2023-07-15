# Content
- [[Semantic web with indexing database structure]].

[[Database controller]]

# Info
**Semantic web database controller** is database controller that provide controlling semantic web database file

## Principles
- It's concrete realization of [[Database controller|abstract database controller]];
- Store only vertexes, nodes and metadata;
- Provide basic get, set all basic data form (vertex, node, sub_node, text, id);
- Realization of semantic web is in another file/s.

## Variables
**Variables:**
- Semantic web - temp of database for more fast accessible to data than working with file;
- List of pair id and position relate start of them list - list of free id of vertexes and nodes;
- Changed list - pairs list consists of id (vertex, node, sub_node) and state (added, changed, removed);
- Metadata of file.
