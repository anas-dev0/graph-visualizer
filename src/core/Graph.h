#pragma once
#include <utility>
#include <vector>

struct Node { float x, y; };   // screen position (plain floats, no SFML)
struct Edge { int u, v, w; };  // endpoints and weight

// Undirected weighted graph. Node ids are 0..n-1 in creation order.
class Graph {
public:
    int addNode(float x, float y);                      // returns the new node id
    int addEdge(int u, int v, int w);                   // returns the edge id, or -1 if invalid
    int nodeAt(float x, float y, float radius) const;   // node under (x, y), or -1
    void clear();

    int nodeCount() const { return static_cast<int>(nodes_.size()); }
    const std::vector<Node>& nodes() const { return nodes_; }
    const std::vector<Edge>& edges() const { return edges_; }

    // (neighbor id, edge id) pairs, in the order the edges were added.
    const std::vector<std::pair<int, int>>& neighbors(int u) const { return adj_[u]; }

private:
    std::vector<Node> nodes_;
    std::vector<Edge> edges_;
    std::vector<std::vector<std::pair<int, int>>> adj_;
};
