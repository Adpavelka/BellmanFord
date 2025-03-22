#pragma once

#include "BellmanFord.h"
#include "TarjanNodeInfo.h"

template <Weight T>
class Tarjan : public virtual BellmanFord<T>
{
public:
    explicit Tarjan(std::shared_ptr<Graph<T>> graph)
    : BellmanFord<T>(graph)
    {
        this->m_heuristics.push_back([this](const int source) { this->initialize(source); });
        this->m_relaxActions.insert(this->m_relaxActions.begin(), [this](const Edge<T>& edge) { this->tarjanRelax(edge); });
    }

    std::string getName() const override
    {
        return "Tarjan";
    }

    ~Tarjan() override = default;

private:
    void iterationStep() override
    {
        for (const auto& node : this->getVertices())
        {
            if (!m_pointerGraph[node].reachable)
            {
                continue;
            }

            for (const auto &edge : this->getNodeEdges(node))
            {
                this->relaxIfPossible(edge);

                if (this->m_negativeCycleDetected)
                {
                    return;
                }
            }
        }
    }

    void initialize(const int)
    {
        m_pointerGraph.clear();
    }

    void tarjanRelax(const Edge<T>& edge)
    {
        if (m_pointerGraph[edge.to].parent.has_value())
        {
            m_pointerGraph[m_pointerGraph[edge.to].parent.value()].children.erase(edge.to); //Vymazse ze sezmenau deti sveho rodice
        }

        m_pointerGraph[edge.to].reachable = true;
        m_pointerGraph[edge.to].parent = edge.from;
        m_pointerGraph[edge.from].children.insert(edge.to);

        if (subTreeContains(edge.to, edge.from)) // verb??? 
        {
            this->m_negativeCycleDetected = true;
        }
    }

    bool subTreeContains(const int root, const int target)
    {
        if (root == target)
        {
            return true;
        }

        for (const auto child : m_pointerGraph[root].children) // at ? 
        {
            m_pointerGraph[child].reachable = false;
            m_pointerGraph[child].parent.reset();

            if (subTreeContains(child, target))
            {
                return true;
            }
        }
        m_pointerGraph[root].children.clear();

        return false;
    }

    std::unordered_map<int, TarjanNodeInfo> m_pointerGraph;
};
