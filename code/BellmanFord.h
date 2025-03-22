#pragma once

#include "Graph.h"
#include "ISingleSourceShortestPath.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_set>
#include <vector>

template <Weight T>
class BellmanFord : public ISingleSourceShortestPath
{
public:
    explicit BellmanFord(std::shared_ptr<Graph<T>> graph)
    : m_graph{std::move(graph)}
    {
        this->m_relaxActions.push_back([this](const Edge<T>& edge) { this->relaxEdge(edge); });
        this->m_iterationSteps.push_back([this]() { this->prepareForNextIteration(); });
        this->m_iterationSteps.push_back([this]() { this->iterationStep(); });

        m_vertices = &m_graph->getVertices();
    }

    void run(const int source) final
    {
        initialize(source);
        initializeHeuristics(source);

        for (int iteration {}; iteration < m_graph->size() && !isTerminationConditionMet(); ++iteration) // O(max(l, V) * E)
        {
            iterate();
        }

        checkNegativeCycle();
    }

    void printPath(const int sink) const final
    {
        if (!m_distances.contains(sink))
        {
            std::cout << "No path to sink " << sink << std::endl;
            return;
        }

        std::vector<int> path;
        int current {sink};

        while (m_parents.contains(current))
        {
            path.push_back(current);
            current = m_parents.at(current);
        }
        path.push_back(current);

        std::cout << "Path to " << sink << std::endl;
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

    /*void printDistances() const
    {
        for (const auto& [vertex, dist] : m_distances)
        {
            std::cout << vertex << " " << dist << std::endl;
        }
    }*/


    void printIntoFile(const std::string& filename) const final
    {
        std::ofstream outFile(filename);
        
        if (!outFile.is_open())
        {
            std::cerr << "Could not open the file for writing!" << std::endl;
            return;
        }

        std::map<int, T> sortedDistances(m_distances.begin(), m_distances.end());
        for (const auto& [vertex, dist] : sortedDistances)
        {
            outFile << vertex << " " << dist << std::endl;
        }

        outFile.close();
    }

    std::string getName() const override
    {
        return "BellmanFord";
    }

    bool hasNegativeCycle() const override
    {
        return m_negativeCycleDetected;
    }

    ~BellmanFord() override = default;

protected:
    virtual void iterationStep()
    {
        for (const auto& node : getVertices())
        {
            for (const auto& edge : getNodeEdges(node))
            {
                relaxIfPossible(edge);
            }
        }
    }

    void relaxIfPossible(const Edge<T>& edge)
    {
        if (canRelax(edge))
        {
            doRelaxActions(edge);
        }
    }

    virtual const std::vector<int>& getVertices()
    {
        return *m_vertices;
    }

    virtual const std::vector<Edge<T>>& getNodeEdges(const int node) const
    {
        return m_graph->getNodeEdges(node);
    }

    const std::vector<int>*                             m_vertices {};
    std::unordered_map<int, int>*                       m_order {};

    bool                                                m_negativeCycleDetected;     
    std::vector<std::function<void()>>                  m_iterationSteps;
    std::vector<std::function<void(const int)>>         m_heuristics;
    std::vector<std::function<void(const Edge<T>&)>>    m_relaxActions;

private:
    void initialize(const int source)
    {
        m_edgeRelaxed = true;
        m_negativeCycleDetected = false;
        m_parents.clear();
        m_distances.clear();
        m_distances[source] = 0;
    }

    void prepareForNextIteration()
    {
        m_edgeRelaxed = false;
    }

    void initializeHeuristics(const int source)
    {
        for (const auto& heuristic : m_heuristics)
        {
            heuristic(source);
        }
    }

    bool canRelax(const Edge<T>& edge) const
    {
        return  m_distances.contains(edge.from) &&
                (!m_distances.contains(edge.to) || m_distances.at(edge.from) + edge.weight < m_distances.at(edge.to));
    }

    void doRelaxActions(const Edge<T>& edge)
    {
        for (const auto& relaxAction : m_relaxActions)
        {
            relaxAction(edge);
        }
    }

    void relaxEdge(const Edge<T>& edge)
    {
        m_distances[edge.to] = m_distances[edge.from] + edge.weight;
        m_parents[edge.to] = edge.from;
        m_edgeRelaxed = true;
    }

    bool isTerminationConditionMet() const
    {
        return m_negativeCycleDetected || !m_edgeRelaxed;
    }

    void iterate()
    {
        for (const auto& iterationStep : m_iterationSteps)
        {
            iterationStep();
        }
    }

    void checkNegativeCycle() // const
    {
        if (m_negativeCycleDetected)
        {
            // std::cout << "Negative cycle detected." << std::endl;
            return;
        }
        
        for (const auto& edge : m_graph->getEdges())
        {
            if (canRelax(edge))
            {
                // std::cout << "Negative cycle detected." << std::endl;
                m_negativeCycleDetected = true;
                return;
            }
        }
    }

    bool                            m_edgeRelaxed;
    std::shared_ptr<Graph<T>>       m_graph;
    std::unordered_map<int, T>      m_distances;
    std::unordered_map<int, int>    m_parents;
};
