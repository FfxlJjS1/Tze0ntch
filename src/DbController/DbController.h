#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <vector>

#include "SemanticWeb.h"

using std::ios;
using std::ios_base;
using std::fstream;
using std::sort;

// A opened database metadates
struct DbMetadata {
public:
	uint16_t firstStructuresCount; // Now, it is not changible
	uint16_t secondStructuresCount; // Now, it is not changible
	size_t secondStructuresStartLineIndex;
	vertexIdType lastVertexWroteId;
	vertexIdType lastNodeWroteId;
};

enum class OperationParametrs : char {
	Adding,
	Changing,
	Deleting
};

class DbController final {
private:
	fstream m_dbStream;
	string m_dbFileName;
	DbMetadata m_dbMetadata{ 20, 20, 2, 0, 0 };
	SemanticWeb m_bufferedToWriteSemanticWeb;

public:
	DbController() : m_bufferedToWriteSemanticWeb(new SemanticWeb(true)){
		m_dbFileName = "DB.mdd";
	};

	DbController(const string dbFileName) : DbController() {
		m_dbFileName = dbFileName;
	}

	~DbController() {
		if (m_dbStream.is_open()) {
			m_dbStream.close();
		}
	}

	// Get last wrote vertex's id to db file
	vertexIdType GetLastWroteVertexId() const;

	// Get last wrote node's id to db file
	vertexIdType GetLastWroteNodeId() const;


	// Operations by DB

	/*
	  Create a database file called m_dbFileName, then unload basci data
	  Or return it to basic form
	  If file is not created return false, else true
	*/
	bool CreateStructuredDbFile();

	/* 
	  Open a default database file path for read for cannot be deletable file
	  If file is not opened by this method return false, else return true
	*/
	bool OpenDb();

	/*
	  For check closing of file
	*/
	bool DbIsOpened() const;

	// Close connection with db file
	// You can unload changes before closing
	void CloseDataBase(const bool toSycn = false);

	// Only try to delete a data base file, without anything
	void DeleteDataBaseFile();


	// Operation by DB file for unload changed data

	/*
	  It will write changes from m_bufferedSemanticWeb into database file
	  It will rewrite all data from database file and replace or delete vertexies and nodes pointed in m_bufferedSemanticWeb
	  So it will read-write many times, so use this method few times
	*/
	void UnloadChangesIntoDBFile();


	// Operations by vertexes

	void OperationVertexInBuffer(Vertex* const vertex, const OperationParametrs operation);

	/*
	  Points vertexes in argument for adding, changing or deleting in database file
	  This method copy all vertexes data and not delete parametr links
	  Only id is reading for delete operation
	*/
	void OperationVertexesInBuffer(const vector<Vertex*>& vertexes, const OperationParametrs operation);

	/*
	  Read vertex that have vertexId as himself vertexId
	  If the vertex not found return new empty vector
	  This method return link to the found vertex
	*/
	Vertex* ReadVertex(const vertexIdType vertexId);

	/*
	  Reads vertexes that have vertexIds as himself vertexId
	  If such vertexies not found return new empty vector
	  This method return vector in new place contains link to found vertexes that did add to bufferedSW
	*/
	vector<Vertex*> ReadVertexes(vector<vertexIdType> vertexIds);

	/*
	  Reads vertexes that have text equal himself text
	  If such vertexes not found return new empty vector
	  This method return new vector in new place contains vertex's ids with equal text
	  Method don't add found vertexes to buffSW
	*/
	vector<vertexIdType>* ReadVertexIdsWithText(const string text);

	// Undo all changes like adding, changing, deleting for Vertexes
	void UndoAllChangesForVertexies(const vector<vertexIdType>& vertexIds);


	// Operations by nodes

	void OperationNodeInBuffer(Node* const node, const OperationParametrs operation);

	/*
	  Points nodes in argument for adding, changing or deleting in database file
	  This method copy all nodes data and not delete parametr links
	  Only id is reading for delete operation
	  Change cannot be undo, because it change subNodes of nodes
	*/
	void OperationNodesInBuffer(const vector<Node*>& nodes, const OperationParametrs operation);

	/*
	  Read node that have the nodeId as himself nodeId
	  If a node not fount return nullptr
	  This method return link to founded node
	*/
	Node* ReadNode(const vertexIdType nodeId);

	/*
	  Reads nodes that have nodeIds as himself nodeId
	  If such nodes not found return nullptr
	  This method return vector constains links to found nodes
	*/
	vector<Node*> ReadNodes(vector<vertexIdType> nodeIds);

	// Undo all changes like adding, changing, deleting for Nodes and 
	void UndoAllChangesForNodes(const vector<vertexIdType>& nodeIds);

	/*
	* Find nodes contains subNode contains vertexId in one of two object
	* If such nodes not found return nullptr
	*/
	vector<vertexIdType> ReadNodeIdsWithVertexIds(vector<vertexIdType>& vertexIds);


	// Operations by subNodes in nodes
	/*
	  Points subNodes in argument for addingm changing or deleting in database file
	  This method copy all subNodes data and not delete parametr links
	  Only node's id and ids of subNode is reading for delete operation
	*/
	void OperationSubNodesInBuffer(const vertexIdType nodeId, vector<SubNode>& subNodes, const OperationParametrs operation);

	// Undo all changes like adding, changind, deleting for subNodes of Node
	void UndoAllChangesForNode(const vertexIdType nodeId);
	
	/*
	  Undo all changes like adding, changing, deleting for subNodes of Node
	*/
	void UndoAllChangesForSubNode(const vertexIdType nodeId, const vector<vertexIdType>& subNodeIds);

	// Undo all changing
	void UndoAllChanges();


private:
	/*
	  Delete ids in add, change, delete vectors of vertexes and nodes for unchangeable in db file
	*/
	void ClearNonWritableElements();

	// Operations for reading

	// Place the pointer at the beginning of the file and load metadatas to m_dbMetadatas
	void ReadMetadataAtCurrentPosition();

	// Read data as Vertex type from current point in line of the file
	Vertex* ReadVertexAtCurrentPosition();

	// Read data as Node type from current point in line of the file
	Node* ReadNodeAtCurrentPosition();
	

	// Operations for writing

	/*
	* Write Metadata type as data to current point in line of the file
	* It using put pointer, please be careful
	*/
	void WriteMetadataAtCurrentPosition(DbMetadata& dbMetadata, fstream& dbFstream);

	/*
	* Write Vertex type as data to current point in line of the file
	* It using put pointer, please be careful
	*/
	void WriteVertexAtCurrentPosition(const Vertex &vertex, fstream &dbFstream);

	/*
	* Write Node type as data to current point in line of the file
	* It using put pointer, please be careful
	*/ 
	void WriteNodeAtCurrentPosition(const Node &node, fstream &dbFstream);

	/*
	* Write SubNode type as data to current point in line of the file
	* It using put pointer, please be careful
	*/
	void WriteSubNodeAtCurrentPosition(const SubNode &subNode, fstream &dbFstream);

	void WriteBufferedVertexiesAtCurrentPosition(vertexIdType &newLastWroteVertexId, size_t &newSecondStructuresStartLineIndex, fstream &dbFstream);
	
	void WriteBufferedNodesAtCurrentPosition(vertexIdType &newLastWroteNodeId, fstream &dbFstream);
};