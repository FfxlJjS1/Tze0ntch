#include "DbController.h"

/*
* In file, I calculate lines and position as index
* Because if one line contain 20 element, last element be 19 as index, that helping in calculate without mines 1 position and additional operations
*/

// Public methods
vertexIdType DbController::GetLastWroteVertexId() const {
	return m_dbMetadata.lastVertexWroteId;
}

vertexIdType DbController::GetLastWroteNodeId() const {
	return m_dbMetadata.lastNodeWroteId;
}

bool DbController::CreateStructuredDbFile() {
	// Return m_bufferedSemanticWeb to structure of basic elements
	m_bufferedToWriteSemanticWeb.Clear();
	m_bufferedToWriteSemanticWeb.fullByBasicElements();

	m_dbStream.open(m_dbFileName, ios_base::out | ios::in | ios_base::binary | ios_base::trunc);

	// If was not created
	if (!m_dbStream.is_open()) {
		return false;
	}

	// Fill db file by empty structure
	m_dbStream.seekp(0, ios::beg);

	WriteMetadataAtCurrentPosition(m_dbMetadata, m_dbStream);
	m_dbStream << "\n\n";

	m_dbStream.flush();

	// Fill db file by structure of basic elements
	SemanticWeb sw;
	sw.fullByBasicElements();

	sw.RemoveNode(SWObjectVertexForAddToDb);
	sw.RemoveNode(SWObjectNodeForAddToDb);
	sw.RemoveNode(SWObjectSubNodeForAddToDb);

	sw.RemoveNode(SWObjectVertexForChangeToDb);
	sw.RemoveNode(SWObjectSubNodeForChangeToDb);
	
	sw.RemoveNode(SWObjectVertexForRemoveToDb);
	sw.RemoveNode(SWObjectNodeForRemoveToDb);
	sw.RemoveNode(SWObjectSubNodeForRemoveToDb);

	OperationNodesInBuffer(*sw.GetNodes(), OperationParametrs::Adding);

	OperationVertexesInBuffer(*sw.GetVertexes(), OperationParametrs::Adding);

	sw.Clear();

	UnloadChangesIntoDBFile();

	m_dbStream.close();
	
	return true;
}

bool DbController::OpenDb() {	
	m_dbStream.open(m_dbFileName, ios_base::in | ios_base::binary);
	
	if (!m_dbStream.is_open()) {
		return false;
	}

	// Get Basic data
	ReadMetadataAtCurrentPosition();

	return true;
}

bool DbController::DbIsOpened() const {
	return m_dbStream.is_open();
}

void DbController::CloseDataBase(const bool toSync) {
	if (toSync) {
		UnloadChangesIntoDBFile();
	}

	m_bufferedToWriteSemanticWeb.Clear();

	m_dbStream.close();
}

void DbController::DeleteDataBaseFile() {
	remove(m_dbFileName.c_str());
}


void DbController::UnloadChangesIntoDBFile() {
	// Clear m_bufferedSemanticWeb for reduce the size
	ClearNonWritableElements();

	// Path and stream to new KB
	const string newDBfilePath = m_dbFileName + ".tmp";
	std::fstream newDBFile(newDBfilePath, ios_base::binary | ios_base::out | ios_base::trunc);

	// New metadata of the new KB
	size_t newSecondStructuresStartLineIndex = 2;
	vertexIdType newLastWroteVertexId = 0;
	vertexIdType newLastWroteNodeId = 0;

	// Run writing
	// Go to begin of older and ned KB
	m_dbStream.seekg(0, ios::beg);
	newDBFile.seekp(0, ios::beg);

	// Place need data size for will be rewrited at the end of this method
	newDBFile.seekp(0, ios::beg);
	WriteMetadataAtCurrentPosition(m_dbMetadata, newDBFile);

	newDBFile << "\n";

	WriteBufferedVertexiesAtCurrentPosition(newLastWroteVertexId, newSecondStructuresStartLineIndex, newDBFile);

	// Go to start line of Nodes
	newDBFile << "\n";
	
	WriteBufferedNodesAtCurrentPosition(newLastWroteNodeId, newDBFile);

	// Update metadata about KB
	m_dbMetadata.secondStructuresStartLineIndex = newSecondStructuresStartLineIndex;
	m_dbMetadata.lastVertexWroteId = newLastWroteVertexId;
	m_dbMetadata.lastNodeWroteId = newLastWroteNodeId;

	newDBFile.seekp(0, ios::beg);
	WriteMetadataAtCurrentPosition(m_dbMetadata, newDBFile);

	// Finishing for write new data to new KB
	m_dbStream.close();
	newDBFile.close();

	// Replace old KB file to newe KB file by deleting old and renaming new KB
	int perfState = -1;

	// Test for availaible rename and remove db files
	if ((perfState = rename(m_dbFileName.c_str(), (m_dbFileName + ".backup").c_str())) == 0
		&& rename(newDBfilePath.c_str(), m_dbFileName.c_str()) == 0) {
		remove((m_dbFileName + ".backup").c_str());

		// Return m_bufferedSemanticWeb to structure of basic elements
		m_bufferedToWriteSemanticWeb.Clear(); //
		m_bufferedToWriteSemanticWeb.fullByBasicElements();

		m_dbStream.open(m_dbFileName);
	}
	else {
		std::cout << "Unsucssesful unloading of data to db" << std::endl;

		if (perfState == 0
			&& rename((m_dbFileName + ".backup").c_str(), m_dbFileName.c_str()) == 0) {
			std::cout << "Old db file have restored. Please check happened situation and try unload updates to db again" << std::endl;

			m_dbStream.open(m_dbFileName);
		}
		else {
			std::cout << "Cannot be restore old db file from backup. Please will be rename {db}.backup by self" << std::endl;
		}
	}
}

void DbController::OperationVertexInBuffer(Vertex* const vertex, const OperationParametrs operation) {
	OperationVertexesInBuffer(vector<Vertex*> {vertex}, operation);
}

