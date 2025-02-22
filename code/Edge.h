#pragma once

#include <concepts>

template <typename T>
concept Weight = std::integral<T> || std::floating_point<T>;

template <Weight T>
struct Edge
{
    int from;
    int to;
    T weight;

    Edge(int from, int to, T weight)
    : from{from}
    , to{to}
    , weight{weight}
    {
    }
};
