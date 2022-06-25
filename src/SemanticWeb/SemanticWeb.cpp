#include "SemanticWeb.h"

vertexIdType SemanticWeb::GetLastBasicElementsId() const {
	return LastBasicElementsId;
}


vector<Vertex*>* SemanticWeb::GetVertexes() const {
	return m_vertexies;
}

void SemanticWeb::SetVertexes(vector<Vertex*>* vertexes) {
	m_vertexies = vertexes;
}

void SemanticWeb::ReplaceVertexes(vector<Vertex*>* vertexes) {
	for (auto iterator = m_vertexies->begin(); iterator != m_vertexies->end(); iterator++) {
		delete (*iterator);
	}

	delete m_vertexies;

	SetVertexes(vertexes);
}


vector<Node*>* SemanticWeb::GetNodes() const {
	return m_nodes;
}

void SemanticWeb::SetNodes(vector<Node*>* nodes) {
	m_nodes = nodes;
}

void SemanticWeb::ReplaceNodes(vector<Node*>* nodes) {
	for (auto iterator = m_nodes->begin(); iterator != m_nodes->end(); iterator++) {
		delete (*iterator);
	}

	delete m_nodes;

	SetNodes(nodes);
}


void SemanticWeb::Clear() {
	for (auto iterator = m_vertexies->begin(); iterator != m_vertexies->end(); iterator++) {
		delete (*iterator);
	}
	m_vertexies->clear();

	for (auto iterator = m_nodes->begin(); iterator != m_nodes->end(); iterator++) {
		delete (*iterator);
	}
	m_nodes->clear();
}


void SemanticWeb::AddOrSetVertex(Vertex* addVertex) {
	// If need next last vertex's id
	if (addVertex->GetId() == 0) {
		if (m_vertexies->size() > 0) { // If vertexies is not empty
			addVertex->SetId(m_vertexies->back()->GetId() + 1);
		}
		else { // If vertexies is empty
			addVertex->SetId(1);
		}
	}

	// If m_subNodes is empty
	if (m_vertexies->size() == 0) {
		m_vertexies->push_back(addVertex);

		return;
	}

	for (auto iterator = m_vertexies->begin(); iterator != m_vertexies->end(); iterator++) {
		if ((*iterator)->GetId() == addVertex->GetId()) { // If it replace parametrs of exists Vertex
			delete (*iterator);

			(*iterator) = addVertex;

			return;
		}
		else if ((*iterator)->GetId() > addVertex->GetId()) {
			m_vertexies->insert(iterator, addVertex);

			return;
		}
	}
	
	// If id is next last vertex's id
	m_vertexies->push_back(addVertex);
}

Vertex* SemanticWeb::GetVertex(vertexIdType id) const {
	for (auto iterator = m_vertexies->begin();
		iterator != m_vertexies->end() && (*iterator)->GetId() <= id;
		iterator++) {
		if ((*iterator)->GetId() == id) {
			return (*iterator);
		}
	}

	return nullptr;
}

void SemanticWeb::RemoveVertex(const vertexIdType id) {
	for (auto iterator = m_vertexies->begin();
		iterator != m_vertexies->end() && (*iterator)->GetId() <= id;
		iterator++) {
		if ((*iterator)->GetId() == id) {
			delete (*iterator);

			m_vertexies->erase(iterator);

			return;
		}
	}
}


void SemanticWeb::AddOrSetNode(Node* addNode) {
	// If need next last node's id
	if (addNode->GetNodeId() <= 0) {
		if (m_nodes->size() > 0) { // If m_nodes is not empty
			addNode->SetNodeId(m_nodes->back()->GetNodeId() + 1);
		}
		else { // If m_nodes is empty
			addNode->SetNodeId(1);
		}
	}

	// If m_nodes is empty
	if (m_nodes->size() == 0) {
		m_nodes->push_back(addNode);

		return;
	}

	for (auto iterator = m_nodes->begin(); iterator < m_nodes->end(); iterator++) {
		if ((*iterator)->GetNodeId() == addNode->GetNodeId()) {
			delete (*iterator);

			(*iterator) = addNode;

			return;
		}
		else if ((*iterator)->GetNodeId() > addNode->GetNodeId()) {
			m_nodes->insert(iterator, addNode);

			return;
		}
	}

	// If id is next last nodes's id
	m_nodes->push_back(addNode);
}

Node* SemanticWeb::GetNode(vertexIdType nodeId) const {
	for (auto iterator = m_nodes->begin();
		iterator != m_nodes->end() && (*iterator)->GetNodeId() <= nodeId;
		iterator++) {
		if ((*iterator)->GetNodeId() == nodeId) {
			return (*iterator);
		}
	}

	return nullptr;
}

void SemanticWeb::RemoveNode(vertexIdType nodeId) {
	for (auto iterator = m_nodes->begin();
		iterator != m_nodes->end() && (*iterator)->GetNodeId() <= nodeId;
		iterator++) {
		if ((*iterator)->GetNodeId() == nodeId) {
			delete (*iterator);

			m_nodes->erase(iterator);

			return;
		}
	}
}


vector<Vertex*>* SemanticWeb::FindVertexEqualText(string text) const {
	vector<Vertex*>* vertexes = new vector<Vertex*>;

	for (auto iterator = m_vertexies->begin(); iterator != m_vertexies->end(); iterator++) {
		if ((*iterator)->GetText().compare(text) == 0) {
			vertexes->push_back(*iterator);
		}
	}

	return vertexes;
}

vector<Vertex*>* SemanticWeb::FindVertexLikeText(string text) const {
	vector<Vertex*>* vertexes = new vector<Vertex*>;

	for (auto iterator = m_vertexies->begin(); iterator != m_vertexies->end(); iterator++) {
		string iterText = (*iterator)->GetText();

		const size_t textLenght = text.length();
		const size_t maxPosition = iterText.length() - textLenght;
		
		for (size_t position = 0; position < maxPosition; position++) {
			if (iterText.substr(position, textLenght) == text) {
				vertexes->push_back(*iterator);

				continue;
			}
		}
	}

	return vertexes;
}