void DbController::WriteBufferedVertexiesAtCurrentPosition(vertexIdType &newLastWroteVertexId, size_t &newSecondStructuresStartLineIndex, fstream &dbFstream) {
	// Sorted in Operations, data for reading and writing
	vector<vertexIdType> vertexesAdd = m_bufferedToWriteSemanticWeb.GetNode(SWObjectVertexForAddToDb)->GetVertexIdsVector();
	vector<vertexIdType> vertexesChange = m_bufferedToWriteSemanticWeb.GetNode(SWObjectVertexForChangeToDb)->GetVertexIdsVector();
	vector<vertexIdType> vertexesDel = m_bufferedToWriteSemanticWeb.GetNode(SWObjectVertexForRemoveToDb)->GetVertexIdsVector();

	uint16_t endLineAfterFollowingObjects = m_dbMetadata.firstStructuresCount;
	size_t addIndex = 0, changeIndex = 0, delIndex = 0; // Universal index for vectors

	// Reading and writing Vertexies to new KB
	for (size_t lineIndex = 1; lineIndex < m_dbMetadata.secondStructuresStartLineIndex; lineIndex++) {
		// Skip first line and the rest of the character '\n'
		string line;
		getline(m_dbStream, line);

		vector<Vertex*> vertexes;

		// Getting vertexies from old KB
		if (lineIndex < m_dbMetadata.secondStructuresStartLineIndex - 1) {
			for (uint16_t index = 0; index < m_dbMetadata.firstStructuresCount; index++) {
				vertexes.push_back(ReadVertexAtCurrentPosition());
			}
		}
		else { // last line of vertexies
			if (m_dbMetadata.lastVertexWroteId > 0) { // If vertexes in KB is not empty
				vertexes.push_back(ReadVertexAtCurrentPosition()); // If only one line of vertexes

				while (vertexes.back()->GetId() < m_dbMetadata.lastVertexWroteId) {
					vertexes.push_back(ReadVertexAtCurrentPosition());
				}
			}
		}

		// Apply changes from m_bufferedSemanticWeb
		if (m_dbMetadata.lastVertexWroteId > 0) { // If vertexes in KB is not empty
			for (size_t index = 0; index < vertexes.size(); index++) {
				vertexIdType vertexId = vertexes.at(index)->GetId();

				// Add
				if (vertexesAdd.size() > 0) {
					while (vertexesAdd.at(addIndex) < vertexId || vertexId == m_dbMetadata.lastVertexWroteId) {
						Vertex* addVertex = m_bufferedToWriteSemanticWeb.GetVertex(vertexesAdd.at(addIndex))->Clone();

						if (vertexId == m_dbMetadata.lastVertexWroteId) {
							vertexes.push_back(addVertex);
						}
						else {
							vertexes.insert(vertexes.begin() + index, addVertex);
						}

						addIndex++;
						index++;
					}
				}

				// Change
				if (vertexesChange.size() > 0 && vertexesChange.at(changeIndex) == vertexId) {
					Vertex* vertexRef = vertexes.at(index);

					vertexRef->SetText(m_bufferedToWriteSemanticWeb.GetVertex(vertexRef->GetId())->GetText());

					changeIndex++;
				}

				// Delete
				if (vertexesDel.size() > 0 && vertexesDel.at(delIndex) == vertexId) {
					vertexes.erase(vertexes.begin() + index);

					index--;
					delIndex++;
				}
			}
		}
		else {
			while (addIndex < vertexesAdd.size()) {
				Vertex* addVertex = m_bufferedToWriteSemanticWeb.GetVertex(vertexesAdd.at(addIndex))->Clone();

				vertexes.push_back(addVertex);

				addIndex++;
			}
		}

		// Writing applied vertexies to new KB
		if (vertexes.size() == 0) {
			break;
		}

		std::reverse(vertexes.begin(), vertexes.end());

		// Update the most highest written vertex id
		newLastWroteVertexId = vertexes.front()->GetId() > newLastWroteVertexId
			? vertexes.front()->GetId() : newLastWroteVertexId;

		while (!vertexes.empty()) {
			if (endLineAfterFollowingObjects == 0) {
				dbFstream << "\n";

				endLineAfterFollowingObjects = m_dbMetadata.firstStructuresCount;

				newSecondStructuresStartLineIndex++;
			}

			WriteVertexAtCurrentPosition(*(vertexes.back()), dbFstream);

			delete vertexes.back();
			vertexes.pop_back();

			endLineAfterFollowingObjects--;
		}
	}
}

