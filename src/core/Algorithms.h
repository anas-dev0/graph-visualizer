#pragma once
#include <vector>
#include "core/Graph.h"
#include "core/Step.h"

// Each algorithm runs to completion and returns the steps it took.
// An invalid start node returns an empty list.
namespace algo {
std::vector<Step> bfs(const Graph& g, int start);
std::vector<Step> dfs(const Graph& g, int start);
std::vector<Step> dijkstra(const Graph& g, int start);
}
