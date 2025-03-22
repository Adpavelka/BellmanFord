#pragma once

#include "Tarjan.h"
#include "YenTwo.h"


template <Weight T>
class TarjanYenTwo : public YenTwo<T>, public Tarjan<T>
{
public:
    explicit TarjanYenTwo(std::shared_ptr<Graph<T>> graph)
        : BellmanFord<T>(graph)
        , YenTwo<T>(graph)
        , Tarjan<T>(graph)
    {
    }

    std::string getName() const override
    {
        return "TarjanYenTwo";
    }

    ~TarjanYenTwo() override = default;
};