void DbController::WriteBufferedNodesAtCurrentPosition(vertexIdType &newLastWroteNodeId, fstream &dbFstream) {
	size_t addIndex = 0, changeIndex = 0, delIndex = 0; // Universal index for vectors

	// Sorted in Operations, data for reading and writing
	vector<vertexIdType> nodesAddIds = m_bufferedToWriteSemanticWeb.GetNode(SWObjectNodeForAddToDb)->GetVertexIdsVector();
	vector<vertexIdType> nodesDelIds = m_bufferedToWriteSemanticWeb.GetNode(SWObjectNodeForRemoveToDb)->GetVertexIdsVector();

	Node* const subNodesAddNode = m_bufferedToWriteSemanticWeb.GetNode(SWObjectSubNodeForAddToDb);
	Node* const subNodesChangeNode = m_bufferedToWriteSemanticWeb.GetNode(SWObjectSubNodeForChangeToDb);
	Node* const subNodesDelNode = m_bufferedToWriteSemanticWeb.GetNode(SWObjectSubNodeForRemoveToDb);

	uint16_t endLineAfterFollowingObjects = m_dbMetadata.secondStructuresCount;

	while (!m_dbStream.eof()) {
		string line;
		getline(m_dbStream, line);

		vector<Node*> nodes;

		// Getting Nodes from old KB
		if (m_dbMetadata.lastNodeWroteId > 0) { // If nodes in KB is not empty
			for (uint16_t index = 0; index < m_dbMetadata.secondStructuresCount; index++) {
				nodes.push_back(ReadNodeAtCurrentPosition());

				if (nodes.back()->GetNodeId() == m_dbMetadata.lastNodeWroteId)
					break;
			}
		}

		// Apply changes for Nodes from m_bufferedSemanticWeb
		if (m_dbMetadata.lastNodeWroteId > 0) { // If nodes in KB is not empty
			for (size_t index = 0; index < nodes.size(); index++) {
				vertexIdType nodeId = nodes.at(index)->GetNodeId();

				if (nodesAddIds.size() > 0) {
					while (nodesAddIds.at(addIndex) < nodeId) {
						Node* node = new Node(nodesAddIds.at(addIndex));

						nodes.insert(nodes.begin() + index, node);

						addIndex++;
						index++;
					}
				}

				if (nodesDelIds.size() > 0 && nodesDelIds.at(delIndex) == nodeId) {
					nodes.erase(nodes.begin() + index);

					delIndex++;
					index--;
				}
			}
		}
		else {
			while (addIndex < nodesAddIds.size() - 1) {
				Node* node = new Node(nodesAddIds.at(addIndex));

				nodes.push_back(node);

				addIndex++;
			}
		}

		// Apply changes for SubNodes of Nodes
		for (size_t nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++) {
			Node* node = nodes.at(nodeIndex);
			vector<SubNode> findedSubNodes;
			SemanticWeb& sw = m_bufferedToWriteSemanticWeb;
			
			if ((findedSubNodes = subNodesAddNode->FindByFirstVertexId(node->GetNodeId())).size() > 0) {
				std::for_each(findedSubNodes.begin(), findedSubNodes.end(),
					[&sw, &node](SubNode& subNode) {
						node->AddOrSetSubNode(
							sw.GetNode(node->GetNodeId())->GetSubNode(subNode.GetSecondVertexId())
						);
					});
			}

			if ((findedSubNodes = subNodesChangeNode->FindByFirstVertexId(node->GetNodeId())).size() > 0) {
				std::for_each(findedSubNodes.begin(), findedSubNodes.end(),
					[&sw, &node](SubNode& subNode) {
						node->AddOrSetSubNode(
							sw.GetNode(node->GetNodeId())->GetSubNode(subNode.GetSecondVertexId())
						);
					});
			}

			if ((findedSubNodes = subNodesDelNode->FindByFirstVertexId(node->GetNodeId())).size() > 0) {
				std::for_each(findedSubNodes.begin(), findedSubNodes.end(),
					[&node](SubNode& subNode) {
						node->RemoveSubNode(subNode.GetSecondVertexId());
					});
			}
		}

		// Writing applied nodes to new KB
		if (nodes.size() == 0) {
			break;
		}

		std::reverse(nodes.begin(), nodes.end());

		newLastWroteNodeId = nodes.front()->GetNodeId() > newLastWroteNodeId
			? nodes.front()->GetNodeId() : newLastWroteNodeId;

		while (!nodes.empty()) {
			if (endLineAfterFollowingObjects == 0) {
				dbFstream << "\n";

				endLineAfterFollowingObjects = m_dbMetadata.secondStructuresCount;
			}

			WriteNodeAtCurrentPosition(*(nodes.back()), dbFstream);

			delete nodes.back();
			nodes.pop_back();

			endLineAfterFollowingObjects--;
		}
	}
}

void DbController::OperationVertexesInBuffer(const vector<Vertex*>& vertexes, const OperationParametrs operation) {
	Node* const addNode = m_bufferedToWriteSemanticWeb.GetNode(SWObjectVertexForAddToDb);
	Node* const changeNode = m_bufferedToWriteSemanticWeb.GetNode(SWObjectVertexForChangeToDb);
	Node* const deleteNode = m_bufferedToWriteSemanticWeb.GetNode(SWObjectVertexForRemoveToDb);
	
	vector<vertexIdType> addVertexesList = addNode->GetVertexIdsVector();
	vector<vertexIdType> changeVertexesList = changeNode->GetVertexIdsVector();
	vector<vertexIdType> deleteVertexesList = deleteNode->GetVertexIdsVector();

	vector<vertexIdType> removeAddIdList, removeChangeIdList, removeDeleteIdList;
	
	for (Vertex const* const vertex : vertexes) {
		vertexIdType vertexId = vertex->GetId();

		if (find(addVertexesList.begin(), addVertexesList.end(), vertexId) != addVertexesList.end()) {
			if (operation == OperationParametrs::Adding) {
				throw "Note have wrote for adding vertex with this id earlier";
			}
			else if (operation == OperationParametrs::Changing) {
				m_bufferedToWriteSemanticWeb.GetVertex(vertexId)->SetText(vertex->GetText());
			}
			else if (operation == OperationParametrs::Deleting) {
				removeAddIdList.push_back(vertexId);

				m_bufferedToWriteSemanticWeb.RemoveVertex(vertexId);

				deleteVertexesList.push_back(vertexId);
			}
		}
		else if (find(changeVertexesList.begin(), changeVertexesList.end(), vertexId) != changeVertexesList.end()) {
			if (operation == OperationParametrs::Adding) {
				throw "Note cannot be add because it have wrote for writing vertex with this id earlier";
			}
			else if (operation == OperationParametrs::Changing) {
				m_bufferedToWriteSemanticWeb.GetVertex(vertexId)->SetText(vertex->GetText());
			}
			else if (operation == OperationParametrs::Deleting) {
				removeChangeIdList.push_back(vertexId);
				
				m_bufferedToWriteSemanticWeb.RemoveVertex(vertexId);

				deleteVertexesList.push_back(vertexId);
			}
		}
		else if (find(deleteVertexesList.begin(), deleteVertexesList.end(), vertexId) != deleteVertexesList.end()) {
			if (operation == OperationParametrs::Adding) {
				removeDeleteIdList.push_back(vertexId);
				
				m_bufferedToWriteSemanticWeb.AddOrSetVertex(vertex->Clone());

				addVertexesList.push_back(vertexId);
			}
			else if (operation == OperationParametrs::Changing) {
				throw "Note cannot be change because it have wrote for deleting vertex with this id earlier";
			}
			else if (operation == OperationParametrs::Deleting) {
				throw "Node cannot be delete because it have wrote for deleting vertex with this id earlier";
			}
		}
		else {
			if (operation == OperationParametrs::Adding) {
				m_bufferedToWriteSemanticWeb.AddOrSetVertex(vertex->Clone());

				addVertexesList.push_back(vertexId);
			}
			else if (operation == OperationParametrs::Changing) {
				m_bufferedToWriteSemanticWeb.AddOrSetVertex(vertex->Clone());

				changeVertexesList.push_back(vertexId);
			}
			else if (operation == OperationParametrs::Deleting) {
				m_bufferedToWriteSemanticWeb.RemoveVertex(vertex->GetId());

				deleteVertexesList.push_back(vertexId);
			}
		}
	}

	addNode->SetVertexIdsVector(addVertexesList);
	addNode->RemoveVertexIdsVectorId(removeAddIdList);
	addNode->SortSubNodesAsVertexIdsVector();

	changeNode->SetVertexIdsVector(changeVertexesList);
	changeNode->RemoveVertexIdsVectorId(removeChangeIdList);
	changeNode->SortSubNodesAsVertexIdsVector();

	deleteNode->SetVertexIdsVector(deleteVertexesList);
	deleteNode->RemoveVertexIdsVectorId(removeDeleteIdList);
	deleteNode->SortSubNodesAsVertexIdsVector();
}

