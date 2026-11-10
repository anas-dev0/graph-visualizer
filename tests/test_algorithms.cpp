#include <algorithm>
#include "catch_amalgamated.hpp"
#include "core/Algorithms.h"
#include "core/Animator.h"

namespace {

// Builds a graph from an edge list. Positions don't matter for algorithms.
Graph makeGraph(int nodeCount, const std::vector<Edge>& edges) {
    Graph g;
    for (int i = 0; i < nodeCount; ++i) g.addNode(0, 0);
    for (const Edge& e : edges) g.addEdge(e.u, e.v, e.w);
    return g;
}

// Nodes in the order the algorithm visited them.
std::vector<int> visitOrder(const std::vector<Step>& steps) {
    std::vector<int> order;
    for (const Step& s : steps)
        if (s.type == StepType::VisitNode) order.push_back(s.id);
    return order;
}

// The state after all steps have been applied.
VisualState finalState(const Graph& g, const std::vector<Step>& steps) {
    return replay(steps, steps.size(), g.nodeCount(), g.edges().size());
}

//      0
//     / \        node 5 is isolated
//    1---2
//    |   |
//    3   4
Graph traversalGraph() {
    return makeGraph(6, {{0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {2, 4, 1}, {1, 2, 1}});
}

// Shortest path 0->1 is 0-2-1 (cost 3), not the direct edge (cost 4). Node 4 is unreachable.
Graph dijkstraGraph() {
    return makeGraph(5, {{0, 1, 4}, {0, 2, 1}, {2, 1, 2}, {1, 3, 1}, {2, 3, 5}});
}

}  // namespace

TEST_CASE("BFS visits nodes level by level") {
    Graph g = traversalGraph();
    auto steps = algo::bfs(g, 0);
    CHECK(visitOrder(steps) == std::vector<int>{0, 1, 2, 3, 4});

    VisualState s = finalState(g, steps);
    CHECK(s.nodes[5] == NodeMark::None);  // unreachable, never visited
    CHECK(std::count(s.edges.begin(), s.edges.end(), EdgeMark::Accepted) == 4);  // spanning tree of 5 nodes
}

TEST_CASE("DFS goes deep before wide") {
    Graph g = traversalGraph();
    auto steps = algo::dfs(g, 0);
    CHECK(visitOrder(steps) == std::vector<int>{0, 1, 3, 2, 4});

    VisualState s = finalState(g, steps);
    CHECK(s.nodes[0] == NodeMark::Done);
    CHECK(s.nodes[5] == NodeMark::None);
    CHECK(std::count(s.edges.begin(), s.edges.end(), EdgeMark::Accepted) == 4);
}

TEST_CASE("traversal from an isolated node visits only that node") {
    Graph g = traversalGraph();
    CHECK(visitOrder(algo::bfs(g, 5)) == std::vector<int>{5});
    CHECK(visitOrder(algo::dfs(g, 5)) == std::vector<int>{5});
}

TEST_CASE("invalid start node gives no steps") {
    Graph g = traversalGraph();
    CHECK(algo::bfs(g, 99).empty());
    CHECK(algo::dfs(g, -1).empty());
    CHECK(algo::dijkstra(Graph{}, 0).empty());
}

TEST_CASE("Dijkstra finds shortest distances") {
    Graph g = dijkstraGraph();
    VisualState s = finalState(g, algo::dijkstra(g, 0));
    CHECK(s.dist == std::vector<int>{0, 3, 1, 4, -1});
}

TEST_CASE("Dijkstra accepts exactly the shortest-path tree edges") {
    Graph g = dijkstraGraph();
    VisualState s = finalState(g, algo::dijkstra(g, 0));
    // edge ids: 0=(0,1,4) 1=(0,2,1) 2=(2,1,2) 3=(1,3,1) 4=(2,3,5)
    CHECK(s.edges[1] == EdgeMark::Accepted);
    CHECK(s.edges[2] == EdgeMark::Accepted);
    CHECK(s.edges[3] == EdgeMark::Accepted);
    CHECK(s.edges[0] != EdgeMark::Accepted);
    CHECK(s.edges[4] != EdgeMark::Accepted);
}
