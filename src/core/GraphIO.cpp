#include "core/GraphIO.h"
#include <fstream>
#include <sstream>

bool saveGraph(const Graph& g, const std::string& path) {
    std::ofstream out(path);
    if (!out) return false;
    for (const Node& n : g.nodes()) out << "n " << n.x << ' ' << n.y << '\n';
    for (const Edge& e : g.edges()) out << "e " << e.u << ' ' << e.v << ' ' << e.w << '\n';
    return static_cast<bool>(out);
}

std::optional<Graph> loadGraph(const std::string& path) {
    std::ifstream in(path);
    if (!in) return std::nullopt;

    Graph g;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string kind;
        if (!(ss >> kind) || kind[0] == '#') continue;  // blank line or comment

        if (kind == "n") {
            float x, y;
            if (!(ss >> x >> y)) return std::nullopt;
            g.addNode(x, y);
        } else if (kind == "e") {
            int u, v, w;
            if (!(ss >> u >> v >> w) || g.addEdge(u, v, w) == -1) return std::nullopt;
        } else {
            return std::nullopt;  // unknown line type
        }
    }
    return g;
}
