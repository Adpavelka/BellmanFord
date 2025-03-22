#pragma once

#include "Graph.h"
#include "ISingleSourceShortestPath.h"

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>

template <Weight T>
class Dijkstra : public ISingleSourceShortestPath
{
public:
    explicit Dijkstra(std::shared_ptr<Graph<T>> graph)
    : m_graph{std::move(graph)}
    {

    }

    void printPath(const int sink) const final
    {
        if (!m_distances.contains(sink))
        {
            std::cout << "No path to sink " << sink << std::endl;
            return;
        }

        std::vector<int> path;
        int current = sink;
        
        while (m_parents.contains(current))
        {
            path.push_back(current);
            current = m_parents.at(current);
        }
        path.push_back(current);

        std::cout << "Path to " << sink << ": ";
        for (auto it = path.rbegin(); it != path.rend(); ++it)
        {
            std::cout << *it;
            if (std::next(it) != path.rend())
            {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }

    void printIntoFile(const std::string& filename) const final
    {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            std::cerr << "Could not open the file for writing!" << std::endl;
            return;
        }

        std::map<int, T> sortedDistances(m_distances.begin(), m_distances.end());
        for (const auto& [vertex, dist] : sortedDistances)
        {
            outFile << vertex << " " << dist << std::endl;
        }
    }

    bool hasNegativeCycle() const override
    {
        return false;
    }

    ~Dijkstra() override = default;

protected:
    void initialize(const int source)
    {
        m_distances.clear();
        m_parents.clear();
        m_distances[source] = 0;
    }

    std::shared_ptr<Graph<T>>       m_graph;
    std::unordered_map<int, T>      m_distances;
    std::unordered_map<int, int>    m_parents;
};
