#include "DijkstraDecreaseKey.h"
#include "DijkstraPriorityQueue.h"
#include "GraphLoader.h"
#include "TarjanYenBoth.h"
#include "TarjanYenOne.h"
#include "TarjanYenTwo.h"
#include "YenBoth.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <vector>

bool areFilesEqual(const std::string& file1, const std::string& file2)
{
    std::ifstream f1(file1);
    std::ifstream f2(file2);

    std::string line;
    std::string line2;
    while (std::getline(f1, line) && std::getline(f2, line2))
    {
        std::istringstream stream (line);
        std::istringstream stream2 (line2);

        int node, distance;
        int node2, distance2;
        
        stream >> node >> distance;
        stream2 >> node2 >> distance2;

        if (node != node2 || distance != distance2)
        {
            return false;
        }
    }

    return true;
}

void compareResults()
{
    std::string directory = "../result";
    std::vector<std::string> files;

    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file())
            files.push_back(entry.path().string());
    }

    if (files.size() < 2)
    {
        return;
    } 

    std::string referenceFile = files[0];

    for (size_t i {1}; i < files.size(); ++i)
    {
        if (!areFilesEqual(referenceFile, files[i]))
        {
            throw std::runtime_error("Output files do not match: " + referenceFile + " and " + files[i]);
        }
    }

    std::cout << "All result files are identical." << std::endl;
}

void runAlgorithm(std::unique_ptr<ISingleSourceShortestPath>& algo, const int source, std::vector<bool>& negativeCycles)
{
    auto start = std::chrono::high_resolution_clock::now();
        
    algo->run(source);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    negativeCycles.push_back(algo->hasNegativeCycle());

    std::cout   << "Algorithm " 
                << std::setw(30) << std::left << algo->getName() 
                << " execution time: " 
                << std::fixed << std::setprecision(6)
                << std::setw(12) << duration.count() 
                << " seconds"
                << std::endl;

    if (!std::filesystem::exists("../result")) {
        std::filesystem::create_directory("../result");
    }
    algo->printIntoFile("../result/" + algo->getName() + ".txt");

    if (!std::filesystem::exists("../times")) {
        std::filesystem::create_directory("../times");
    }
    std::ofstream outFile("../times/" + algo->getName() + ".txt", std::ios::app);
    if (outFile) {
        outFile  << duration.count() << "\t\t\t" << algo->getNodeVisitedCount() << std::endl;
    }
    outFile.close();
}

template <Weight T>
void testAll(const int source, const std::vector<Edge<T>>& edges)
{
    auto graph = std::make_shared<Graph<int>>(edges);

    std::vector<std::unique_ptr<ISingleSourceShortestPath>> algorithms;
    algorithms.emplace_back(std::make_unique<BellmanFord<T>>(graph));
    algorithms.emplace_back(std::make_unique<YenOne<T>>(graph));
    algorithms.emplace_back(std::make_unique<YenTwo<T>>(graph));
    algorithms.emplace_back(std::make_unique<YenBoth<T>>(graph));
    algorithms.emplace_back(std::make_unique<Tarjan<T>>(graph));
    algorithms.emplace_back(std::make_unique<TarjanYenOne<T>>(graph));
    algorithms.emplace_back(std::make_unique<TarjanYenTwo<T>>(graph));
    algorithms.emplace_back(std::make_unique<TarjanYenBoth<T>>(graph));

    std::vector<std::unique_ptr<ISingleSourceShortestPath>> algorithmsDijkstra;
    algorithmsDijkstra.emplace_back(std::make_unique<DijkstraPriorityQueue<T>>(graph));
    //algorithmsDijkstra.emplace_back(std::make_unique<DijkstraDecreaseKey<T>>(graph));

    std::vector<bool> negativeCycles;
    for (auto& algo : algorithms)
    {
        runAlgorithm(algo, source, negativeCycles);
    }

    if (!std::all_of(negativeCycles.begin(), negativeCycles.end(), [&](auto val) { return val == negativeCycles.front(); }))
    {
        throw std::runtime_error("Negative cycles detected incorrectly");
    }

    if (!negativeCycles.front())
    {
        for (auto& algo : algorithmsDijkstra)
        {
            runAlgorithm(algo, source, negativeCycles);
        }

        compareResults();
    }

    std::cout << std::endl;
}



int main()
{
    /*{
        std::cout << "Positive :)" << std::endl;
        std::vector<Edge<int>> edges {{1, 3, 2}, {4, 3, -1}, {2, 4, 1}, {1, 2, 1}, {0, 1, 5}};
        testAll(0, edges);
    }*/

    /*{
        std::cout << "Negative :(" << std::endl;
        std::vector<Edge<int>> edges {{0, 1, 4}, {1, 2, -6}, {2, 3, 5}, {3, 1, -2}};
        testAll(0, edges);
    }*/

    /*{
        std::cout << "One Graph :D" << std::endl;
        std::vector<Edge<int>> edges = loadEdgesFromFile<int>("..\\graphs\\USA-road-d.NY.gr");

        
        testAll(1, edges);
    }*/

    {
        std::cout << "All -_-" << std::endl;

        auto directory = "..\\..\\graphs";

        for (auto name : std::filesystem::directory_iterator(directory))
        {
            std::cout << name.path().string() << std::endl;

            std::vector<Edge<int>> edges = loadEdgesFromFile<int>(name.path().string());

            testAll(1, edges);
        }
    }

    return 0;
}
