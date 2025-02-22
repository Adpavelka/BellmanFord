#pragma once

#include "YenOne.h"
#include "YenTwo.h"

template <Weight T>
class YenBoth : public YenOne<T>, public YenTwo<T>
{
public:
    explicit YenBoth(std::shared_ptr<Graph<T>> graph)
        : BellmanFord<T>(graph)
        , YenOne<T>(graph)
        , YenTwo<T>(graph)
    {
    }

    std::string getName() const override
    {
        return "YenBoth";
    }
};