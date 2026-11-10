#pragma once
#include <optional>
#include <string>
#include "core/Graph.h"

// Text format, one item per line (blank lines and lines starting with # are ignored):
//   n <x> <y>       a node; ids follow line order (first node is 0)
//   e <u> <v> <w>   an edge between nodes u and v with weight w
bool saveGraph(const Graph& g, const std::string& path);
std::optional<Graph> loadGraph(const std::string& path);  // nullopt if missing or malformed
