#pragma once
#include <string>
#include<fstream>

#include "CommonTypeDef.h"

using std::string, std::fstream;

struct Vertex final {
private:
    vertexIdType m_id = 0;
    string m_text = "";

public:
    Vertex() noexcept {};

    Vertex(const vertexIdType id) : Vertex(id, "") {};

    Vertex(const string text) : Vertex(0, text) {};

    Vertex(const vertexIdType id, const string text) : m_id(id), m_text(text) {};


    void SetId(const vertexIdType newId) noexcept {
        m_id = newId;
    }

    vertexIdType GetId() const noexcept {
        return m_id;
    }

    void SetText(const string newText) {
        m_text = newText;
    }

    string GetText() const {
        return m_text;
    }

    Vertex* Clone() const {
        return new Vertex(m_id, m_text);
    }

    // Operators
    bool operator==(Vertex const& vertex) const {
        return vertex.GetId() == m_id && vertex.GetText() == m_text;
    }

    bool operator!=(Vertex const& vertex) const {
        return vertex.GetId() != m_id || vertex.GetText() != m_text;
    }

    bool operator==(const int vertexId) noexcept {
        return m_id == vertexId;
    }
};