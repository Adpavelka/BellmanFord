#pragma once

#include "Tarjan.h"
#include "YenOne.h"
#include "YenTwo.h"

template <Weight T>
class TarjanYenBoth : public YenOne<T>, public YenTwo<T>, public Tarjan<T>
{
public:
    explicit TarjanYenBoth(std::shared_ptr<Graph<T>> graph)
        : BellmanFord<T>(graph)
        , YenOne<T>(graph)
        , YenTwo<T>(graph)
        , Tarjan<T>(graph)
    {
    }

    std::string getName() const override
    {
        return "TarjanYenBoth";
    }
};