Vertex* DbController::ReadVertex(vertexIdType vertexId) {
	Vertex* vertex = nullptr;

	vector<vertexIdType> vertexIds = { vertexId };

	vector<Vertex*> vertexes = ReadVertexes(vertexIds);

	if (vertexes.size() > 0) {
		vertex = vertexes.at(0);
	}

	return vertex;
}

vector<Vertex*> DbController::ReadVertexes(vector<vertexIdType> vertexIds) {
	vector<Vertex*> vertexes;

	if (!DbIsOpened()) {
		return vertexes;
	}
	
	// Sorting the vector as precaution
	sort(vertexIds.begin(), vertexIds.end());

	// Search requested vertexes in m_bufferedSemanticWeb
	{
		vector<vertexIdType> restVertexIds;

		for (vertexIdType vertexId : vertexIds) {
			Vertex* vertexPointer = m_bufferedToWriteSemanticWeb.GetVertex(vertexId);

			if (vertexPointer != nullptr) { // If it have found in m_bufferedSemanticWeb
				vertexes.push_back(vertexPointer);
			}
			else{ // If it have not found
				restVertexIds.push_back(vertexId);
			}
		}

		vertexIds.clear();
		for (vertexIdType vertexId : restVertexIds) {
			vertexIds.push_back(vertexId);
		}
	}

	if (vertexIds.size() == 0) {
		return vertexes;
	}

	// Search rest requested vertexes in db file
	// Set nessesary variables for file working and place the pointer at the beginning of the file
	Vertex* vertex = nullptr;
	string line;
	size_t vertexElementIndex = 0;
	vertexIdType vertexIdInNextLine = (vertexIdType)(m_dbMetadata.firstStructuresCount) + 1,
		vertexNextLineIndex = 0;
	size_t lineIndex = 1;

	m_dbStream.seekg(0, ios_base::beg);

	// Skip lines contains metadata
	getline(m_dbStream, line);

	vertexNextLineIndex = vertexIds.at(vertexElementIndex) / m_dbMetadata.firstStructuresCount;

	// Iteration by vertexIds and add vertexies to result, while file is not ended and exist element, that need be finded and readed
	while (lineIndex < m_dbMetadata.secondStructuresStartLineIndex && vertexElementIndex < vertexIds.size()) {
		// Iteration by lines
		while (vertexIdInNextLine < vertexIds.at(vertexElementIndex)) {
			std::streampos previousVertexiesLineInStartPosition = m_dbStream.tellg();

			getline(m_dbStream, line);
			lineIndex++;

			vertex = ReadVertexAtCurrentPosition();
			vertexIdInNextLine = vertex->GetId();

			if (vertexIdInNextLine > vertexIds.at(vertexElementIndex)) {
				m_dbStream.seekg(m_dbStream.tellg() - previousVertexiesLineInStartPosition, ios_base::cur);
				lineIndex--;
			}

			delete vertex;
		}

		size_t currentNumberInLine = 0;

		// While few nodes, that must readed by program, is one line
		while (vertexElementIndex < vertexIds.size() && vertexIdInNextLine > vertexIds.at(vertexElementIndex)) {
			vertex = ReadVertexAtCurrentPosition();

			// If it is needed vertex, push it to buffSW and return vector
			if (vertex->GetId() == vertexIds.at(vertexElementIndex)) {
				m_bufferedToWriteSemanticWeb.AddOrSetVertex(vertex);

				vertexes.push_back(vertex);
				vertexElementIndex++;
			}
			else {
				// If is not needed delete
				delete vertex;
			}

			if (++currentNumberInLine > m_dbMetadata.firstStructuresCount) {
				for (;
					vertexElementIndex < vertexIds.size()
					&& vertexIdInNextLine <= vertexIds.at(vertexElementIndex);
					vertexElementIndex++);
			}
		}
	}

	return vertexes;
}

vector<vertexIdType>* DbController::ReadVertexIdsWithText(string text) {
	vector<vertexIdType>* vertexIds = new vector<vertexIdType>();

	// TODO: Searching in m_bufferedSemanticWeb

	Vertex* vertex;
	vertexIdType currentVertexId = 1;
	string line;
	
	getline(m_dbStream, line);

	if (m_dbMetadata.lastVertexWroteId <= 0) {
		for (unsigned int lineIndex = 0;
			lineIndex < m_dbMetadata.secondStructuresStartLineIndex;
			lineIndex++) {
			for (vertexIdType currentIndexInLine = 0;
					currentVertexId < m_dbMetadata.lastVertexWroteId
					&& currentIndexInLine < m_dbMetadata.firstStructuresCount;
				currentIndexInLine++) {
				vertex = ReadVertexAtCurrentPosition();

				if (vertex->GetText() == text) {
					vertexIds->push_back(vertex->GetId());
				}

				delete vertex;
			}
		}
	}

	return vertexIds;
}

void DbController::OperationNodeInBuffer(Node* const node, const OperationParametrs operation) {
	OperationNodesInBuffer(vector<Node*> {node}, operation);
}

