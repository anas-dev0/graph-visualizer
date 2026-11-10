#include "core/Graph.h"

int Graph::addNode(float x, float y) {
    nodes_.push_back({x, y});
    adj_.emplace_back();
    return nodeCount() - 1;
}

int Graph::addEdge(int u, int v, int w) {
    bool valid = u >= 0 && v >= 0 && u < nodeCount() && v < nodeCount() && u != v && w >= 1;
    if (!valid) return -1;

    for (const auto& neighbor : adj_[u])
        if (neighbor.first == v) return -1;  // already connected

    int id = static_cast<int>(edges_.size());
    edges_.push_back({u, v, w});
    adj_[u].push_back({v, id});
    adj_[v].push_back({u, id});
    return id;
}

int Graph::nodeAt(float x, float y, float radius) const {
    for (int i = 0; i < nodeCount(); ++i) {
        float dx = nodes_[i].x - x;
        float dy = nodes_[i].y - y;
        if (dx * dx + dy * dy <= radius * radius) return i;
    }
    return -1;
}

void Graph::clear() {
    nodes_.clear();
    edges_.clear();
    adj_.clear();
}
