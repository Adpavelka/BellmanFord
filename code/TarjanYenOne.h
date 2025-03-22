#pragma once

#include "Tarjan.h"
#include "YenOne.h"

template <Weight T>
class TarjanYenOne : public YenOne<T>, public Tarjan<T>
{
public:
    explicit TarjanYenOne(std::shared_ptr<Graph<T>> graph)
        : BellmanFord<T>(graph)
        , YenOne<T>(graph)
        , Tarjan<T>(graph)
    {
    }

    std::string getName() const override
    {
        return "TarjanYenOne";
    }

    ~TarjanYenOne() override = default;
};
