#pragma once

#include "BellmanFord.h"

#include <algorithm>
#include <ranges>

template <Weight T>
class YenOne : public virtual BellmanFord<T>
{
public:
    explicit YenOne(std::shared_ptr<Graph<T>> graph)
    : BellmanFord<T>(graph)
    {
       this->m_heuristics.push_back([this](const int source) { initialize(source); });
       this->m_relaxActions.push_back([this](const Edge<T>& edge) { this->addActiveNode(edge); });
       this->m_iterationSteps.insert(this->m_iterationSteps.begin(), [this]() { this->prepareForNextIteration(); });
    }

    std::string getName() const override
    {
        return "YenOne";
    }

    ~YenOne() override = default;

private:
    void initialize(const int source)
    {
        m_verticesSet.clear();
        m_activeVertices.clear();
        m_nextActiveVertices.clear();
        m_nextActiveVertices.push_back(source);
    }

    void addActiveNode(const Edge<T>& edge)
    {
        if (auto [it, inserted] = m_verticesSet.insert(edge.to); inserted)
        {
            m_nextActiveVertices.push_back(edge.to);
        }
    }

    void prepareForNextIteration()
    {
        m_activeVertices.swap(m_nextActiveVertices);
        m_nextActiveVertices.clear();
        m_verticesSet.clear();
    }

    const std::vector<int>& getVertices() override
    {
        if (this->m_order != nullptr)
        {
            std::ranges::sort(m_activeVertices, [this](int a, int b) {
                return (*this->m_order)[a] < (*this->m_order)[b]; // at na vyjímku? return this->m_order->at(a) < this->m_order->at(b); 
            });
        }
        return m_activeVertices;
    }

    std::vector<int>        m_nextActiveVertices;
    std::vector<int>        m_activeVertices;

    std::unordered_set<int> m_verticesSet;
};
