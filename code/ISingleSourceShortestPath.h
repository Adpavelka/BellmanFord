#pragma once

#include <string>

class ISingleSourceShortestPath
{
public:
    virtual void run(const int source) = 0;
    virtual void printPath(const int sink) const = 0;
    virtual void printIntoFile(const std::string& filename) const = 0;
    virtual std::string getName() const = 0;

    virtual bool hasNegativeCycle() const = 0;

    virtual ~ISingleSourceShortestPath() = default;
};
