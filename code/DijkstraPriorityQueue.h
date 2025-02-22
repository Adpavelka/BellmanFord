#pragma once

#include "Dijkstra.h"

#include <queue>

template <Weight T>
class DijkstraPriorityQueue : public Dijkstra<T>
{
public:
    explicit DijkstraPriorityQueue(std::shared_ptr<Graph<T>> graph)
    : Dijkstra<T>(graph)
    {

    }

    void run(const int source) final  // O (V + E * log V)
    {
        this->initialize(source);
        
        using Pair = std::pair<T, int>;
        std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq; 
        pq.emplace(0, source);
        
        while (!pq.empty())
        {
            const auto& [distance, node] = pq.top();
            pq.pop();

            if (distance > this->m_distances[node])
            {
                continue;
            }

            for (const auto& [_, to, weight] : this->m_graph->getNodeEdges(node))
            {
                if (!this->m_distances.contains(to) || this->m_distances[node] + weight < this->m_distances[to])
                {
                    this->m_distances[to] = this->m_distances[node] + weight;
                    this->m_parents[to] = node;
                    pq.emplace(this->m_distances[to], to);
                }
            }
        }
    }

    std::string getName() const final
    {
        return "Dijkstra - Priority Queue";
    }

    ~DijkstraPriorityQueue() override = default;
};