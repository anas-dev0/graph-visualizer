#include "core/Algorithms.h"
#include <functional>
#include <limits>
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

}  // namespace algo
