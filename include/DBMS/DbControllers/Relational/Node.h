#pragma once
#include <algorithm>
#include <functional>
#include <vector>
#include<fstream>

#include "subNode.h"

using std::vector, std::function, std::fstream;

class Node final {
private:
    // Id of Node in plently of Nodes between Vertexies
    vertexIdType m_nodeId = 0;

    // Plently of SubNodes with attributical Vertex where id equip nodeId
    vector<SubNode>* m_subNodes = nullptr;

    vector<SubNode> FindByPredicate(function<bool(SubNode& inArg)> predicate) const;

public:
    Node() : m_subNodes(new vector<SubNode>) {};

    Node(const vertexIdType nodeId) {
        m_subNodes = new vector<SubNode>;
        m_nodeId = nodeId;
    }

    Node(vector<SubNode>* subNodes) noexcept : m_subNodes(subNodes) {};

    Node(const vertexIdType nodeId, vector<SubNode>* subNodes) : Node(subNodes) {
        m_nodeId = nodeId;
    }

    // Get attributical vertex id
    vertexIdType GetNodeId() const;

    // Set attributical vertex id
    void SetNodeId(const vertexIdType newNodeId);

    // Get link to m_subNodes
    vector<SubNode>* GetSubNodes() const;

    vector<SubNode>* GetCloneSubNodes() const;

    // Set argument link as m_subNodes without delete older vector link
    void SetSubNodes(vector<SubNode>* subNodes);

    /*
    * Delete all older subNodes by 'delete'
    * Set argument link as m_subNodes
    */
    void ReplaceSubNodes(vector<SubNode>* subNodes);


    // Operations with objects in vector

    /*
    * Append a new SubNode into pairs
    * Or Set its parametrs to exists SubNode
    * If subNode's id is 0, it will be set next last id
    * If m_subNodes is null, will set subNode' id to 1
    */
    void AddOrSetSubNode(SubNode subNode);

    // Get SubNode from pairs, where SubNode id equip subNodeId
    SubNode GetSubNode(const vertexIdType subNodeId) const;

    // Remove SubNode in m_subNodes, that contains that subNodeId
    void RemoveSubNode(const vertexIdType subNodeId);


    // Operations for find objects in m_subNodes

    vector<SubNode> FindByVertexIdAnyWhere(const vertexIdType vertexId) const;

    vector<SubNode> FindByVertexIdsAnyWhere(vector<vertexIdType>& vertexIds) const;


    vector<SubNode> FindByFirstVertexId(const vertexIdType firtsVertexId) const;

    vector<SubNode> FindByFirstVertexIds(vector<vertexIdType>& firtsVertexIds) const;


    vector<SubNode> FindBySecondVertexId(const vertexIdType secondsVertexId) const;

    vector<SubNode> FindBySecondVertexIds(vector<vertexIdType>& secondsVertexIds) const;


    vector<SubNode> FindByBothVertexIds(const vertexIdType firstVertexId, const vertexIdType secondVertexId) const;

    // Operations with present of m_subNodes as vector of numbers

    /*
    * Get all SubNodes from this Node and form it to vector of int
    * Where ints is objects, that contains in subNodes at position first and second vertexies id
    */
    vector<vertexIdType> GetVertexIdsVector() const;

    /*
      Pair vertex's ids, then create SubNode with each created pair
      After this method, please delete vertexIds, because it copy data of vertexIds but not set link
    */
    void SetVertexIdsVector(vector<vertexIdType>& vertexIds);

    /*
    * Get sorted vertex's ids and set it to m_subNodes
    * This method of sorting sort from 0, so result may begin by 0, 0, ...
    */
    void SortSubNodesAsVertexIdsVector();

    /*
    * Remove vertex's ids in SubNode in Vector present, then set result vector present to m_subNodes
    * After this method, please delete vertexIds, becutse it copy data of vertexIds but not set link
    */
    void RemoveVertexIdsVectorId(const vector<vertexIdType>& vertexIds);

    // Method for create a new Node with clone data of this Node with subNodes
    Node* Clone() const {
        Node* node = new Node(m_nodeId, GetCloneSubNodes());

        return node;
    }

    // Destructor for clear a Node's subNodes in RAM
    ~Node() {
        delete m_subNodes;
    }

    // Operators
    bool operator==(const Node& node) const {
        if (node.GetNodeId() != m_nodeId) {
            return false;
        }

        vector<SubNode>* nodeSubNodes = node.GetSubNodes();

        for (const SubNode& subNode : *m_subNodes) {
            bool copyIs = false;

            for (const SubNode& nSubNode : *nodeSubNodes) {
                if (subNode == nSubNode) {
                    copyIs = true;

                    break;
                }
            }

            if (!copyIs) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const Node& node) const {
        return !(*this == node);
    }
};
