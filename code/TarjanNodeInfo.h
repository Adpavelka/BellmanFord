#pragma once

#include <optional>
#include <unordered_set>

struct TarjanNodeInfo
{
    bool                        reachable {true};
    std::optional<int>          parent {};
    std::unordered_set<int>     children {};
};