void DbController::UndoAllChangesForVertexies(const vector<vertexIdType>& vertexIds) {
	m_bufferedToWriteSemanticWeb.GetNode(SWObjectVertexForAddToDb)->RemoveVertexIdsVectorId(vertexIds);
	m_bufferedToWriteSemanticWeb.GetNode(SWObjectVertexForChangeToDb)->RemoveVertexIdsVectorId(vertexIds);
	m_bufferedToWriteSemanticWeb.GetNode(SWObjectVertexForRemoveToDb)->RemoveVertexIdsVectorId(vertexIds);

	for (const vertexIdType vertexId : vertexIds) {
		m_bufferedToWriteSemanticWeb.RemoveVertex(vertexId);
	}
}

void DbController::OperationNodesInBuffer(const vector<Node*>& nodes, const OperationParametrs operation) {
	vector<vertexIdType> addNodeIdList = 
		m_bufferedToWriteSemanticWeb.GetNode(SWObjectNodeForAddToDb)->GetVertexIdsVector();
	vector<vertexIdType> deleteNodeIdList = 
		m_bufferedToWriteSemanticWeb.GetNode(SWObjectNodeForRemoveToDb)->GetVertexIdsVector();

	vector<vertexIdType> removeAddIdList, removeDeleteIdList;

	for (const Node* node : nodes) {
		vertexIdType nodeId = node->GetNodeId();

		if (find(addNodeIdList.begin(), addNodeIdList.end(), nodeId) != addNodeIdList.end()) {
			if (operation == OperationParametrs::Adding) {
				throw "Note have wrote for adding vertex with this id earlier";
			}
			else if (operation == OperationParametrs::Changing) {
				vector<SubNode>* subNodesBeforeCh = m_bufferedToWriteSemanticWeb.GetNode(nodeId)->GetCloneSubNodes();
				vector<SubNode>* subNodesAfterCh = node->GetCloneSubNodes();

				for (size_t index = 0; index < subNodesBeforeCh->size(); index++) {
					OperationSubNodesInBuffer(nodeId, *subNodesBeforeCh, OperationParametrs::Deleting);
				}

				for (size_t index = 0; index < subNodesAfterCh->size(); index) {
					OperationSubNodesInBuffer(nodeId, *subNodesAfterCh, OperationParametrs::Adding);
				}

				delete subNodesBeforeCh;
				delete subNodesAfterCh;
			}
			else if (operation == OperationParametrs::Deleting) {
				removeAddIdList.push_back(nodeId);

				OperationSubNodesInBuffer(nodeId,
					*m_bufferedToWriteSemanticWeb.GetNode(nodeId)->GetSubNodes(),
						OperationParametrs::Deleting);

				m_bufferedToWriteSemanticWeb.RemoveNode(nodeId);

				deleteNodeIdList.push_back(nodeId);
			}
		}
		else if (find(deleteNodeIdList.begin(), deleteNodeIdList.end(), nodeId) != deleteNodeIdList.end()) {
			if (operation == OperationParametrs::Adding) {
				removeDeleteIdList.push_back(nodeId);

				vector<SubNode>* subNodes = node->GetCloneSubNodes();

				m_bufferedToWriteSemanticWeb.AddOrSetNode(new Node(nodeId));

				addNodeIdList.push_back(nodeId);

				OperationSubNodesInBuffer(nodeId, *subNodes, OperationParametrs::Adding);

				delete subNodes;
			}
			else if (operation == OperationParametrs::Changing) {
				throw "You can not change Node, because there is not nothing to change";
			}
			else if (operation == OperationParametrs::Deleting) {
				throw "Node cannot be delete because it have wrote for deleting vertex with this id earlier";
			}
		}
		else if(operation == OperationParametrs::Adding) {
			vector<SubNode>* subNodes = node->GetCloneSubNodes();
			
			m_bufferedToWriteSemanticWeb.AddOrSetNode(new Node(nodeId));

			addNodeIdList.push_back(nodeId);

			OperationSubNodesInBuffer(nodeId, *subNodes, OperationParametrs::Adding);

			delete subNodes;
		}
	}

	Node* addNode = m_bufferedToWriteSemanticWeb.GetNode(SWObjectNodeForAddToDb);
	Node* deleteNode = m_bufferedToWriteSemanticWeb.GetNode(SWObjectNodeForRemoveToDb);

	addNode->SetVertexIdsVector(addNodeIdList);
	addNode->RemoveVertexIdsVectorId(removeAddIdList);
	addNode->SortSubNodesAsVertexIdsVector();

	deleteNode->SetVertexIdsVector(deleteNodeIdList);
	deleteNode->RemoveVertexIdsVectorId(removeDeleteIdList);
	deleteNode->SortSubNodesAsVertexIdsVector();
}

Node* DbController::ReadNode(const vertexIdType nodeId) {
	Node* node = nullptr;

	vector<vertexIdType> nodeIds = { nodeId };

	vector<Node*> nodes = ReadNodes(nodeIds);

	if (nodes.size() > 0) {
		node = nodes.at(0);
	}

	return node;
}

