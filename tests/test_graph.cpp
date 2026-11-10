#include "catch_amalgamated.hpp"
#include "core/Graph.h"

using Neighbors = std::vector<std::pair<int, int>>;

TEST_CASE("nodes get sequential ids") {
    Graph g;
    CHECK(g.addNode(0, 0) == 0);
    CHECK(g.addNode(10, 0) == 1);
    CHECK(g.nodeCount() == 2);
}

TEST_CASE("addEdge rejects invalid edges") {
    Graph g;
    g.addNode(0, 0);
    g.addNode(10, 0);
    g.addNode(20, 0);

    REQUIRE(g.addEdge(0, 1, 5) == 0);
    CHECK(g.addEdge(0, 1, 3) == -1);   // duplicate
    CHECK(g.addEdge(1, 0, 3) == -1);   // duplicate, reversed
    CHECK(g.addEdge(0, 0, 1) == -1);   // self-loop
    CHECK(g.addEdge(0, 7, 1) == -1);   // unknown node
    CHECK(g.addEdge(-1, 1, 1) == -1);  // negative id
    CHECK(g.addEdge(1, 2, 0) == -1);   // weight must be >= 1
    CHECK(g.edges().size() == 1);
}

TEST_CASE("edges are undirected") {
    Graph g;
    g.addNode(0, 0);
    g.addNode(10, 0);
    g.addEdge(0, 1, 5);

    CHECK(g.neighbors(0) == Neighbors{{1, 0}});
    CHECK(g.neighbors(1) == Neighbors{{0, 0}});
}

TEST_CASE("nodeAt finds a node within the radius") {
    Graph g;
    g.addNode(100, 100);
    CHECK(g.nodeAt(105, 100, 20) == 0);
    CHECK(g.nodeAt(200, 200, 20) == -1);
}

TEST_CASE("clear removes everything") {
    Graph g;
    g.addNode(0, 0);
    g.addNode(10, 0);
    g.addEdge(0, 1, 1);
    g.clear();
    CHECK(g.nodeCount() == 0);
    CHECK(g.edges().empty());
}
