#pragma once

#include "BellmanFord.h"

#include <algorithm>
#include <random>
#include <ranges>

template <Weight T>
class YenTwo : public virtual BellmanFord<T>  //  Bannister & Eppstein (2012)’s improvement????
{
public:
    explicit YenTwo(std::shared_ptr<Graph<T>> graph)
    : BellmanFord<T>(graph)
    {
        this->m_heuristics.push_back([this](const int source) { this->initialize(source); });
        this->m_iterationSteps.push_back([this]() { this->reversedOrderIteration(); });

        splitEdges();
    }

    std::string getName() const override
    {
        return "YenTwo";
    }

    ~YenTwo() override = default;

private:
    void initialize(const int)
    {
        m_reverseDirection = false;
        this->m_order = &m_classicalOrder; // shared_ptr
        this->m_vertices = &m_classicalVertices; // shared_ptr
    }

    void reversedOrderIteration()
    {
        m_reverseDirection = true;
        this->m_order = &m_reverseOrder; // shared_ptr
        this->m_vertices = &m_reversedVertices; // shared_ptr
        this->iterationStep();

        m_reverseDirection = false;
        this->m_order = &m_classicalOrder; // shared_ptr
        this->m_vertices = &m_classicalVertices; // shared_ptr
    }

    void splitEdges()
    {
        m_classicalVertices = BellmanFord<T>::getVertices();

        std::random_device rd;
        std::mt19937 g(rd());
        std::ranges::shuffle(m_classicalVertices, g);

        m_reversedVertices = m_classicalVertices;
        std::ranges::reverse(m_reversedVertices);

        for (size_t i {}; i < m_classicalVertices.size(); ++i)
        {
            m_classicalOrder[m_classicalVertices[i]] = static_cast<int>(i);
            m_reverseOrder[m_classicalVertices[i]] = static_cast<int>(m_classicalVertices.size() - i);
        }

        for (const auto& node : m_classicalVertices)
        {
            for (const auto& edge : BellmanFord<T>::getNodeEdges(node))
            {
                if (m_classicalOrder[edge.from] < m_classicalOrder[edge.to])
                {
                    m_edgesInOrder[node].push_back(edge);
                }
                else
                {
                    m_edgesInReversed[node].push_back(edge);
                }
            }
        }

        this->m_vertices = &m_classicalVertices; // shared_ptr
        this->m_order = &m_classicalOrder; // shared_ptr
    }

    virtual const std::vector<Edge<T>>& getNodeEdges(const int node) const
    {

        if (m_reverseDirection)
        {
            auto it = m_edgesInReversed.find(node);
            return (it == m_edgesInReversed.end()) ? m_emptyEdgeList : it->second;
        }
        else
        {
            auto it = m_edgesInOrder.find(node);
            return (it == m_edgesInOrder.end()) ? m_emptyEdgeList : it->second;
        }
    }

    std::unordered_map<int, int>                    m_classicalOrder;
    std::unordered_map<int, int>                    m_reverseOrder;

    std::vector<int>                                m_classicalVertices;
    std::vector<int>                                m_reversedVertices;


    bool                                            m_reverseDirection;
    static inline const std::vector<Edge<T>>        m_emptyEdgeList = {};
    std::unordered_map<int, std::vector<Edge<T>>>   m_edgesInOrder;
    std::unordered_map<int, std::vector<Edge<T>>>   m_edgesInReversed;
};