vector<Node*> DbController::ReadNodes(vector<vertexIdType> nodeIds) {
	vector<Node*> nodes;

	if (!DbIsOpened() || nodeIds.size() == 0) {
		return nodes;
	}

	sort(nodes.begin(), nodes.end());

	// Search requested vertexes in m_bufferedSemanticWeb
	{
		vector<vertexIdType> restNodeIds;

		for (auto nodeId : nodeIds) {
			Node* nodePointer = m_bufferedToWriteSemanticWeb.GetNode(nodeId);

			if (nodePointer != nullptr) { // If it have found in m_bufferedSemanticWeb
				nodes.push_back(nodePointer);
			}
			else { // If it have not found
				restNodeIds.push_back(nodeId);
			}
		}

		nodeIds.clear();
		for (auto vertexId : restNodeIds) {
			nodeIds.push_back(vertexId);
		}
	}

	// Search rest requested vertexes in db file
	// Set nessesary variables for file working and place the pointer at the beginning of the file
	Node* node = nullptr;
	string line;
	size_t lineIndex = 0, nodeElementIndex = 0;
	vertexIdType nodeIdInNextLine = (vertexIdType)(m_dbMetadata.secondStructuresCount) + 1;

	m_dbStream.seekg(0, ios_base::beg);

	while (lineIndex < m_dbMetadata.secondStructuresStartLineIndex) {
		getline(m_dbStream, line);

		lineIndex++;
	}

	// Sorting the vector as precaution
	sort(nodeIds.begin(), nodeIds.end());

	while (!m_dbStream.eof() && nodeElementIndex < nodeIds.size()) {
		// Iteration by lines
		while (nodeIdInNextLine < nodeIds.at(nodeElementIndex)) {
			std::streampos previousNodesLineInStartPosition = m_dbStream.tellg();

			getline(m_dbStream, line);

			node = ReadNodeAtCurrentPosition();
			nodeIdInNextLine = node->GetNodeId();

			if (nodeIdInNextLine > nodeIds.at(nodeElementIndex)) {
				m_dbStream.seekg(m_dbStream.tellg() - previousNodesLineInStartPosition, ios_base::cur);
			}

			delete node;
		}

		// Iteration for elements in line
		size_t currentIndexInLine = 0;

		while (nodeElementIndex < nodeIds.size() && nodeIdInNextLine > nodeIds.at(nodeElementIndex)) {
			node = ReadNodeAtCurrentPosition();

			if (node->GetNodeId() == nodeIds.at(nodeElementIndex)) {
				m_bufferedToWriteSemanticWeb.AddOrSetNode(node);

				nodes.push_back(node);
				nodeElementIndex++;
			}
			else {
				delete node;
			}

			if (++currentIndexInLine > m_dbMetadata.secondStructuresCount) {
				for (;
					nodeElementIndex < nodeIds.size()
					&& nodeIdInNextLine <= nodeIds.at(nodeElementIndex);
					nodeElementIndex++);
			}
		}
	}

	return nodes;
}

void DbController::UndoAllChangesForNodes(const vector<vertexIdType>& nodeIds) {
	m_bufferedToWriteSemanticWeb.GetNode(SWObjectNodeForAddToDb)->RemoveVertexIdsVectorId(nodeIds);
	m_bufferedToWriteSemanticWeb.GetNode(SWObjectNodeForRemoveToDb)->RemoveVertexIdsVectorId(nodeIds);

	for (vertexIdType nodeId : nodeIds) {
		UndoAllChangesForNode(nodeId);
	}
}


vector<vertexIdType> DbController::ReadNodeIdsWithVertexIds(vector<vertexIdType>& vertexIds) {
	vector<vertexIdType> nodeIds;
	
	if (!DbIsOpened()) {
		return nodeIds;
	}

	sort(vertexIds.begin(), vertexIds.end());

	// Searching nodes contains someone vertexId from vertexIds in m_bufferedSemanticWeb
	{
		vector<Node*>* buffNodes = m_bufferedToWriteSemanticWeb.GetNodes();

		for (Node* node : *buffNodes) {
			vector<SubNode> foundSubNodes = node->FindByVertexIdsAnyWhere(vertexIds);

			if (foundSubNodes.size() > 0) {
				nodeIds.push_back(node->GetNodeId());
			}
		}
	}

	// Searching nodes contains someone vertexId from vertexIds in db file
	Node* node = nullptr;
	vertexIdType currentNodeId = 1;
	size_t lineIndex = 0;
	string line;

	m_dbStream.seekg(0, ios_base::beg);

	while (lineIndex < m_dbMetadata.secondStructuresStartLineIndex) {
		getline(m_dbStream, line);

		lineIndex++;
	}

	if (m_dbMetadata.lastNodeWroteId <= 0) {
		while (!m_dbStream.eof()) {
			for (size_t currentIndexInLine = 0;
				currentNodeId < m_dbMetadata.lastNodeWroteId
				&& currentIndexInLine < m_dbMetadata.secondStructuresCount;
				currentIndexInLine++) {
				node = ReadNodeAtCurrentPosition();

				vector<SubNode> foundSubNodesInDb = node->FindByVertexIdsAnyWhere(vertexIds);

				if (foundSubNodesInDb.size() > 0) {
					nodeIds.push_back(node->GetNodeId());
				}

				delete node;
			}
		}
	}

	return nodeIds;
}

