#pragma once

#include "Edge.h"

#include <algorithm>
#include <random>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <Weight T>
class Graph
{
public:
    explicit Graph(std::vector<Edge<T>> edges)
    : m_edges(std::move(edges))
    {
        std::unordered_set<int> vertices;
        for (const auto& edge : m_edges)
        {
            m_adjacent[edge.from].push_back(edge);

            if (auto [_, inserted] = vertices.insert(edge.from); inserted) // keep order 
            {
                m_vertices.push_back(edge.from);
            }
            if (auto [_, inserted] = vertices.insert(edge.to); inserted)
            {
                m_vertices.push_back(edge.to);
            }
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::ranges::shuffle(m_vertices, g);
    }

    const std::vector<Edge<T>>& getNodeEdges(const int node) const
    {
        auto it = m_adjacent.find(node);
        return (it == m_adjacent.end()) ? m_emptyEdgeList : it->second;
    }

    const std::vector<Edge<T>>& getEdges() const
    {
        return m_edges;
    }

    const std::vector<int>& getVertices() const
    {
        return m_vertices;
    }

    int size() const
    {
        return std::ssize(m_vertices);
    }

private:
    static inline const std::vector<Edge<T>>        m_emptyEdgeList = {};
    std::unordered_map<int, std::vector<Edge<T>>>   m_adjacent;
    std::vector<int>                                m_vertices;
    std::vector<Edge<T>>                            m_edges;
};
