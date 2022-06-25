#pragma once
#include <cstdint>
#include<fstream>

#include "CommonTypeDef.h"

using std::fstream;

// Class for containt a pair id of tho nodes and them similary
class SubNode final {
private:
    // Local pair id for movement between SubNodes vector
    vertexIdType m_id = 0;

    // First vertex's id
    vertexIdType m_vertexId1 = 0;

    // Second vertex's id
    vertexIdType m_vertexId2 = 0;

    // Byte format similary between first vertex and second vertex
    uint8_t m_own = 0;

public:
    // Constructor for create an empty SubNode object
    SubNode() noexcept {};

    // Constructors for create a SubNode object with data
    SubNode(const vertexIdType vertexId1, const vertexIdType vertexId2) noexcept : m_vertexId1(vertexId1), m_vertexId2(vertexId2) {};

    SubNode(const vertexIdType vertexId1, const vertexIdType vertexId2, uint8_t own) noexcept : SubNode(vertexId1, vertexId2) {
        m_own = own;
    }

    SubNode(const vertexIdType id, const vertexIdType vertexId1, const vertexIdType vertexId2, const uint8_t own) noexcept : SubNode(vertexId1, vertexId2, own) {
        m_id = id;
    };

    // Set local SubNode id
    void SetId(const vertexIdType id) noexcept {
        m_id = id;
    };

    // Get local SubNode id
    vertexIdType GetId() const noexcept {
        return m_id;
    };

    // Get id from vertexId1
    vertexIdType GetFirstVertexId() const noexcept {
        return m_vertexId1;
    };

    // Set id to vertetxId1
    void SetFirstVertexId(const vertexIdType newVertexId1) noexcept {
        m_vertexId1 = newVertexId1;
    };

    // Get id from vertexId2
    vertexIdType GetSecondVertexId() const noexcept {
        return m_vertexId2;
    };

    // Set id to vertexId2
    void SetSecondVertexId(const vertexIdType newVertexId2) noexcept {
        m_vertexId2 = newVertexId2;
    };

    // Get byte from own between vertexId1 and vertexId2 vertexies
    uint8_t GetOwn() const noexcept {
        return m_own;
    };

    // Set byte to own vetween vertexId1 and vertexId2 vertexies
    void SetOwn(const uint8_t newOwn) noexcept {
        m_own = newOwn;
    };

    // Operators
    bool operator==(const SubNode& subNode) const noexcept {
        return subNode.GetId() == m_id
            && subNode.GetFirstVertexId() == m_vertexId1
            && subNode.GetSecondVertexId() == m_vertexId2
            && subNode.GetOwn() == m_own;
    }

    bool operator!=(const SubNode& subNode) const noexcept {
        return !(*this == subNode);
    }

    bool operator==(const vertexIdType subNodeId) const noexcept {
        return m_id == subNodeId;
    }
};