void DbController::OperationSubNodesInBuffer(const vertexIdType nodeId, vector<SubNode>& subNodes, const OperationParametrs operation) {
	bool isListedNode = false;

	{
		vector<vertexIdType> listNodes = m_bufferedToWriteSemanticWeb.GetNode(SWObjectDataListFormat)->GetVertexIdsVector();

		if (find(listNodes.begin(), listNodes.end(), nodeId) != listNodes.end()) {
			isListedNode = true;
		}
	}

	Node* const addNode = m_bufferedToWriteSemanticWeb.GetNode(SWObjectSubNodeForAddToDb);
	Node* const changeNode = m_bufferedToWriteSemanticWeb.GetNode(SWObjectSubNodeForChangeToDb);
	Node* const deleteNode = m_bufferedToWriteSemanticWeb.GetNode(SWObjectSubNodeForRemoveToDb);

	if (isListedNode) {
		// If node have many vertex's id as list
		Node* node = ReadNode(nodeId);
		size_t sizeBeforeOperation = node->GetSubNodes()->size() * 2;

		{
			vector<vertexIdType> subNodeIdsList;

			if (operation == OperationParametrs::Adding) {
				subNodeIdsList = node->GetVertexIdsVector();

				std::for_each(subNodes.begin(), subNodes.end(), [&subNodeIdsList](SubNode& subNode) {
					subNodeIdsList.push_back(subNode.GetFirstVertexId());
					subNodeIdsList.push_back(subNode.GetSecondVertexId());
					});

				node->SetVertexIdsVector(subNodeIdsList);
			}
			else if (operation == OperationParametrs::Changing) {
				throw "SubNodes of list node type cannot be changed by SubNode. Only add or remove vertex's ids";
			}
			else if (operation == OperationParametrs::Deleting) {
				std::for_each(subNodes.begin(), subNodes.end(), [&subNodeIdsList](SubNode& subNode) {
					subNodeIdsList.push_back(subNode.GetFirstVertexId());
					subNodeIdsList.push_back(subNode.GetSecondVertexId());
					});

				node->RemoveVertexIdsVectorId(subNodeIdsList);
			}
		}

		node->SortSubNodesAsVertexIdsVector();

		size_t sizeAfterOperation = node->GetSubNodes()->size() * 2;
			
		if (sizeAfterOperation > sizeBeforeOperation) {
			for (size_t index = 0; index < sizeBeforeOperation; index++) {
				changeNode->AddOrSetSubNode(SubNode(nodeId, index + 1, 255));
			}
			for (size_t index = sizeBeforeOperation; index < sizeAfterOperation; index++) {
				addNode->AddOrSetSubNode(SubNode(nodeId, index + 1, 255));
			}
		}
		else if(sizeAfterOperation < sizeBeforeOperation) {
			for (size_t index = 0; index < sizeAfterOperation; index++) {
				changeNode->AddOrSetSubNode(SubNode(nodeId, index+1, 255));
			}
			for (size_t index = sizeAfterOperation; index < sizeBeforeOperation; index++) {
				deleteNode->AddOrSetSubNode(SubNode(nodeId, index+1, 255));
			}
		}
		else if (sizeAfterOperation == sizeBeforeOperation) {
			for (size_t index = 0; index < sizeAfterOperation; index++) {
				changeNode->AddOrSetSubNode(SubNode(nodeId, index, 255));
			}
		}
	}
	else {
		// If node have pair of two vertex
		for (SubNode& subNode : subNodes) {
			vector<SubNode> findedSubNodes;
			vertexIdType subNodeId = subNode.GetId();
			bool operated = true;

			if ((findedSubNodes = addNode->FindByBothVertexIds(nodeId, subNodeId)).size() > 0) {
				if (operation == OperationParametrs::Adding) {
					throw "Cannot add added subNode";
				}
				else if (operation == OperationParametrs::Changing) {
					m_bufferedToWriteSemanticWeb.GetNode(nodeId)->AddOrSetSubNode(subNode);
				}
				else if (operation == OperationParametrs::Deleting) {
					m_bufferedToWriteSemanticWeb.GetNode(nodeId)->RemoveSubNode(subNodeId);

					vertexIdType subNodeAddId =  findedSubNodes.at(0).GetId();

					addNode->RemoveSubNode(subNodeAddId);
				}
			}
			else if ((findedSubNodes = changeNode->FindByBothVertexIds(nodeId, subNodeId)).size() > 0) {
				if (operation == OperationParametrs::Adding) {
					throw "Cannot be added exist SubNode";
				}
				else if (operation == OperationParametrs::Changing) {
					m_bufferedToWriteSemanticWeb.GetNode(nodeId)->AddOrSetSubNode(subNode);
				}
				else if (operation == OperationParametrs::Deleting) {
					m_bufferedToWriteSemanticWeb.GetNode(nodeId)->RemoveSubNode(subNodeId);

					changeNode->RemoveSubNode(findedSubNodes.at(0).GetId());

					deleteNode->AddOrSetSubNode(SubNode(nodeId, subNodeId, 255));
				}
			}
			else if ((findedSubNodes = deleteNode->FindByBothVertexIds(nodeId, subNodeId)).size() > 0) {
				if (operation == OperationParametrs::Adding) {
					deleteNode->RemoveSubNode(findedSubNodes.at(0).GetId());

					addNode->AddOrSetSubNode(SubNode(nodeId, subNodeId, 255));
					m_bufferedToWriteSemanticWeb.GetNode(nodeId)->AddOrSetSubNode(subNode);
				}
				else if (operation == OperationParametrs::Changing) {
					throw "Cannot be change deleted SubNode";
				}
				else if (operation == OperationParametrs::Deleting) {
					throw "Cannot be delete deleted SubNode";
				}
			}
			else {
				operated = false;
			}

			if (!operated) {
				if (operation == OperationParametrs::Adding) {
					addNode->AddOrSetSubNode(SubNode(nodeId, subNodeId, 255));
					m_bufferedToWriteSemanticWeb.GetNode(nodeId)->AddOrSetSubNode(subNode);
				}
				else if (operation == OperationParametrs::Changing) {
					changeNode->AddOrSetSubNode(SubNode(nodeId, subNodeId, 255));
					m_bufferedToWriteSemanticWeb.GetNode(nodeId)->AddOrSetSubNode(subNode);
				}
				else if (operation == OperationParametrs::Deleting) {
					deleteNode->AddOrSetSubNode(SubNode(nodeId, subNodeId, 255));
					m_bufferedToWriteSemanticWeb.GetNode(nodeId)->AddOrSetSubNode(subNode);
				}
			}
		}
	}
}

void DbController::UndoAllChangesForNode(const vertexIdType nodeId) {
	// TODO: Make
}

void DbController::UndoAllChangesForSubNode(const vertexIdType nodeId, const vector<vertexIdType>& subNodeIds) {
	// TODO: Make
}


// Private methods

