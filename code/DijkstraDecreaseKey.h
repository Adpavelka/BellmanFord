#pragma once

#include "Dijkstra.h"

#include <limits>
#include <unordered_set>

template <Weight T>
class DijkstraDecreaseKey : public Dijkstra<T>
{
public:
    explicit DijkstraDecreaseKey(std::shared_ptr<Graph<T>> graph)
    : Dijkstra<T>(graph) // move? zjisti detaily
    {

    }

    void run(const int source) final  // O(V ^ 2)
    {
        this->initialize(source);
        
        std::unordered_set<int> visited;
        
        while (ssize(visited) < this->m_graph->size())
        {
            int node = selectMinDistanceNode(visited);
            if (node == -1)
            {
                break;
            }

            visited.insert(node);
            
            for (const auto& [_, to, weight] : this->m_graph->getNodeEdges(node))
            {
                if (!visited.contains(to) || this->m_distances[node] + weight < this->m_distances[to])
                {
                    this->m_distances[to] = this->m_distances[node] + weight;
                    this->m_parents[to] = node;
                }
            }
        }
    }

    std::string getName() const final
    {
        return "Dijkstra - Decrease Key";
    }

    ~DijkstraDecreaseKey() override = default;

private:
    int selectMinDistanceNode(const std::unordered_set<int>& visited) const
    {
        int minNode {-1};
        T minDistance = std::numeric_limits<T>::max();

        for (const auto& [vertex, distance] : this->m_distances)
        {
            if (!visited.contains(vertex) && distance < minDistance)
            {
                minDistance = distance;
                minNode = vertex;
            }
        }

        return minNode;
    }
};