#pragma once

#include "Edge.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <Weight T>
std::vector<Edge<T>> loadEdgesFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    std::vector<Edge<T>> edges;

    if (!file.is_open())
    {
        std::cerr << "Could not open the file!" << std::endl;
        return edges;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream stream(line);
        std::string ignore; // each line starts with letter a for some reason
        int from, to;
        T weight;

        stream >> ignore;
        
        stream >> from >> to >> weight;

        edges.emplace_back(from, to, weight);
    }

    return edges;
}
