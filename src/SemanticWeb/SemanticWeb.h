#pragma once
#include <algorithm>

#include "CommonTypeDef.h"

#include "ReservedSemanticObjects.h"

#include "vertex.h"
#include "node.h"

using std::find;

class SemanticWeb final {
private:
    // Vector of vertexies
    vector <Vertex*>* m_vertexies;

    // Vector of nodes
    vector <Node*>* m_nodes;

    // Count of basic elements, that cannot be using by user
    const vertexIdType LastBasicElementsId = 60;

public:
    SemanticWeb() : m_vertexies(new vector<Vertex*>), m_nodes(new vector<Node*>) {};

    // Constructor for full Semantic Web by Basci elements
    SemanticWeb(bool fullBasicElements) : SemanticWeb() {
        if (fullBasicElements) {
            fullByBasicElements();
        }
    };

    vertexIdType GetLastBasicElementsId() const;


    // Operations with Vertex vector

    // Get link to m_vertexies
    vector<Vertex*>* GetVertexes() const;

    // Set argument link as m_vertexies without delete older vector link
    void SetVertexes(vector<Vertex*>* vertexes);

    /*
    * Delete all older vertexies by 'delete'
    * Set argument link as m_vertexies
    */
    void ReplaceVertexes(vector<Vertex*>* vertexes);


    // Operation with Node vector

    // Get link to m_nodes
    vector<Node*>* GetNodes() const;

    // Set argument link as m_nodes without delete older vector link
    void SetNodes(vector<Node*>* nodes);

    /*
    * Delete all older nodes by 'delete'
    * Set argument link as m_nodes
    */
    void ReplaceNodes(vector<Node*>* nodes);

    /*
    * Delete all data in vertexes and nodes vector and to clear them
    */
    void Clear();


    // Operations with objects of Vertexies

    /*
    *  Append vertex to semantic web
    *  If object with Vertex argument id exists, it will replace by new object
    */
    void AddOrSetVertex(Vertex* addVertex);

    /*
    *  Get Vertex, that have such id, for get or set arguments from it
    *  If vertex was not found return nullptr
    *  So, check nullptr
    */
    Vertex* GetVertex(const vertexIdType id) const;

    // Remove vertex, that have such id, from vertexies list
    void RemoveVertex(const vertexIdType id);


    // Operations with objects of nodes
   
    /*
    *  Append node to semantic web
    *  If object with Vertex argument id exists, it will replace by new object
    */
    void AddOrSetNode(Node* newNode);

    /*
    *  Get Node, that have such id, for get or set arguments from it
    *  If node was not found return nullptr
    *  So my advice is check nullptr
    */
    Node* GetNode(const vertexIdType nodeId) const;

    // Remove node, that have such id, from nodes list
    void RemoveNode(const vertexIdType nodeId);


    // Operations for find objects in vertexies
    
    /*
    *  Find vertex, where vertex's text compare method string argument
    *  If not find, will be return 0
    */
    vector<Vertex*>* FindVertexEqualText(string text) const;

    // Find vertex, where vertex's text like method string argument
    vector<Vertex*>* FindVertexLikeText(string text) const;

    /*
    * Filling this Semantic Web by defined basic elements
    * Undefined basic elements will not fulling to this Semantic Web
    */
    void fullByBasicElements();


    SemanticWeb* Clone() const {
        SemanticWeb* sw = new SemanticWeb();

        for (auto iterator = m_vertexies->begin(); iterator != m_vertexies->end(); iterator++) {
            sw->GetVertexes()->push_back((*iterator)->Clone());
        }

        for (auto iterator = m_nodes->begin(); iterator != m_nodes->end(); iterator++) {
            sw->GetNodes()->push_back((*iterator)->Clone());
        }

        return sw;
    }


    ~SemanticWeb() {
        for (auto iterator = m_vertexies->begin(); iterator != m_vertexies->end(); iterator++) {
            delete (*iterator);
        }

        delete m_vertexies;

        for (auto iterator = m_nodes->begin(); iterator != m_nodes->end(); iterator++) {
            delete (*iterator);
        }

        delete m_nodes;
    }

    // Operators
    bool operator==(SemanticWeb const& sw1) const {
        vector<Vertex*>* sw1Vertexes = sw1.GetVertexes();

        for (auto vertex : (* m_vertexies)) {
            for (auto sw1Vertex : (*sw1Vertexes)) {
                if (*vertex != *sw1Vertex) {
                    return false;
                }
            }
        }

        vector<Node*>* sw1Nodes = sw1.GetNodes();

        for (auto node : (*m_nodes)) {
            for (auto sw1Node : (*sw1Nodes)) {
                if (*node != *sw1Node) {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator!=(SemanticWeb const& sw1) const {
        return !(*this == sw1);
    }
};