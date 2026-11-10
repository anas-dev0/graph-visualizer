#include "core/Algorithms.h"
#include <algorithm>
#include <functional>
#include <limits>
#include <numeric>
#include <queue>

namespace {

bool validNode(const Graph& g, int u) { return u >= 0 && u < g.nodeCount(); }

void dfsVisit(const Graph& g, int u, std::vector<bool>& seen, std::vector<Step>& steps) {
    seen[u] = true;
    steps.push_back({StepType::VisitNode, u});
    for (auto [v, e] : g.neighbors(u)) {
        steps.push_back({StepType::ConsiderEdge, e});
        if (seen[v]) continue;
        steps.push_back({StepType::AcceptEdge, e});
        dfsVisit(g, v, seen, steps);
    }
    steps.push_back({StepType::FinishNode, u});
}

}  // namespace

namespace algo {

std::vector<Step> bfs(const Graph& g, int start) {
    std::vector<Step> steps;
    if (!validNode(g, start)) return steps;

    std::vector<bool> seen(g.nodeCount(), false);
    std::queue<int> queue;
    seen[start] = true;
    queue.push(start);
    steps.push_back({StepType::VisitNode, start});

    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();
        for (auto [v, e] : g.neighbors(u)) {
            steps.push_back({StepType::ConsiderEdge, e});
            if (seen[v]) continue;
            seen[v] = true;
            queue.push(v);
            steps.push_back({StepType::AcceptEdge, e});
            steps.push_back({StepType::VisitNode, v});
        }
        steps.push_back({StepType::FinishNode, u});
    }
    return steps;
}

std::vector<Step> dfs(const Graph& g, int start) {
    std::vector<Step> steps;
    if (!validNode(g, start)) return steps;

    std::vector<bool> seen(g.nodeCount(), false);
    dfsVisit(g, start, seen, steps);
    return steps;
}

std::vector<Step> dijkstra(const Graph& g, int start) {
    std::vector<Step> steps;
    if (!validNode(g, start)) return steps;

    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dist(g.nodeCount(), INF);
    std::vector<int> parentEdge(g.nodeCount(), -1);  // edge used to reach each node
    std::vector<bool> done(g.nodeCount(), false);

    using Item = std::pair<int, int>;  // (distance, node)
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;  // min-heap

    dist[start] = 0;
    pq.push({0, start});
    steps.push_back({StepType::SetDistance, start, 0});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (done[u]) continue;  // outdated queue entry
        done[u] = true;

        steps.push_back({StepType::VisitNode, u});
        if (parentEdge[u] != -1) steps.push_back({StepType::AcceptEdge, parentEdge[u]});

        for (auto [v, e] : g.neighbors(u)) {
            if (done[v]) continue;
            steps.push_back({StepType::ConsiderEdge, e});
            int newDist = d + g.edges()[e].w;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                parentEdge[v] = e;
                pq.push({newDist, v});
                steps.push_back({StepType::SetDistance, v, newDist});
            }
        }
        steps.push_back({StepType::FinishNode, u});
    }
    return steps;
}

std::vector<Step> kruskal(const Graph& g) {
    std::vector<Step> steps;
    const auto& edges = g.edges();

    // Edge ids sorted by weight (stable: equal weights keep insertion order).
    std::vector<int> order(edges.size());
    std::iota(order.begin(), order.end(), 0);
    std::stable_sort(order.begin(), order.end(),
                     [&](int a, int b) { return edges[a].w < edges[b].w; });

    // Union-find: following parent[] from a node leads to its component's root.
    std::vector<int> parent(g.nodeCount());
    std::iota(parent.begin(), parent.end(), 0);
    auto find = [&](int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];  // path halving keeps the trees flat
            x = parent[x];
        }
        return x;
    };

    for (int e : order) {
        steps.push_back({StepType::ConsiderEdge, e});
        int rootU = find(edges[e].u);
        int rootV = find(edges[e].v);
        if (rootU == rootV) {  // already connected: this edge would make a cycle
            steps.push_back({StepType::RejectEdge, e});
            continue;
        }
        parent[rootU] = rootV;  // merge the two components
        steps.push_back({StepType::AcceptEdge, e});
        steps.push_back({StepType::FinishNode, edges[e].u});
        steps.push_back({StepType::FinishNode, edges[e].v});
    }
    return steps;
}

}  // namespace algo