void DbController::ClearNonWritableElements() {
	Node* checkingNode = nullptr;
	vector<vertexIdType> removeElementsIdList;

	// Delete unchangeable ids in add, change, delete vectors of vertexes
	vector<vertexIdType> nonChangibleElementsList = 
		m_bufferedToWriteSemanticWeb.GetNode(SWObjectUnchangeableVertex)->GetVertexIdsVector();
	
	for (vertexIdType nodeId : {SWObjectVertexForAddToDb,
									SWObjectVertexForChangeToDb, SWObjectVertexForRemoveToDb}) {
		checkingNode = m_bufferedToWriteSemanticWeb.GetNode(nodeId);
		const vector<vertexIdType> operationElementsList = checkingNode->GetVertexIdsVector();

		for (vertexIdType id : nonChangibleElementsList) {
			if (find(operationElementsList.begin(), operationElementsList.end(), id)
				!= operationElementsList.end()) {
				removeElementsIdList.push_back(id);
			}
		}

		checkingNode->RemoveVertexIdsVectorId(removeElementsIdList);
		removeElementsIdList.clear();
	}

	// Delete unchangeable ids in add, change, delete vectors of nodes
	nonChangibleElementsList = m_bufferedToWriteSemanticWeb.GetNode(SWObjectUnchangeableNode)->GetVertexIdsVector();
	
	for (vertexIdType nodeId : {SWObjectNodeForAddToDb, SWObjectNodeForRemoveToDb}) {
		checkingNode = m_bufferedToWriteSemanticWeb.GetNode(nodeId);
		const vector<vertexIdType> operationElementsList = checkingNode->GetVertexIdsVector();

		for (vertexIdType id : nonChangibleElementsList) {
			if (find(operationElementsList.begin(), operationElementsList.end(), id)
				!= operationElementsList.end()) {
				removeElementsIdList.push_back(id);
			}
		}

		checkingNode->RemoveVertexIdsVectorId(removeElementsIdList);
		removeElementsIdList.clear();
	}
}


void DbController::ReadMetadataAtCurrentPosition() {
	// Place the pointer at the beginning of the file
	m_dbStream.seekg(0, ios_base::beg);

	uint16_t firstStructCount = 0;
	uint16_t secondStructCount = 0;
	size_t secStructsStartLineIndex = 0;
	vertexIdType lastVertexWroteId = 0;
	vertexIdType lastNodeWroteId = 0;
	
	m_dbStream.read((char*)&firstStructCount, sizeof(firstStructCount));
	m_dbStream.read((char*)&secondStructCount, sizeof(secondStructCount));
	m_dbStream.read((char*)&secStructsStartLineIndex, sizeof(secStructsStartLineIndex));
	m_dbStream.read((char*)&lastVertexWroteId, sizeof(lastVertexWroteId));
	m_dbStream.read((char*)&lastNodeWroteId, sizeof(lastNodeWroteId));

	m_dbMetadata.firstStructuresCount = firstStructCount;
	m_dbMetadata.secondStructuresCount = secondStructCount;
	m_dbMetadata.secondStructuresStartLineIndex = secStructsStartLineIndex;
	m_dbMetadata.lastVertexWroteId = lastVertexWroteId;
	m_dbMetadata.lastNodeWroteId = lastNodeWroteId;
}

Vertex* DbController::ReadVertexAtCurrentPosition() {
	Vertex* vertex = new Vertex();

	vertexIdType vertexId = 0;
	string text;

	m_dbStream.read((char*)&vertexId, sizeof(vertexId));
	std::getline(m_dbStream, text, '\0');

	vertex->SetId(vertexId);
	vertex->SetText(text);

	return vertex;
}

Node* DbController::ReadNodeAtCurrentPosition() {
	Node* node = new Node();
	
	vector<SubNode>* subNodes = new vector<SubNode>();
	SubNode subNode;
	vertexIdType nodeId = 0;

	m_dbStream.read((char*)&nodeId, sizeof(nodeId));
	node->SetNodeId(nodeId);

	vertexIdType subNodeId = 0, firstVertexId = 0, secondVertexId = 0;
	uint8_t themOwn = 0;

	m_dbStream.read((char*)&subNodeId, sizeof(subNodeId));

	while (subNodeId > 0) {
		m_dbStream.read((char*)&firstVertexId, sizeof(firstVertexId));
		m_dbStream.read((char*)&secondVertexId, sizeof(secondVertexId));
		m_dbStream.read((char*)&themOwn, sizeof(themOwn));

		subNodes->push_back(SubNode(subNodeId, firstVertexId, secondVertexId, themOwn));

		m_dbStream.read((char*)&subNodeId, sizeof(subNodeId));
	}

	node->ReplaceSubNodes(subNodes);

	return node;
}


void DbController::WriteMetadataAtCurrentPosition(DbMetadata& dbMetadata, fstream& dbFstream) {
	dbFstream.seekp(0, ios_base::beg);

	dbFstream.write((char*)&dbMetadata.firstStructuresCount, sizeof(dbMetadata.firstStructuresCount));
	dbFstream.write((char*)&dbMetadata.secondStructuresCount, sizeof(dbMetadata.secondStructuresCount));
	dbFstream.write((char*)&dbMetadata.secondStructuresStartLineIndex, sizeof(dbMetadata.secondStructuresStartLineIndex));
	dbFstream.write((char*)&dbMetadata.lastVertexWroteId, sizeof(dbMetadata.lastVertexWroteId));
	dbFstream.write((char*)&dbMetadata.lastNodeWroteId, sizeof(dbMetadata.lastNodeWroteId));
}

void DbController::WriteVertexAtCurrentPosition(const Vertex &vertex, fstream &dbFstream) {
	vertexIdType vertexId = vertex.GetId();
	string text = vertex.GetText();

	dbFstream.write((char*)&vertexId, sizeof(vertexId));

	dbFstream << text.c_str();
}

void DbController::WriteNodeAtCurrentPosition(const Node &node, fstream &dbFstream) {
	vertexIdType nodeId = node.GetNodeId();

	dbFstream.write((char*)&nodeId, sizeof(nodeId));

	auto subNodes = node.GetSubNodes();

	for (size_t index = 0; index < subNodes->size(); index++) {
		WriteSubNodeAtCurrentPosition(subNodes->at(index), dbFstream);
	}

	vertexIdType subNodesEnd = 0;

	dbFstream.write((char*)&subNodesEnd, sizeof(subNodesEnd));
}

void DbController::WriteSubNodeAtCurrentPosition(const SubNode &subNode, fstream &dbFstream) {
	vertexIdType subNodeId = subNode.GetId(), firstVerId = subNode.GetFirstVertexId(),
		seconVerId = subNode.GetSecondVertexId();
	uint8_t own = subNode.GetOwn();

	dbFstream.write((char*)&subNodeId, sizeof(subNodeId));
	dbFstream.write((char*)&firstVerId, sizeof(firstVerId));
	dbFstream.write((char*)&seconVerId, sizeof(seconVerId));
	dbFstream.write((char*)&own, sizeof(own));
}
