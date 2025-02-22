#include "DijkstraDecreaseKey.h"
#include "DijkstraPriorityQueue.h"
#include "GraphLoader.h"
#include "TarjanYenBoth.h"
#include "TarjanYenOne.h"
#include "TarjanYenTwo.h"
#include "YenBoth.h"

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <vector>


template <Weight T>
void testAll(const int source, const std::vector<Edge<T>>& edges)
{
    auto graph = std::make_shared<Graph<int>>(edges);

    std::vector<std::unique_ptr<ISingleSourceShortestPath>> algorithms;

    algorithms.emplace_back(std::make_unique<BellmanFord<T>>(graph));
    algorithms.emplace_back(std::make_unique<YenTwo<T>>(graph));

    //algorithms.emplace_back(std::make_unique<DijkstraPriorityQueue<T>>(graph));
    //algorithms.emplace_back(std::make_unique<DijkstraDecreaseKey<T>>(graph));
    //algorithms.emplace_back(std::make_unique<TarjanYenOne<T>>(graph));
    /*algorithms.emplace_back(std::make_unique<YenOne<T>>(graph));

    
    algorithms.emplace_back(std::make_unique<YenBoth<T>>(graph));
    algorithms.emplace_back(std::make_unique<Tarjan<T>>(graph));
    algorithms.emplace_back(std::make_unique<TarjanYenTwo<T>>(graph));
    algorithms.emplace_back(std::make_unique<TarjanYenBoth<T>>(graph));*/



    for (auto& algo : algorithms) {
        auto start = std::chrono::high_resolution_clock::now();
        
        algo->run(source);
        

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout   << "Algorithm " 
                    << std::setw(30) << std::left << algo->getName() 
                    << " execution time: " 
                    << std::fixed << std::setprecision(6)
                    << std::setw(12) << duration.count() 
                    << " seconds"
                    << std::endl;

        /*if (!std::filesystem::exists("result")) {
            std::filesystem::create_directory("result");
        }
        algo->printIntoFile("../result/" + algo->getName() + ".txt");*/

        std::ofstream outFile("large_graph/" + algo->getName() + ".txt", std::ios::app);
        if (outFile) {
            outFile  << duration.count() << std::endl;
        }
        outFile.close();
    }
}



int main()
{
    {
        std::cout << "Positive :)" << std::endl;
        std::vector<Edge<int>> edges {{1, 3, 2}, {4, 3, -1}, {2, 4, 1}, {1, 2, 1}, {0, 1, 5}};
        testAll(0, edges);
    }

    /*{
        std::cout << "Negative :(" << std::endl;
        std::vector<Edge<int>> edges {{0, 1, 4}, {1, 2, -6}, {2, 3, 5}, {3, 1, -2}};
        testAll(0, edges);
    }*/

    {
        std::cout << "Biiiiiig :D" << std::endl;
        std::vector<Edge<int>> edges = loadEdgesFromFile<int>("C:\\Users\\42072\\Desktop\\codes\\graphs\\USA-road-d.NY.gr");

        
        testAll(1, edges);
    }

    /*{
        std::cout << "Random :OOO" << std::endl;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> vertexDist(0, 5000);
        std::uniform_int_distribution<int> weightDist(1, 1000);

        std::vector<Edge<int>> edges;
        for (int i {}; i < 100000; ++i) {
            edges.emplace_back(vertexDist(gen), vertexDist(gen), weightDist(gen));
        }

        testAll(3333, edges);
    }*/

    /*{
        std::cout << "Full Graph + Random Shuffle 8)" << std::endl;

        for (int k {}; k < 20000; ++k) {
            if (k % 100 == 0) {
                std::cout << k << std::endl;
            }

            std::vector<Edge<int>> edges;
            for (int i {}; i < 200; ++i) {
                for (int j {i + 1}; j < 200; ++j) {
                    edges.emplace_back(i, j, -1);
                }
            }

            // shuffle
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(edges.begin(), edges.end(), g);

            testAll(0, edges);
        }
    }*/

    

    return 0;
}
