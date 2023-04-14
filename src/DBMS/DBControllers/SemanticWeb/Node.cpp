#include "../../../../include/DBMS/DbControllers/Relational/Node.h"

vertexIdType Node::GetNodeId() const{
    return m_nodeId;
}

void Node::SetNodeId(vertexIdType newNodeId) {
    m_nodeId = newNodeId;
}

vector<SubNode>* Node::GetSubNodes() const{
    return m_subNodes;
}

vector<SubNode>* Node::GetCloneSubNodes() const {
    vector<SubNode>* cloneSubNodes = new vector<SubNode>;

    for (SubNode& ownSubNode : *m_subNodes) {
        cloneSubNodes->push_back(ownSubNode);
    }

    return cloneSubNodes;
}

void Node::SetSubNodes(vector<SubNode>* subNodes) {
    m_subNodes = subNodes;
}

void Node::ReplaceSubNodes(vector<SubNode>* subNodes) {
    if(m_subNodes != nullptr)
        delete m_subNodes;

    SetSubNodes(subNodes);
}

void Node::AddOrSetSubNode(SubNode addSubNode) {
    // If need next last subNode's id
    if (addSubNode.GetId() == 0) {
        if (m_subNodes->size() > 0) { // If m_subNodes is not empty
            addSubNode.SetId(m_subNodes->back().GetId() + 1);
        }
        else { // If m_subNodes is empty
            addSubNode.SetId(1);
        }
    }

    // If m_subNodes is empty
    if (m_subNodes->size() == 0) {
        m_subNodes->push_back(addSubNode);

        return;
    }

    for (auto iterator = m_subNodes->begin(); iterator != m_subNodes->end(); iterator++) {
        if (iterator->GetId() == addSubNode.GetId()) { // If it replace parametrs of exists SubNode
            iterator->SetFirstVertexId(addSubNode.GetFirstVertexId());
            iterator->SetSecondVertexId(addSubNode.GetSecondVertexId());
            iterator->SetOwn(addSubNode.GetOwn());

            return;
        }
        else if (iterator->GetId() > addSubNode.GetId()) {
            m_subNodes->insert(iterator, addSubNode);

            return;
        }
    }

    // If id is next last subNode's id
    m_subNodes->push_back(addSubNode);
}

SubNode Node::GetSubNode(const vertexIdType subNodeId) const{
    SubNode subNode;

    for (SubNode subNode : *m_subNodes) {
        if (subNode.GetId() == subNodeId) {
            return subNode;
        }
    }

    return SubNode();
}

void Node::RemoveSubNode(vertexIdType subNodeId) {
    // Search SubNode index in pairs, where SubNode id equip subNodeId 
    for (auto iterator = m_subNodes->begin();
        iterator < m_subNodes->end() && iterator->GetId() <= subNodeId;
        iterator++) {
        if (iterator->GetId() == subNodeId) {
            m_subNodes->erase(iterator);

            return;
        }
    }
}

vector<SubNode> Node::FindByVertexIdAnyWhere(vertexIdType vertexId) const {
    return FindByVertexIdsAnyWhere(vector<vertexIdType> { vertexId });
}

vector<SubNode> Node::FindByVertexIdsAnyWhere(vector<vertexIdType>& vertexIds) const {
    function<bool(SubNode& inArg)> predicate = [&vertexIds](SubNode& subNode) {
        for (vertexIdType vertexId : vertexIds) {
            if (subNode.GetFirstVertexId() == vertexId || subNode.GetSecondVertexId() == vertexId) {
                return true;
            }
        }

        return false;
    };

    return FindByPredicate(predicate);
}

vector<SubNode> Node::FindByFirstVertexId(const vertexIdType firstVertexId) const {
    return FindByFirstVertexIds(vector<vertexIdType> { firstVertexId });
}

vector<SubNode> Node::FindByFirstVertexIds(vector<vertexIdType>& firstVertexIds) const {
    function<bool(SubNode& inArg)> predicate = [&firstVertexIds](SubNode& subNode) {
        return find(firstVertexIds.begin(), firstVertexIds.end(), subNode.GetFirstVertexId())
            != firstVertexIds.end();
    };

    return FindByPredicate(predicate);
}


vector<SubNode> Node::FindBySecondVertexId(const vertexIdType secondVertexId) const {
    return FindBySecondVertexIds(vector<vertexIdType> { secondVertexId });
}

vector<SubNode> Node::FindBySecondVertexIds(vector<vertexIdType>& secondVertexIds) const {
    function<bool(SubNode& inArg)> predicate = [&secondVertexIds](SubNode& subNode) {
        return find(secondVertexIds.begin(), secondVertexIds.end(), subNode.GetSecondVertexId())
            != secondVertexIds.end();
    };
    
    return FindByPredicate(predicate);
}


vector<SubNode> Node::FindByBothVertexIds(const vertexIdType firstVertexId, const vertexIdType secondVertexId) const {
    function<bool(SubNode& inArg)> predicate = [&firstVertexId, &secondVertexId](SubNode& subNode) {
        return subNode.GetFirstVertexId() == firstVertexId
            && subNode.GetSecondVertexId() == secondVertexId;
    };

    return FindByPredicate(predicate);
}

vector<vertexIdType> Node::GetVertexIdsVector() const {
    vector<vertexIdType> ints;

    for (SubNode subNode : *m_subNodes) {
        ints.push_back(subNode.GetFirstVertexId());
        ints.push_back(subNode.GetSecondVertexId());
    }

    if (ints.size() > 0 && ints.back() == 0) {
        ints.pop_back();
    }

    return ints;
}


void Node::SetVertexIdsVector(vector<vertexIdType>& vertexIds) {
    bool isAddedZero = false;
    
    if (vertexIds.size() % 2 == 1) {
        vertexIds.push_back(0);
        isAddedZero = true;
    }

    int index = 0;

    m_subNodes->clear();
    delete m_subNodes;

    m_subNodes = new vector<SubNode>;

    for (size_t index = 1; index <= vertexIds.size()/2; index++) {
        m_subNodes->push_back(
            SubNode(index,
                vertexIds.at((int)index * 2-2),
                vertexIds.at((int)index * 2-1), 255)
        );
    }

    if (isAddedZero) {
        vertexIds.pop_back();
    }
}

void Node::SortSubNodesAsVertexIdsVector() {
    auto vertexIds = GetVertexIdsVector();

    sort(vertexIds.begin(), vertexIds.end());

    SetVertexIdsVector(vertexIds);
}

void Node::RemoveVertexIdsVectorId(const vector<vertexIdType>& vertexIdsForRemove) {
    if (vertexIdsForRemove.size() == 0) {
        return;
    }

    auto vertexIds = GetVertexIdsVector();
    vector<vertexIdType> vertexIdsAfterRemoved;

    for (vertexIdType vertexId : vertexIds) {
        if (find(vertexIdsForRemove.begin(), vertexIdsForRemove.end(),
                vertexId) == vertexIdsForRemove.end()) {
            vertexIdsAfterRemoved.push_back(vertexId);
        }
    }

    SetVertexIdsVector(vertexIdsAfterRemoved);
}

vector<SubNode> Node::FindByPredicate(function<bool(SubNode& inArg)> predicate) const {
    vector<SubNode> subNodes;

    for (SubNode subNode : *m_subNodes) {
        if (predicate(subNode)) {
            subNodes.push_back(subNode);
        }
    }

    return subNodes;
}