void SemanticWeb::fullByBasicElements() {
	// Elements of Semantic Web
	m_vertexies->push_back(new Vertex(SWObjectNodeId, "Признаки"));
	m_nodes->push_back(new Node(SWObjectNodeId));

	m_vertexies->push_back(new Vertex(SWObjectDataListFormat, "Списочный формат данных"));
	m_nodes->push_back(new Node(SWObjectDataListFormat));

	m_vertexies->push_back(new Vertex(SWObjectInformationalCode, "Информационные коды"));
	m_nodes->push_back(new Node(SWObjectInformationalCode));
	
	m_vertexies->push_back(new Vertex(SWObjectInheritedVertexes, "Наследуемые вершины"));
	m_nodes->push_back(new Node(SWObjectInheritedVertexes));

	m_vertexies->push_back(new Vertex(SWObjectNodeCategor, "Категории признаков"));
	m_nodes->push_back(new Node(SWObjectNodeCategor));




	// Elements of DbController
	m_vertexies->push_back(new Vertex(SWObjectUnchangeableVertex, "Неизменяемые вершины"));
	m_nodes->push_back(new Node(SWObjectUnchangeableVertex));

	m_vertexies->push_back(new Vertex(SWObjectUnchangeableNode, "Неизменяемые признаки"));
	m_nodes->push_back(new Node(SWObjectUnchangeableNode));
	
	m_vertexies->push_back(new Vertex(SWObjectFreeVertexIds
		, "Id свободных вершин до последней записанной id вершины в БЗ"));
	m_nodes->push_back(new Node(SWObjectFreeVertexIds));

	m_vertexies->push_back(new Vertex(SWObjectVertexForAddToDb, "Вершины на добавление в БЗ"));
	m_nodes->push_back(new Node(SWObjectVertexForAddToDb));

	m_vertexies->push_back(new Vertex(SWObjectNodeForAddToDb, "Узлы на добавление в БЗ"));
	m_nodes->push_back(new Node(SWObjectNodeForAddToDb));

	m_vertexies->push_back(new Vertex(SWObjectSubNodeForAddToDb, "Подузлы на добавление в БЗ"));
	m_nodes->push_back(new Node(SWObjectSubNodeForAddToDb));

	m_vertexies->push_back(new Vertex(SWObjectVertexForChangeToDb, "Вершины на изменение в БЗ"));
	m_nodes->push_back(new Node(SWObjectVertexForChangeToDb));

	m_vertexies->push_back(new Vertex(SWObjectSubNodeForChangeToDb, "Подузлы на изменение в БЗ"));
	m_nodes->push_back(new Node(SWObjectSubNodeForChangeToDb));

	m_vertexies->push_back(new Vertex(SWObjectVertexForRemoveToDb, "Вершины на удаление из БЗ"));
	m_nodes->push_back(new Node(SWObjectVertexForRemoveToDb));

	m_vertexies->push_back(new Vertex(SWObjectNodeForRemoveToDb, "Узел на удаление из БЗ"));
	m_nodes->push_back(new Node(SWObjectNodeForRemoveToDb));

	m_vertexies->push_back(new Vertex(SWObjectSubNodeForRemoveToDb, "Подузел узла на удаление из БЗ"));
	m_nodes->push_back(new Node(SWObjectSubNodeForRemoveToDb));




	// Elements of IIS
	m_vertexies->push_back(new Vertex(SWObjectProgramOfModules, "Программные модули"));
	m_nodes->push_back(new Node(SWObjectProgramOfModules));

	m_vertexies->push_back(new Vertex(SWObjectModulesName, "Названия модулей"));
	m_nodes->push_back(new Node(SWObjectModulesName));

	m_vertexies->push_back(new Vertex(SWObjectModuleIdVertexList, "Идентификаторы модулей"));
	m_nodes->push_back(new Node(SWObjectModuleIdVertexList));

	m_vertexies->push_back(new Vertex(SWObjectInputUserRequest, "Входной запрос пользователя"));
	
	m_vertexies->push_back(new Vertex(SWObjectAdditionalToUserRequest, "Дополнение к запросу пользователя"));

	m_vertexies->push_back(new Vertex(SWObjectOutputSystemResponse, "Выходной ответ системы"));

	/// Reserver module for code support reserved method
	//// Main module
	m_vertexies->push_back(new Vertex(SWObjectMainModuleNameId, MainModuleNameInSW));
	m_vertexies->push_back(new Vertex(SWObjectMainModuleTextId,
		"using \"" ModuleManageNameInSW "\"\n"
		"using \"" IISModuleNameInSW "\"\n"
		"using \"" NetModuleNameInSW "\"\n"
		"using \"" UserControlNameInSW "\"\n\n"
		"void main() {\n"
		"	bool isRun = true;\n\n"
		"	while(isRun) {\n"
		"		tree jsonPt = GetTreeFromStringJSONFormat(WaitStringFromClient());\n"
		"		ulong requestType = GetULongByKeyFromTree(jsonPt, \"RequestType\");\n\n"
		"		// Define request goal \n"
		"		if(requestType == 0) {\n"
		"			isRun = false;\n"
		"		}\n"
		"		else if(requestType == 1 && isAuthorized()) { // Run interpret additional text as module\n"
		"			CreateModule(5, \"Temp\", GetStringJSONFormatFromTree(jsonPt, \"CodeForInterpret\"));\n\n"
		"			UsingModuleWithId(GetModuleId(\"CodeForInterpret\"));\n\n"
		"			TempMain(); // run imterpret temp module main method\n\n"
		"			DisuseModuleWithId(GetModuleId(\"CodeForInterpret\"));\n\n"
		"			RemoveModule(5);\n"
		"		}\n"
		"		else if(requestType >= 100 && requestType <= 200) { // For analyze request codes\n"
		"			if(requestType == 100) { // Get import profiles\n"
		"				ulong calcModuleId = GetModuleId(\"" CalculateDemographyNameInSW "\");\n\n"
		"				UsingModuleWithId(calcModuleId);\n\n"
		"				SendImportProfiles();\n\n"
		"				DisuseModuleWithId(calcModuleId);\n"
		"			}\n"
		"			else if(requestType == 110) { // Get selected data presentations\n"
		"				ulong calcModuleId = GetModuleId(\"" CalculateDemographyNameInSW "\");\n"
		"				UsingModuleWithId(calcModuleId);\n\n"
		"				SendSelectedDataPresentations(jsonPt);\n\n"
		"				DisuseModuleWithId(calcModuleId);\n"
		"			}\n"
		"			else if(requestType == 120) { // Get analyze types\n"
		"				ulong calcModuleId = GetModuleId(\"" CalculateDemographyNameInSW "\");\n"
		"				UsingModuleWithId(calcModuleId);\n\n"
		"				SendAnalyzeTypes(jsonPt);\n\n"
		"				DisuseModuleWithId(calcModuleId);\n"
		"			}\n"
		"			else if(requestType == 150) { // For calculate by selected data presentation and analyze type\n"
		"				ulong calcModuleId = GetModuleId(\"" CalculateDemographyNameInSW "\");\n\n"
		"				UsingModuleWithId(calcModuleId);\n\n"
		"				AnalyzeAndSendResponce(jsonPt);\n\n"
		"				DisuseModuleWithId(calcModuleId);\n"
		"			}\n"
		"			else {\n"
		"				tree responce;\n\n"
		"				AddULongWithKeyToTree(responce, \"ResponceType\", 200);\n"
		"				AddStringWtihKeyToTree(responce, \"ErrorMessage\", \"Undefined request code in server\");\n\n"
		"				SendStringToClient(GetStringJSONFormatFromTree(responce));\n"
		"			}\n"
		"		}\n"
		"		else {\n"
		"			tree responce;\n\n"
		"			if(requestType == 4 && isAuthorized()) { // Get module names\n"
		"				string[]  moduleNames = GetModuleNames();\n"
		"				ulong moduleNamesCount = len(moduleNames);\n"
		"				tree moduleNamesTree;\n\n"
		"				for(ulong index = 0; index < moduleNamesCount; index = index + 1;) {\n"
		"					AddStringToTreeArray(moduleNamesTree, moduleNames[index]);\n"
		"				}\n\n"
		"				AddULongWithKeyToTree(responce, \"ResponceType\", 4);\n"
		"				AddChildWithKeyToTree(responce, \"ModuleNames\", moduleNamesTree);\n"
		"			}\n"
		"			else if(requestType == 5 && isAuthorized()) { // Get module text with name\n"
		"				AddULongWithKeyToTree(responce, \"ResponceType\", 5);\n"
		"				AddStringWithKeyToTree(responce, \"ModuleText\", GetModuleText(GetStringByKeyFromTree(jsonPt, \"ModuleName\")));\n"
		"			}\n"
		"			else if(requestType == 6 && isAuthorized()) { // Set new module text\n"
		"				ulong moduleId = GetModuleId(GetStringByKeyFromTree(jsonPt, \"ModuleName\"));\n"
		"				string newModuleText = GetStringByKeyFromTree(jsonPt, \"ModuleText\");\n\n"
		"				SetModuleText(moduleId, newModuleText);\n\n"
		"				AddULongWithKeyToTree(responce, \"ResponceType\", 6);\n"
		"				AddBoolWithKeyToTree(responce, \"ChangeStatus\", true);\n"
		"			}\n"
		"			else if(requestType == 7 && isAuthorized()) { // Create new module\n"
		"				string moduleName = GetStringByKeyFromTree(jsonPt, \"ModuleName\");\n"
		"				string moduleText = GetStringByKeyFromTree(jsonPt, \"ModuleText\");\n\n"
		"				CreateModule(GetMaxModuleId()+1, moduleName, moduleText);\n\n"
		"				AddULongWithKeyToTree(responce, \"ResponceType\", 7);\n"
		"			}\n"
		"			else if(requestType == 8 && isAuthorized()) { // Change module name\n"
		"				ulong moduleId = GetModuleId(GetStringByKeyFromTree(jsonPt, \"OldModuleName\"));\n"
		"				string newModuleName = GetStringByKeyFromTree(jsonPt, \"NewModuleName\");\n\n"
		"				SetModuleName(moduleId, newModuleName);\n\n"
		"				AddULongWithKeyToTree(responce, \"ResponceType\", 8);\n"
		"			}\n"
		"			else if(requestType == 9 && isAuthorized()) { // Delete module\n"
		"				ulong moduleId = GetModuleId(GetStringByKeyFromTree(jsonPt, \"ModuleName\"));\n\n"
		"				RemoveModule(moduleId);\n\n"
		"				AddULongWithKeyToTree(responce, \"ResponceType\", 9);\n"
		"			}\n"
		"			else if(requestType == 20 && isAuthorized()) { // Unload db data\n"
		"				bool performState = UnloadDbChanges();\n\n"
		"				if(performState) {\n"
		"					AddULongWithKeyToTree(responce, \"ResponceType\", 20);\n"
		"				}\n"
		"				else {\n"
		"					AddULongWithKeyToTree(responce, \"ResponceType\", 1000);\n"
		"					AddStringWithKeyToTree(responce, \"ErrorMessage\", \"Wrong in unload data base\");\n"
		"				}\n"
		"			}\n"
		"			else if(requestType == 50) { // Try authorize user as admin\n"
		"				string login = GetStringByKeyFromTree(jsonPt, \"Login\"),\n"
		"					password = GetStringByKeyFromTree(jsonPt, \"Password\");\n\n"
		"				AddULongWithKeyToTree(responce, \"ResponceType\", 50);\n"
		"				AddBoolWithKeyToTree(responce, \"AuthorizedStatus\", AuthorizeUser(login, password));\n"
		"			}\n"
		"			else if(requestType == 51) { // Get authorize status\n"
		"				AddULongWithKeyToTree(responce, \"ResponceType\", 51);\n"
		"				AddBoolWithKeyToTree(responce, \"AuthorizedStatus\", isAuthorized());\n"
		"			}\n"
		"			else if(requestType == 52) { // unauthorize user as admin\n"
		"				UnauthorizeUser();\n\n"
		"				AddULongWithKeyToTree(responce, \"ResponceType\", 52);\n"
		"			}\n"
		"			else { // Undefined request type for execute\n"
		"				AddULongWithKeyToTree(responce, \"ResponceType\", 1000);\n"
		"				AddStringWithKeyToTree(responce, \"ErrorMessage\", \"Undefined request type for execute\");\n"
		"			}\n\n"
		"			SendStringToClient(GetStringJSONFormatFromTree(responce));\n"
		"		}\n"
		"	}\n"
		"}\n"));

	//// DbController module
	m_vertexies->push_back(new Vertex(SWObjectDbControllerModuleNameId, DbControllerModuleNameInSW));
	m_vertexies->push_back(new Vertex(SWObjectDbControllerModuleTextId,
		"bool UnloadDbChanges() {\n"
		"	return UnloadDbChangesToDbFile();\n"
		"}\n\n"
		"ulong[] GetListOfFreeVertexIdsInDb() {\n"
		"	return GetListOfFreeVertexIds();\n"
		"}\n\n"
		"ulong GetLastWroteVertexIdInDb() {\n"
		"	return GetLastWroteVertexId();\n"
		"}\n\n"
		"ulong GetLastWroteNodeIdInDb() {\n"
		"	return GetLastWroteNodeId();\n"
		"}\n\n"
		"void CreateVertexWithDataInDb(ulong vertexId, string vertexText) {\n"
		"	CreateVertexWithData(vertexId, vertexText);\n"
		"}\n\n"
		"string GetStrFromVertexWithId(ulong vertexId) {\n"
		"	return GetStringFromVertexWithId(vertexId);\n"
		"}\n\n"
		"void SetStrToVertexWithId(ulong vertexId, string text) {\n"
		"	SetStringToVertexWithId(vertexId, text);\n"
		"}\n\n"
		"void DeleteVertexWithIdInDb(ulong vertexId) {\n"
		"	DeleteVertexWithId(vertexId);\n"
		"}\n\n"
		"ulong[] GetExistsNodeIdsInDb() {\n"
		"	return GetExistsNodeIds();\n"
		"}\n\n"
		"void CreateNodeWithIdInDb(ulong nodeId) {\n"
		"	CreateNodeWithId(nodeId);\n"
		"}\n\n"
		"void DeleteNodeWithIdInDb(ulong nodeId) {\n"
		"	DeleteNodeWithId(nodeId);\n"
		"}\n\n"
		"struct subNode {\n"
		"	ulong subNodeId;\n"
		"	ulong firstVertexId;\n"
		"	ulong secondVertexId;\n"
		"	ubyte own;\n"
		"}\n\n"
		"void CreateSubNodeWithDataInNodeWithIdInDb(ulong nodeId, subNode sN) {\n"
		"	CreateSubNodeWithDataInNodeWithId(nodeId, sN);\n"
		"}\n\n"
		"ulong[] GetSubNodeIdsOnNodeWithIdInDb(ulong nodeId) {\n"
		"	return GetSubNodeIdsOfNodeWithId(nodeId);\n"
		"}\n\n"
		"subNode[] GetSubNodesOfNodeWithIdInDb(ulong nodeId) {\n"
		"	return GetSubNodesOfNodeWithId(nodeId);\n"
		"}\n\n"

		"subNode[] FindByVertexIdAnyWhereInNodeWithIdInDb(ulong nodeId, ulong vertexId) {\n"
		"	return FindByVertexIdAnyWhereInNodeWithId(nodeId, vertexId);\n"
		"}\n\n"
		"subNode[] FindByFirstVertexIdInNodeWithIdInDb(ulong nodeId, ulong firstVertexId) {\n"
		"	return FindByFirstVertexIdInNodeWithId(nodeId, firstVertexId);\n"
		"}\n\n"
		"subNode[] FindBySecondVertexIdInNodeWithIdInDb(ulong nodeId, ulong secondVertexId) {\n"
		"	return FindBySecondVertexIdInNodeWithId(nodeId, secondVertexId);\n"
		"}\n\n"
		"subNode[] FindByBothVertexIdsInNodeWithIdInDb(ulong nodeId, ulong firstVertexId, ulong secondVertexId) {\n"
		"	return FindByBothVertexIdsInNodeWithId(nodeId, firstVertexId, secondVertexId);\n"
		"}\n\n"

		"subNode GetSubNodeWithIdFromNodeWithIdInDb(ulong nodeId, ulong subNodeId) {\n"
		"	return GetSubNodeWithIdFromNodeWithId(nodeId, subNodeId);\n"
		"}\n\n"
		"void SetSubNodeInNodeWithIdInDb(ulong nodeId, subNode sN) {\n"
		"	SetSubNodeInNodeWithId(nodeId, sN);\n"
		"}\n\n"
		"void DeleteSubNodeWithIdInNodeWithIdInDb(ulong nodeId, ulong subNodeId) {\n"
		"	DeleteSubNodeWithIdInNodeWithId(nodeId, subNodeId);\n"
		"}\n"));

	//// IIS module
	m_vertexies->push_back(new Vertex(SWObjectIISModuleNameId, IISModuleNameInSW));
	m_vertexies->push_back(new Vertex(SWObjectIISModuleTextId,
		"void RestartMyInterpretator() {\n"
		"	RestartInterpretator();\n"
		"}\n\n"
		"void UsingModuleWithId(ulong moduleId) {\n"
		"	UsingModuleWithId(moduleId);\n"
		"}\n\n"
		"void DisuseModuleWithId(ulong moduleId) {\n"
		"	DisuseModuleWithId(moduleId);\n"
		"}\n\n"
		"void PerformSyntaxAnalizyModule(ulong moduleId) {\n"
		"	PerformSyntaxAnalizyModule(moduleId);\n"
		"}\n"));

	//// Network module
	m_vertexies->push_back(new Vertex(SWObjectNetModuleNameId, NetModuleNameInSW));
	m_vertexies->push_back(new Vertex(SWObjectNetModuleTextId, 
		"void WaitConnectWithClient() {\n"
		"	WaitConnectionWithClient();\n"
		"}\n\n"
		"bool IsOpenConnection() {\n"
		"	return IsOpenConnectionWithClient();\n"
		"}\n\n"
		"void DisconnectConnection() {\n"
		"	DisconectConnectionWithClient();\n"
		"}\n\n"
		"void SendStringToClient(string text) {\n"
		"	SendStringToClient(text);\n"
		"}\n\n"
		"string WaitStringFromClient() {\n"
		"	return WaitGetStringFromClient();\n"
		"}\n"));

	m_vertexies->push_back(new Vertex(SWObjectModuleManageNameId, ModuleManageNameInSW));
	m_vertexies->push_back(new Vertex(SWObjectModuleManageTextId,
		"using \"DbController\"\n\n"

		"// Node contain pairs of module id and vertex ids contains module name\n"
		"ulong moduleIdNameVertexIdPairs = " + std::to_string(SWObjectModulesName) + ";\n\n"
		"// Return id of vertex contains name of module with id equal moduleId in parametr\n"
		"ulong GetModuleNameVertexId(ulong moduleId) {\n"
		"	subNode[] subNodes = GetSubNodesOfNodeWithIdInDb(moduleIdNameVertexIdPairs);\n"
		"	ulong subNodesSize = len(subNodes);\n"
		"	ulong foundVertexId = 0;\n\n"
		"	for(ulong index = 0; index < subNodesSize; index=index+1;) {\n"
		"		if(subNodes[index].firstVertexId == moduleId) {\n"
		"			return subNodes[index].secondVertexId;\n"
		"		}\n"
		"	}\n\n"
		"	return 0;\n"
		"}\n\n"

		"// Node contain pairs of module id and vertex ids contains module text\n"
		"ulong moduleIdTextVertexIdPairs = "+std::to_string(SWObjectProgramOfModules) + ";\n\n"
		"// Return id of vertex contains text of module with id equal moduleId in parametr\n"
		"ulong GetModuleTextVertexId(ulong moduleId) {\n"
		"	subNode[] subNodes = GetSubNodesOfNodeWithIdInDb(moduleIdTextVertexIdPairs);\n"
		"	ulong subNodesSize = len(subNodes);\n"
		"	ulong foundVertexId = 0;\n\n"
		"	for(ulong index = 0; index < subNodesSize; index=index+1;) {\n"
		"		if(subNodes[index].firstVertexId == moduleId) {\n"
		"			return subNodes[index].secondVertexId;\n"
		"		}\n"
		"	}\n\n"
		"	return 0;\n"
		"}\n\n"

		"ulong moduleIdsListVertexId = " + std::to_string(SWObjectModuleIdVertexList) + ";\n\n"
		"ulong[] GetModuleIds() {\n"
		"	subNode[] subNodes = GetSubNodesOfNodeWithIdInDb(moduleIdsListVertexId);\n"
		"	ulong subNodesSize = len(subNodes);\n"
		"	ulong moduleIdsCount = 0;\n\n"
		"	for(ulong index = 0; index < subNodesSize; index = index+1;) {\n"
		"		if(subNodes[index].firstVertexId != 0) {\n"
		"			moduleIdsCount = moduleIdsCount + 1;\n"
		"		}\n"
		"		if (subNodes[index].secondVertexId != 0) {\n"
		"			moduleIdsCount = moduleIdsCount + 1;\n"
		"		}\n"
		"	}\n\n"
		"	ulong[moduleIdsCount] moduleIds;\n"
		"	moduleIdsCount = 0;\n\n"
		"	for(ulong index = 0; index < subNodesSize && moduleIdsCount < len(moduleIds); index = index+1;) {\n"
		"		if(subNodes[index].firstVertexId != 0) {\n"
		"			moduleIds[moduleIdsCount] = subNodes[index].firstVertexId;\n"
		"			moduleIdsCount = moduleIdsCount+1;\n"
		"		}\n"
		"		if (subNodes[index].secondVertexId != 0) {\n"
		"			moduleIds[moduleIdsCount] = subNodes[index].secondVertexId;\n"
		"			moduleIdsCount = moduleIdsCount + 1;\n"
		"		}\n"
		"	}\n\n"
		"	return moduleIds;\n"
		"}\n\n"

		"ulong GetMaxModuleId() {\n"
		"	ulong[] moduleIds = GetModuleIds();\n"
		"	ulong maxModuleId = moduleIds[0];\n\n"
		"	for(ulong index = 0; index < len(moduleIds); index = index + 1;) {\n"
		"		if(moduleIds[index] > maxModuleId) {\n"
		"			maxModuleId = moduleIds[index];"
		"		}\n"
		"	}\n\n"
		"	return maxModuleId;\n"
		"}\n"

		"string[] GetModuleNames() {\n"
		"	ulong[] moduleIds = GetModuleIds();\n"
		"	ulong moduleIdsCount = len(moduleIds);"
		"	string[moduleIdsCount] moduleNames;\n\n"
		"	for(ulong index = 0; index < moduleIdsCount; index = index+1;) {\n"
		"		moduleNames[index] = GetModuleName(moduleIds[index]);\n"
		"	}\n\n"
		"	return moduleNames;\n"
		"}\n\n"

		"// Create vertexes with name and text of module and add subNodes to nodes for describe node relationships\n"
		"// If module with id exists exception\n"
		"void CreateModule(ulong moduleId, string moduleName, string moduleText) {\n"
		"	ulong moduleNameVertexId = GetLastWroteVertexIdInDb()+1;\n"
		"	ulong moduleTextVertexId = GetLastWroteVertexIdInDb()+2;\n\n"
		"	CreateVertexWithDataInDb(moduleNameVertexId, moduleName);\n"
		"	CreateVertexWithDataInDb(moduleTextVertexId, moduleText);\n\n"
		"	// Add SubNode about pair module id and vertex id contains module name\n"
		"	subNode moduleNameSubNode;\n"
		"	moduleNameSubNode.firstVertexId = moduleId;\n"
		"	moduleNameSubNode.secondVertexId = moduleNameVertexId;\n"
		"	CreateSubNodeWithDataInNodeWithIdInDb(moduleIdNameVertexIdPairs, moduleNameSubNode);\n\n"
		"	// Add SubNode about pair module id and vertex id contains module text\n"
		"	subNode moduleTextSubNode;\n"
		"	moduleTextSubNode.firstVertexId = moduleId;\n"
		"	moduleTextSubNode.secondVertexId = moduleTextVertexId;\n"
		"	CreateSubNodeWithDataInNodeWithIdInDb(moduleIdTextVertexIdPairs, moduleTextSubNode);\n\n"
		"	// Add module id in vertex contains module ids list\n"
		"	subNode[] moduleIdsListSubNodes = GetSubNodesOfNodeWithIdInDb(moduleIdsListVertexId);\n"
		"	if(moduleIdsListSubNodes[len(moduleIdsListSubNodes)-1].secondVertexId == 0) {\n"
		"		subNode lastNotEntitySN = moduleIdsListSubNodes[len(moduleIdsListSubNodes)-1];\n"
		"		lastNotEntitySN.secondVertexId = moduleId;\n"
		"		SetSubNodeInNodeWithIdInDb(moduleIdsListVertexId, lastNotEntitySN);\n"
		"	}\n"
		"	else {\n"
		"		subNode newModuleSN;\n"
		"		newModuleSN.subNodeId = 0;\n"
		"		newModuleSN.firstVertexId = moduleId;\n"
		"		newModuleSN.secondVertexId = 0;\n"
		"		newModuleSN.own = 255;\n"
		"		CreateSubNodeWithDataInNodeWithIdInDb(moduleIdsListVertexId, newModuleSN);\n"
		"	}\n"
		"}\n\n"

		"// Return id of module with name equal moduleName in parametr\n"
		"// If module with name is not exist return 0\n"
		"ulong GetModuleId(string moduleName) {\n"
		"	ulong[] moduleIds = GetModuleIds();\n"
		"	ulong moduleIdsSize = len(moduleIds);\n\n"
		"	for(ulong index = 0; index < moduleIdsSize; index = index + 1;) {\n"
		"		if(moduleIds[index] != 0 && GetStrFromVertexWithId(GetModuleNameVertexId(moduleIds[index])) == moduleName) {\n"
		"			return moduleIds[index];\n"
		"		}\n"
		"	}\n\n"
		"	return 0;\n"
		"}\n\n"

		"// Return name of module with id equal moduleId in parametr\n"
		"// If module with id is not exist return exception\n"
		"string GetModuleName(ulong moduleId) {\n"
		"	return GetStrFromVertexWithId(GetModuleNameVertexId(moduleId));\n"
		"}\n\n"
		"void SetModuleName(ulong moduleId, string moduleName) {\n"
		"	SetStrToVertexWithId(GetModuleNameVertexId(moduleId), moduleName);\n"
		"}\n\n"
		
		"// Return text of module with id equal moduleId in parametr\n"
		"// If module with name is not exist return exception\n"
		"string GetModuleText(ulong moduleId) {\n"
		"	return GetStrFromVertexWithId(GetModuleTextVertexId(moduleId));\n"
		"}\n\n"
		"string GetModuleText(string moduleName) {\n"
		"	return GetModuleText(GetModuleId(moduleName));\n"
		"}\n\n"
		"void SetModuleText(ulong moduleId, string moduleText) {\n"
		"	SetStrToVertexWithId(GetModuleTextVertexId(moduleId), moduleText);\n"
		"}\n\n"
		
		"// Remove vertexes contains name and text and remove from subnodes moduleId\n"
		"void RemoveModule(ulong moduleId) {\n"
		"	DeleteVertexWithIdInDb(GetModuleNameVertexId(moduleId));\n"
		"	DeleteVertexWithIdInDb(GetModuleTextVertexId(moduleId));\n\n"
		"	DeleteSubNodeWithIdInNodeWithIdInDb(moduleIdNameVertexIdPairs, FindByFirstVertexIdInNodeWithIdInDb(moduleIdNameVertexIdPairs, moduleId)[0].secondVertexId);\n"
		"	DeleteSubNodeWithIdInNodeWithIdInDb(moduleIdTextVertexIdPairs, FindByFirstVertexIdInNodeWithIdInDb(moduleIdTextVertexIdPairs, moduleId)[0].secondVertexId);\n\n"
		"	// Delete module id in vertex contents module ids list\n"
		"	subNode sNModId = FindByVertexIdAnyWhereInNodeWithIdInDb(moduleIdsListVertexId, moduleId)[0];\n"
		"	if(sNModId.firstVertexId == moduleId) {\n"
		"		sNModId.firstVertexId = moduleId;\n"
		"	}\n"
		"	else {\n"
		"		sNModId.secondVertexId = moduleId;\n"
		"	}\n"
		"	SetSubNodeInNodeWithIdInDb(moduleIdsListVertexId, sNModId);\n"
		"}\n"));

		m_vertexies->push_back(new Vertex(SWObjectPermissionManageNameId, PermissionManageNameInSW));
		m_vertexies->push_back(new Vertex(SWObjectPermissionManageTextId, 
			""));

		m_vertexies->push_back(new Vertex(SWObjectUserControlNameId, UserControlNameInSW));
		m_vertexies->push_back(new Vertex(SWObjectUserControlTextId,
			"bool isAuthorized = false;\n"
			"string AdminLogin = \"log\";\n"
			"string AdminPassword = \"gol\";\n\n"

			"bool AuthorizeUser(string login, string password) {\n"
			"	isAuthorized = AdminLogin == login && AdminPassword == password;\n"
			"	return isAuthorized;\n"
			"}\n\n"
			"bool isAuthorized() {\n"
			"	return isAuthorized;\n"
			"}\n\n"
			"void UnauthorizeUser() {\n"
			"	isAuthorized = false;\n"
			"}\n"));

		m_vertexies->push_back(new Vertex(SWObjectCalculateDemographyNameId, CalculateDemographyNameInSW));
		m_vertexies->push_back(new Vertex(SWObjectCalculateDemographyTextId,
			"using \"" NetModuleNameInSW "\"\n\n"

			"string[] importProfiles = {\"Табличный документ: объект(дискретный)-год\", \"Табличный документ: объект(непрерывный)-год\", \"Табличный документ: объект(дискретное)-ранг\", \"Табличный документ: объект(номинальный)-ранг\"};\n\n"

			"string[] generalAnalyzeTypes = {\"Необработанные данные\", \"Общая таблица статистики\","
				"\"Таблица рейтинга по приращению\","
				"\"График медианы объектов\", \"График среднего значения объектов\", \"График разности крайних значений объектов\"};\n"
			"string[] analyzeTypes = {\"Необработанные данные\", \"Общая таблица по объекту\","
				"\"Таблица мер центральной тенденции\", \"График данных\","
				"\"Прогноз по медиане\", \"Прогноз по среднему значению\", \"Прогноз по моде\", \"Прогноз по разности крайних значений\"};\n"
			"string[] chartTypes = {\"Line\", \"Column\"};\n"

			"ulong predictForNDates = 1;\n\n"

			"struct lineWithObjectLongData {\n"
			"	string objectName;\n"
			"	long[] data;\n"
			"}\n\n"

			"struct lineWithObjectDoubleData {\n"
			"	string objectName;\n"
			"	double[] data;\n"
			"}\n\n"

			"struct lineWithObjectStringData {\n"
			"	string objectName;\n"
			"	string[] data;\n"
			"}\n\n"

			"void SendImportProfiles() {\n"
			"	tree responce, importProfilesTree;\n\n"
			"	for(ulong index = 0; index < len(importProfiles); index = index+1;) {\n"
			"		AddStringToTreeArray(importProfilesTree, importProfiles[index]);\n"
			"	}\n\n"
			"	AddChildWithKeyToTree(responce, \"ImportProfiles\", importProfilesTree);\n"
			"	AddULongWithKeyToTree(responce, \"ResponceType\", 100);\n\n"
			"	SendStringToClient(GetStringJSONFormatFromTree(responce));\n"
			"}\n\n"
			"void SendSelectedDataPresentations(tree jsonPt) {\n"
			"	tree responce, selDatPresTree;\n\n"
			"	string importProfile = GetStringByKeyFromTree(jsonPt, \"ImportProfile\");\n"
			"	string[] analyzeObjects = GetAnalyzeObjectsName(GetChildByKeyFromTree(jsonPt, \"ImportedTable\"), importProfile);\n"
			"	ulong analyzeObjSize = len(analyzeObjects);\n\n"
			"	AddStringToTreeArray(selDatPresTree, \"Общее\");\n\n"
			"	for(ulong index = 0; index < analyzeObjSize; index = index + 1;) {\n"
			"		AddStringToTreeArray(selDatPresTree, analyzeObjects[index]);\n"
			"	}\n\n"
			"	AddULongWithKeyToTree(responce, \"ResponceType\", 110);\n"
			"	AddChildWithKeyToTree(responce, \"SelectedDataPresentations\", selDatPresTree);\n\n"
			"	SendStringToClient(GetStringJSONFormatFromTree(responce));\n"
			"}\n\n"
			"void SendAnalyzeTypes(tree jsonPt) {\n"
			"	tree responce, analyzeTypesTree;\n"
			"	string selectedDataPresentation = GetStringByKeyFromTree(jsonPt, \"SelectedDataPresentation\");\n"
			"	string importProfile = GetStringByKeyFromTree(jsonPt, \"ImportProfile\");\n\n"
			"	if(selectedDataPresentation == \"Общее\") {\n"
			"		AddStringToTreeArray(analyzeTypesTree, generalAnalyzeTypes[0]);\n\n"
			"		if(importProfile == importProfiles[0] || importProfile == importProfiles[2]) {\n"
			"			for(ulong index = 1; index < len(generalAnalyzeTypes); index = index+1;) {\n"
			"				AddStringToTreeArray(analyzeTypesTree, generalAnalyzeTypes[index]);\n"
			"			}\n"
			"		}\n"
			"		else if(importProfile == importProfiles[1]) {\n"
			"			for(ulong index = 1; index < len(generalAnalyzeTypes); index = index+1;) {\n"
			"				AddStringToTreeArray(analyzeTypesTree, generalAnalyzeTypes[index]);\n"
			"			}\n"
			"		}\n"
			"		else if(importProfile == importProfiles[3]) {\n"
			"			AddStringToTreeArray(analyzeTypesTree, generalAnalyzeTypes[1]);\n"
			"		}\n"
			"	}\n"
			"	else {\n"
			"		AddStringToTreeArray(analyzeTypesTree, analyzeTypes[0]);\n"
			"		AddStringToTreeArray(analyzeTypesTree, analyzeTypes[1]);\n"
			"		AddStringToTreeArray(analyzeTypesTree, analyzeTypes[2]);\n"
			"		AddStringToTreeArray(analyzeTypesTree, analyzeTypes[3]);\n\n"
			"		if(importProfile == importProfiles[0] || importProfile == importProfiles[1]) {\n"
			"			AddStringToTreeArray(analyzeTypesTree, analyzeTypes[4]);\n"
			"			AddStringToTreeArray(analyzeTypesTree, analyzeTypes[5]);\n\n"
			"			if(importProfile == importProfiles[0]) {\n"
			"				AddStringToTreeArray(analyzeTypesTree, analyzeTypes[6]);\n\n"
			"			}\n"
			"			AddStringToTreeArray(analyzeTypesTree, analyzeTypes[7]);\n\n"
			"		}\n"
			"	}\n\n"
			"	AddChildWithKeyToTree(responce, \"AnalyzeTypes\", analyzeTypesTree);\n"
			"	AddULongWithKeyToTree(responce, \"ResponceType\", 120);\n\n"
			"	SendStringToClient(GetStringJSONFormatFromTree(responce));\n"
			"}\n\n"

			"void AnalyzeAndSendResponce(tree jsonPt) {\n"
			"	tree responce, analyzeResult, dates, YDatas;\n\n"
			"	string importProfile = GetStringByKeyFromTree(jsonPt, \"ImportProfile\");\n"
			"	string analyzeType = GetStringByKeyFromTree(jsonPt, \"AnalyzeType\");\n"
			"	string selectedDataPresent = GetStringByKeyFromTree(jsonPt, \"SelectedDataPresent\");\n"
			"	tree importedTablePt = GetChildByKeyFromTree(jsonPt, \"ImportedTable\");\n\n"

			"	if(selectedDataPresent == \"Общее\") {\n"
			"		if (analyzeType == generalAnalyzeTypes[0]) {\n"
			"			analyzeResult = UnanalyzeData(importedTablePt);\n"
			"		}\n"
			"		else if (analyzeType == generalAnalyzeTypes[1]) {\n"
			"			analyzeResult = GeneralTableDataAnalyze(importedTablePt, importProfile);\n"
			"		}\n"
			"		else if (analyzeType == generalAnalyzeTypes[2]) {\n"
			"			analyzeResult = AnalyzeIncrementTopTable(importedTablePt, importProfile);\n"
			"		}\n"
			"		else if (analyzeType == generalAnalyzeTypes[3]) {\n"
			"			analyzeTable = AnalyzeChartMedianObjects(importedTablePt, importProfile);\n"
			"		}\n"
			"		else if (analyzeType == generalAnalyzeTypes[4]) {\n"
			"			analyzeTable = AnalyzeChartAverageObjects(importedTablePt, importProfile);\n"
			"		}\n"
			"		else if (analyzeType == generalAnalyzeTypes[5]) {\n"
			"			analyzeTable = AnalyzeChartExtremeObjects(importedTablePt, importProfile);\n"
			"		}\n\n"
			"	}\n"
			"	else {\n"
			"		if (analyzeType == analyzeTypes[0]) {\n"
			"			analyzeResult = UnanalyzeData(importedTablePt, selectedDataPresent);\n"
			"		}\n"
			"		else if (analyzeType == analyzeTypes[1]) {\n"
			"			analyzeResult = GeneralTableDataAnalyze(importedTablePt, selectedDataPresent, importProfile);\n"
			"		}\n"
			"		else if (analyzeType == analyzeTypes[2]) {\n"
			"			analyzeResult = AnalyzeCentralTrendTable(importedTablePt, selectedDataPresent, importProfile);\n"
			"		}\n"
			"		else if (analyzeType == analyzeTypes[3]) {\n"
			"			analyzeResult = ChartDataShow(importedTablePt, selectedDataPresent, importProfile);\n"
			"		}\n"
			"		else if (analyzeType == analyzeTypes[4]) {\n"
			"			analyzeResult = AnalyzeMedianForecast(importedTablePt, selectedDataPresent, importProfile);\n"
			"		}\n"
			"		else if (analyzeType == analyzeTypes[5]) {\n"
			"			analyzeResult = AnalyzeAverageForecast(importedTablePt, selectedDataPresent, importProfile);\n"
			"		}\n"
			"		else if (analyzeType == analyzeTypes[6]) {\n"
			"			analyzeResult = AnalyzeModeForecast(importedTablePt, selectedDataPresent, importProfile);\n"
			"		}\n"
			"		else if (analyzeType == analyzeTypes[7]) {\n"
			"			analyzeResult = AnalyzeExtremeForecast(importedTablePt, selectedDataPresent, importProfile);\n"
			"		}\n"
			"	}\n\n"
			"	AddULongWithKeyToTree(responce, \"ResponceType\", 150);\n"
			"	AddChildWithKeyToTree(responce, \"AnalyzeResult\", analyzeResult);\n\n"
			"	SendStringToClient(GetStringJSONFormatFromTree(responce));\n"
			"}\n\n"

			"tree UnanalyzeData(tree importedTablePt) {\n"
			"	tree analyzeResponce, columnsTree, rowsTree;\n"
			"	columnsTree = GetColumnNamesTree(importedTablePt);\n\n"
			"	lineWithObjectStringData[] rows = GetAnalyzeStringObjects(importedTablePt);\n"
			"	ulong rowsCount = len(rows);\n\n"
			"	for(ulong index = 0; index < rowsCount; index = index + 1;) {\n"
			"		tree rowTree;\n"
			"		lineWithObjectStringData row = rows[index];\n\n"
			"		AddStringToTreeArray(rowTree, row.objectName);\n\n"
			"		ulong dataCount = len(row.data);\n\n"
			"		for(ulong dataIndex = 0; dataIndex < dataCount; dataIndex = dataIndex + 1;) {\n"
			"			AddStringToTreeArray(rowTree, row.data[dataIndex]);\n"
			"		}\n"
			"		AddChildWithKeyToTree(rowsTree, ULongToString(index+1), rowTree);\n"
			"	}\n"
			"	AddULongWithKeyToTree(analyzeResponce, \"RowCount\", rowsCount);\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Table\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"Columns\", columnsTree);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"Rows\", rowsTree);\n"
			"	return analyzeResponce;\n"
			"}\n"

			"tree GeneralTableDataAnalyze(tree importedTablePt, string importProfile) {\n"
			"	tree analyzeResponce, columnsTree, rowsTree;\n"
			"	;\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Table\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"Columns\", columnsTree);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"Rows\", rowsTree);\n"
			"	return analyzeResponce;\n"
			"}\n"

			"tree AnalyzeIncrementTopTable(tree importedTablePt, string importProfile) {\n"
			"	tree analyzeResponce, columnsTree, rowsTree;\n"
			"	;\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Table\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"Columns\", columnsTree);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"Rows\", rowsTree);\n"
			"	return analyzeResponce;\n"
			"}\n"	

			"tree AnalyzeChartMedianObjects(tree importedTablePt, string importProfile) {\n"
			"	tree analyzeResponce, XData, YData;\n"
			"	string yDataType = \"Int\";\n"
			"	;\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Chart\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"SeriesName\", \"Тренд медианы\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ChartType\", \"Column\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataXBind\", XData);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataYBind\", YData);\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"YDataType\", yDataType);\n"
			"	return analyzeResponce;\n"
			"}\n"

			"tree AnalyzeChartAverageObjects(tree importedTablePt, string importProfile) {\n"
			"	tree analyzeResponce, XData, YData;\n"
			"	string yDataType = \"Int\";\n"
			"	;\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Chart\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"SeriesName\", \"Тренд среднего значения\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ChartType\", \"Column\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataXBind\", XData);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataYBind\", YData);\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"YDataType\", yDataType);\n"
			"	return analyzeResponce;\n"
			"}\n"

			"tree AnalyzeChartExtremeObjects(tree importedTablePt, string importProfile) {\n"
			"	tree analyzeResponce, XData, YData;\n"
			"	string yDataType = \"Int\";\n"
			"	;\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Chart\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"SeriesName\", \"Тренд среднего значения\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ChartType\", \"Column\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataXBind\", XData);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataYBind\", YData);\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"YDataType\", yDataType);\n"
			"	return analyzeResponce;\n"
			"}\n"

			// Simple
			"tree UnanalyzeData(tree importedTablePt, string selectedDataPresent) {\n"
			"	tree analyzeResponce, columnsTree, rowsTree;\n"
			"	columnsTree = GetColumnNamesTree(importedTablePt);\n\n"
			"	lineWithObjectStringData[] rows = GetAnalyzeStringObjects(importedTablePt);\n"
			"	ulong rowsCount = len(rows);\n\n"
			"	for(ulong index = 0; index < rowsCount; index = index + 1;) {\n"
			"		if(rows[index].objectName == selectedDataPresent) {\n"
			"			tree rowTree;\n"
			"			lineWithObjectStringData row = rows[index];\n\n"
			"			AddStringToTreeArray(rowTree, row.objectName);\n\n"
			"			ulong dataCount = len(row.data);\n\n"
			"			for(ulong dataIndex = 0; dataIndex < dataCount; dataIndex = dataIndex + 1;) {\n"
			"				AddStringToTreeArray(rowTree, row.data[dataIndex]);\n"
			"			}\n"
			"			AddChildWithKeyToTree(rowsTree, ULongToString(index+1), rowTree);\n"
			"			index = rowsCount;"
			"		}\n"
			"	}\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Table\");\n"
			"	AddULongWithKeyToTree(analyzeResponce, \"RowCount\", rowsCount);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"Columns\", columnsTree);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"Rows\", rowsTree);\n"
			"	return analyzeResponce;\n"
			"}\n"

			"tree GeneralTableDataAnalyze(tree importedTablePt, string selectedDataPresent, string importProfile) {\n"
			"	tree analyzeResponce, columnsTree, rowsTree;\n"
			"	;\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Table\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"Columns\", columnsTree);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"Rows\", rowsTree);\n"
			"	return analyzeResponce;\n"
			"}\n"

			"tree AnalyzeCentralTrendTable(tree importedTablePt, string selectedDataPresent, string importProfile) {\n"
			"	tree analyzeResponce, columnsTree, rowsTree;\n"

			"	AddStringToTreeArray(columnsTree, \"Цетральные тренды\");\n"
			"	AddStringToTreeArray(columnsTree, \"Значение\");\n\n"

			"	lineWithObjectStringData[] rows = GetAnalyzeStringObjects(importedTablePt);\n\n"
			"	string averageVal, medianVal, maxVal, minVal;\n\n"
			"	for(ulong index = 0; index < len(rows); index = index + 1;) {\n"
			"		if(rows[index].objectName == selectedDataPresent) {\n"
			"			string[] data = rows[index].data;\n"
			"			maxVal = data[0];\n"
			"			minVal = data[0];\n\n"

			"			for(ulong dataIndex = 0; dataIndex < len(data); dataIndex = dataIndex + 1;) {\n"
			"				if(importProfile == importProfiles[0]) {\n"
			"					averageVal = IntToString(StringToInt(averageVal) + StringToInt(data[dataIndex]) / (len(data)-1));\n"
			"				}\n"
			"				else if(importProfile == importProfiles[1]) {\n"
			"					double val = StringToDouble(data[dataIndex]);"
			"					averageVal = DoubleToString(StringToDouble(averageVal) + val / (len(data)-1));\n"

			"					if(StringToDouble(maxVal) > val) {\n"
			"						maxVal = DoubleToString(val);\n"
			"					}\n"
			"					if(StringToDouble(minVal) < val) {\n"
			"						minVal = DoubleToString(val);\n"
			"					}\n"
			"				}\n"
			"			}\n\n"

			"			index = len(rows);\n"
			"		}\n"
			"	}\n"

			"	if(importProfile == importProfiles[0]) {\n"
			"		medianVal = IntToString(StringToInt(maxVal) - StringToInt(minVal));\n"
			"	}\n"
			"	else if(importProfile == importProfiles[1]) {\n"
			"		medianVal = DoubleToString(StringToDouble(maxVal) - StringToDouble(minVal));\n"
			"	}\n"

			"	tree aveRowTree, medianRowTree;\n\n"
			"	AddStringToTreeArray(aveRowTree, \"Среднее значение\");\n"
			"	AddStringToTreeArray(aveRowTree, averageVal);\n"
			"	AddStringToTreeArray(medianRowTree, \"Медиана\");\n"
			"	AddStringToTreeArray(medianRowTree, medianVal);\n\n"
				
			"	AddChildWithKeyToTree(rowsTree, \"1\", aveRowTree);\n"
			"	AddChildWithKeyToTree(rowsTree, \"2\", medianRowTree);\n"

			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Table\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"Columns\", columnsTree);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"Rows\", rowsTree);\n"
			"	return analyzeResponce;\n"
			"}\n"

			"tree ChartDataShow(tree importedTablePt, string selectedDataPresent, string importProfile) {\n"
			"	tree analyzeResponce, XData, YData;\n"
			"	string yDataType = \"Int\";\n"
			"	string[] firstColumnDatas = GetTreeAsStringArray(GetChildByKeyFromTree(importedTablePt, ULongToString(1)));\n\n"

			"	for(ulong index = 1; index < len(firstColumnDatas); index = index + 1;) {\n"
			"		AddStringToTreeArray(XData, firstColumnDatas[index]);\n"
			"	}\n\n"

			"	lineWithObjectStringData[] rows = GetAnalyzeStringObjects(importedTablePt);\n"
			"	for(ulong index = 0; index < len(rows); index = index + 1;) {\n"
			"		if(rows[index].objectName == selectedDataPresent) {\n"
			"			string[] data = rows[index].data;\n\n"
			"			for(ulong dataIndex = 0; dataIndex < len(data); dataIndex = dataIndex + 1;) {\n"
			"				if(importProfile == importProfiles[0]) {\n"
			"					AddIntToTreeArray(YData, StringToInt(data[dataIndex]));\n"
			"				}\n"
			"				else if(importProfile == importProfiles[1]) {\n"
			"					AddDoubleToTreeArray(YData, StringToDouble(data[dataIndex]));\n\n"
			"					yDataType = \"Double\";\n"
			"				}\n"
			"			}\n\n"

			"			index = len(rows);\n"
			"		}\n"
			"	}\n"

			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Chart\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"SeriesName\", \"Обработанные данные\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ChartType\", \"Line\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataXBind\", XData);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataYBind\", YData);\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"YDataType\", yDataType);\n"
			"	return analyzeResponce;\n"
			"}\n"

			"tree AnalyzeMedianForecast(tree importedTablePt, string selectedDataPresent, string importProfile) {\n"
			"	tree analyzeResponce, XData, YData;\n"
			"	string yDataType = \"Int\";\n"
			"	;\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Chart\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"SeriesName\", \"Тренд среднего значения\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ChartType\", \"Column\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataXBind\", XData);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataYBind\", YData);\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"YDataType\", yDataType);\n"
			"	return analyzeResponce;\n"
			"}\n"

			"tree AnalyzeAverageForecast(tree importedTablePt, string selectedDataPresent, string importProfile) {\n"
			"	tree analyzeResponce, XData, YData;\n"
			"	string yDataType = \"Int\";\n"
			"	string[] firstColumnDatas = GetTreeAsStringArray(GetChildByKeyFromTree(importedTablePt, ULongToString(1)));\n\n"

			"	for(ulong index = 1; index < len(firstColumnDatas); index = index + 1;) {\n"
			"		AddStringToTreeArray(XData, firstColumnDatas[index]);\n"
			"	}\n\n"

			"	lineWithObjectStringData[] rows = GetAnalyzeStringObjects(importedTablePt);\n"
			"	for(ulong index = 0; index < len(rows); index = index + 1;) {\n"
			"		if(rows[index].objectName == selectedDataPresent) {\n"
			"			string averageVal = \"0\";\n"
			"			string[] data = rows[index].data;\n\n"
			"			for(ulong dataIndex = 0; dataIndex < len(data); dataIndex = dataIndex + 1;) {\n"
			"				if(importProfile == importProfiles[0]) {\n"
			"					int val = StringToInt(data[dataIndex]);\n\n"
			"					averageVal = IntToString(StringToInt(averageVal) + val / len(data));\n\n"
			"					AddIntToTreeArray(YData, val);\n"
			"				}\n"
			"				else if(importProfile == importProfiles[1]) {\n"
			"					double val = StringToDouble(data[dataIndex]);\n\n"
			"					averageVal = DoubleToString(StringToDouble(averageVal) + val / len(data));\n\n"
			"					AddDoubleToTreeArray(YData, val);\n"
			"					yDataType = \"Double\";\n"
			"				}\n"
			"			}\n\n"

			"			for(ulong nextN = 0; nextN < predictForNDates; nextN = nextN + 1;) {\n"
			"				ResizeArray(firstColumnDatas, len(firstColumnDatas) + 1);\n\n"
			"				string lastYear = firstColumnDatas[len(firstColumnDatas)-2];\n"
			"				firstColumnDatas[len(firstColumnDatas)-1] = ULongToString(StringToULong(Substring(lastYear, 0, 4)) + 1) + Substring(lastYear, 4);\n"
			"				AddStringToTreeArray(XData, firstColumnDatas[len(firstColumnDatas)-1]);\n"

			"				ResizeArray(data, len(data)+1);\n"
			"				if(importProfile == importProfiles[0]) {\n"
			"					int nextAverage = (StringToInt(averageVal) * (len(data) - 1) + StringToInt(data[len(data)-2]) / (len(data)-1));\n"
			"					data[len(data)-1] = averageVal;\n"
			"					AddIntToTreeArray(YData, StringToInt(averageVal));\n\n"
			"					averageVal = IntToString(nextAverage);\n"
			"				}\n"
			"				else if(importProfile == importProfiles[1]) {\n"
			"					double nextAverage = (StringToDouble(averageVal) * (len(data) - 1) + StringToDouble(data[len(data)-2]) / (len(data)-1));\n"
			"					data[len(data)-1] = averageVal;\n"
			"					AddDoubleToTreeArray(YData, StringToDouble(averageVal));\n\n"
			"					averageVal = DoubleToString(nextAverage);\n"
			"				}\n"
			"			}\n"

			"			index = len(rows);\n"
			"		}\n"
			"	}\n\n"

			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Chart\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"SeriesName\", \"Тренд среднего значения\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ChartType\", \"Column\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataXBind\", XData);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataYBind\", YData);\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"YDataType\", yDataType);\n"
			"	return analyzeResponce;\n"
			"}\n"

			"tree AnalyzeModeForecast(tree importedTablePt, string selectedDataPresent, string importProfile) {\n"
			"	tree analyzeResponce, XData, YData;\n"
			"	string yDataType = \"Int\";\n"
			"	;\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Chart\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"SeriesName\", \"Тренд среднего значения\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ChartType\", \"Column\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataXBind\", XData);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataYBind\", YData);\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"YDataType\", yDataType);\n"
			"	return analyzeResponce;\n"
			"}\n"

			"tree AnalyzeExtremeForecast(tree importedTablePt, string selectedDataPresent, string importProfile) {\n"
			"	tree analyzeResponce, XData, YData;\n"
			"	string yDataType = \"Int\";\n"
			"	string[] firstColumnDatas = GetTreeAsStringArray(GetChildByKeyFromTree(importedTablePt, ULongToString(1)));\n\n"
			
			"	for(ulong index = 1; index < len(firstColumnDatas); index = index + 1;) {\n"
			"		AddStringToTreeArray(XData, firstColumnDatas[index]);\n"
			"	}\n\n"

			"	lineWithObjectStringData[] rows = GetAnalyzeStringObjects(importedTablePt);\n"
			"	for(ulong index = 0; index < len(rows); index = index + 1;) {\n"
			"		if(rows[index].objectName == selectedDataPresent) {\n"
			"			string[] data = rows[index].data;\n\n"
			"			for(ulong dataIndex = 0; dataIndex < len(data); dataIndex = dataIndex + 1;) {\n"
			"				if(importProfile == importProfiles[0]) {\n"
			"					AddIntToTreeArray(YData, StringToInt(data[dataIndex]));\n"
			"				}\n"
			"				else if(importProfile == importProfiles[1]) {\n"
			"					AddDoubleToTreeArray(YData, StringToDouble(data[dataIndex]));\n"
			"					yDataType = \"Double\";\n"
			"				}\n"
			"			}\n\n"

			"			for(ulong nextN = 0; nextN < predictForNDates; nextN = nextN + 1;) {\n"
			"				ResizeArray(firstColumnDatas, len(firstColumnDatas) + 1);\n\n"
			"				string lastYear = firstColumnDatas[len(firstColumnDatas)-2];\n"
			"				firstColumnDatas[len(firstColumnDatas)-1] = ULongToString(StringToULong(Substring(lastYear, 0, 4)) + 1) + Substring(lastYear, 4);\n"
			"				AddStringToTreeArray(XData, firstColumnDatas[len(firstColumnDatas)-1]);\n"

			"				string firstExtreme = data[0];\n"
			"				string lastExtreme = data[len(data)-1];\n\n"
			"				ResizeArray(data, len(data)+1);\n"
			"				if(importProfile == importProfiles[0]) {\n"
			"					ulong newVal = 2*StringToULong(lastExtreme) - StringToULong(firstExtreme);\n"
			"					data[len(data)-1] = ULongToString(newVal);\n"
			"					AddULongToTreeArray(YData, newVal);\n"
			"				}\n"
			"				else if(importProfile == importProfiles[1]) {\n"
			"					double newVal = 2*StringToDouble(lastExtreme) - StringToDouble(firstExtreme);\n"
			"					data[len(data)-1] = DoubleToString(newVal);\n"
			"					AddDoubleToTreeArray(YData, newVal);\n"
			"				}\n"
			"			}\n"

			"			index = len(rows);\n"
			"		}\n"
			"	}\n"

			"	AddStringWithKeyToTree(analyzeResponce, \"ResultPresentFormat\", \"Chart\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"SeriesName\", \"Тренд среднего значения\");\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"ChartType\", \"Column\");\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataXBind\", XData);\n"
			"	AddChildWithKeyToTree(analyzeResponce, \"DataYBind\", YData);\n"
			"	AddStringWithKeyToTree(analyzeResponce, \"YDataType\", yDataType);\n"
			"	return analyzeResponce;\n"
			"}\n"


			"tree GetColumnNamesTree(tree importedTablePt) {\n"
			"	string[] firstColumnDatas = GetTreeAsStringArray(GetChildByKeyFromTree(importedTablePt, ULongToString(1)));\n"
			"	tree columnTree;\n\n"
			"	for(ulong index = 0; index < len(firstColumnDatas); index = index + 1;) {\n"
			"		AddStringToTreeArray(columnTree, firstColumnDatas[index]);\n"
			"	}\n\n"
			"	return columnTree;\n"
			"}\n"

			"string[] GetAnalyzeObjectsName(tree importedTablePt, string importProfile) {\n"
			"	ulong lastLine = GetULongByKeyFromTree(importedTablePt, \"LastLine\");\n"
			"	string[lastLine-1] analyzeObjects;\n\n"
			"	for(ulong line = 2; line <= lastLine; line = line + 1;) {\n"
			"		string[] columns = GetTreeAsStringArray(GetChildByKeyFromTree(importedTablePt, ULongToString(line)));\n\n"
			"		analyzeObjects[line-2] = columns[0];\n"
			"	}\n"
			"	return analyzeObjects;\n"
			"}\n\n"

			"lineWithObjectDoubleData[] GetAnalyzeDoubleObjects(tree importedTablePt) {\n"
			"	ulong lastLine = GetULongByKeyFromTree(importedTablePt, \"LastLine\");\n"
			"	lineWithObjectDoubleData[lastLine] analyzeObjects;\n\n"
			"	for(ulong line = 2; line <= lastLine; line = line + 1;) {\n"
			"		string[] columns = GetTreeAsStringArray(GetChildByKeyFromTree(importedTablePt, ULongToString(line)));\n\n\n"
			"		analyzeObjects[line-2].objectName = columns[0];\n"
			"		ResizeArray(analyzeObjects[line-2].data, len(columns)-1);\n\n"
			"		for(ulong column = 2; column <= len(columns); column= column + 1;) {\n"
			"			analyzeObjects[line-2].data[column-2] = StringToDouble(columns[column-1]);\n"
			"		}\n\n"
			"	}\n"
			"	return analyzeObjects;\n"
			"}\n\n"

			"lineWithObjectLongData[] GetAnalyzeLongObjects(tree importedTablePt) {\n"
			"	ulong lastLine = GetULongByKeyFromTree(importedTablePt, \"LastLine\");\n"
			"	lineWithObjectLongData[lastLine] analyzeObjects;\n\n"
			"	for(ulong line = 2; line <= lastLine; line = line + 1;) {\n"
			"		string[] columns = GetTreeAsStringArray(GetChildByKeyFromTree(importedTablePt, ULongToString(line)));\n\n\n"
			"		analyzeObjects[line-2].objectName = columns[0];\n"
			"		ResizeArray(analyzeObjects[line-2].data, len(columns)-1);\n\n"
			"		for(ulong column = 2; column <= len(columns); column= column + 1;) {\n"
			"			analyzeObjects[line-2].data[column-2] = StringToLong(columns[column-1]);\n"
			"		}\n\n"
			"	}\n"
			"	return analyzeObjects;\n"
			"}\n\n"

			"lineWithObjectStringData[] GetAnalyzeStringObjects(tree importedTablePt) {\n"
			"	ulong lastLine = GetULongByKeyFromTree(importedTablePt, \"LastLine\");\n"
			"	lineWithObjectStringData[lastLine] analyzeObjects;\n\n"
			"	for(ulong line = 2; line <= lastLine; line = line + 1;) {\n"
			"		string[] columns = GetTreeAsStringArray(GetChildByKeyFromTree(importedTablePt, ULongToString(line)));\n\n\n"
			"		analyzeObjects[line-2].objectName = columns[0];\n"
			"		ResizeArray(analyzeObjects[line-2].data, len(columns)-1);\n\n"
			"		for(ulong columnIndex = 1; columnIndex < len(columns); columnIndex = columnIndex + 1;) {\n"
			"			analyzeObjects[line-2].data[columnIndex-1] = columns[columnIndex];\n"
			"		}\n\n"
			"	}\n"
			"	return analyzeObjects;\n"
			"}\n\n"

			"tree UnanalyzedData(tree importedTable) {\n"
			"	tree analyzeResult;\n\n"
			"	"
			"	return analyzeResult;\n"
			"}\n"));


	// Adds bacis SubNodes to Nodes
	Node* nodeForAddingBasics = nullptr;

	// Add 1-st Node
	nodeForAddingBasics = GetNode(SWObjectNodeId);

	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectNodeId, SWObjectDataListFormat, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectInformationalCode, SWObjectInheritedVertexes, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectNodeCategor, SWObjectUnchangeableVertex, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectUnchangeableNode, SWObjectFreeVertexIds, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectVertexForAddToDb, SWObjectNodeForAddToDb, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectSubNodeForAddToDb, SWObjectVertexForChangeToDb, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectSubNodeForChangeToDb, SWObjectVertexForRemoveToDb, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectNodeForRemoveToDb, SWObjectSubNodeForRemoveToDb, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectProgramOfModules, SWObjectModulesName, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectModuleIdVertexList, 0, 255));

	// Add 2-nd in Node
	nodeForAddingBasics = GetNode(SWObjectDataListFormat);

	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectNodeId, SWObjectDataListFormat, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectInformationalCode, SWObjectUnchangeableVertex, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectUnchangeableNode, SWObjectFreeVertexIds, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectVertexForAddToDb, SWObjectNodeForAddToDb, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectVertexForChangeToDb, SWObjectVertexForRemoveToDb, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectNodeForRemoveToDb, 0, 255));
	
	// Add 11-th in Node
	nodeForAddingBasics = GetNode(SWObjectUnchangeableVertex);
	
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectInputUserRequest, SWObjectAdditionalToUserRequest, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectOutputSystemResponse, 0, 255));

	// Add 12-th id Node
	nodeForAddingBasics = GetNode(SWObjectUnchangeableNode);

	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectVertexForAddToDb, SWObjectNodeForAddToDb, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectSubNodeForAddToDb, SWObjectVertexForChangeToDb, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectSubNodeForChangeToDb, SWObjectVertexForRemoveToDb, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, SWObjectNodeForRemoveToDb, SWObjectSubNodeForRemoveToDb, 255));

	//// Linking modules
	// list of ids of exists module
	nodeForAddingBasics = GetNode(SWObjectModuleIdVertexList);

	nodeForAddingBasics->AddOrSetSubNode(SubNode(MainModuleIdInSW, DbControllerModuleIdInSW));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(IISModuleIdInSW, NetModuleIdInSW));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(ModuleManageIdInSW, PermissionManageIdInSW));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(UserControlIdInSW, CalculateDemographyIdInSW));

	// Modules names
	nodeForAddingBasics = GetNode(SWObjectModulesName);

	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, MainModuleIdInSW, SWObjectMainModuleNameId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, DbControllerModuleIdInSW, SWObjectDbControllerModuleNameId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, IISModuleIdInSW, SWObjectIISModuleNameId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, NetModuleIdInSW, SWObjectNetModuleNameId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, ModuleManageIdInSW, SWObjectModuleManageNameId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, PermissionManageIdInSW, SWObjectPermissionManageNameId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, UserControlIdInSW, SWObjectUserControlNameId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, CalculateDemographyIdInSW, SWObjectCalculateDemographyNameId, 255));

	// ProgramOfModules
	nodeForAddingBasics = GetNode(SWObjectProgramOfModules);

	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, MainModuleIdInSW, SWObjectMainModuleTextId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, DbControllerModuleIdInSW, SWObjectDbControllerModuleTextId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, IISModuleIdInSW, SWObjectIISModuleTextId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, NetModuleIdInSW, SWObjectNetModuleTextId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, ModuleManageIdInSW, SWObjectModuleManageTextId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, PermissionManageIdInSW, SWObjectPermissionManageTextId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, UserControlIdInSW, SWObjectUserControlTextId, 255));
	nodeForAddingBasics->AddOrSetSubNode(SubNode(0, CalculateDemographyIdInSW, SWObjectCalculateDemographyTextId, 255));


	// Set unusing basic elements by empty element
	for(vertexIdType nextVertexId = 1; nextVertexId < LastBasicElementsId; nextVertexId++) {
		if(GetVertex(nextVertexId) == nullptr) {
			AddOrSetVertex(new Vertex(nextVertexId));
		}
	}
}