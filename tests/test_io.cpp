#include <cstdio>
#include <fstream>
#include "catch_amalgamated.hpp"
#include "core/GraphIO.h"

namespace {
const char* kTmp = "build/test_io_tmp.txt";

void writeFile(const std::string& text) { std::ofstream(kTmp) << text; }
}  // namespace

TEST_CASE("save then load gives the same graph") {
    Graph g;
    g.addNode(10, 20);
    g.addNode(30.5f, 40);
    g.addNode(50, 60);
    g.addEdge(0, 1, 7);
    g.addEdge(1, 2, 3);

    REQUIRE(saveGraph(g, kTmp));
    auto loaded = loadGraph(kTmp);
    REQUIRE(loaded.has_value());

    REQUIRE(loaded->nodeCount() == 3);
    CHECK(loaded->nodes()[1].x == 30.5f);
    CHECK(loaded->nodes()[2].y == 60);
    REQUIRE(loaded->edges().size() == 2);
    CHECK(loaded->edges()[0].u == 0);
    CHECK(loaded->edges()[0].v == 1);
    CHECK(loaded->edges()[0].w == 7);
    CHECK(loaded->edges()[1].w == 3);
    std::remove(kTmp);
}

TEST_CASE("empty file loads an empty graph; comments and blank lines are skipped") {
    writeFile("");
    REQUIRE(loadGraph(kTmp).has_value());
    CHECK(loadGraph(kTmp)->nodeCount() == 0);

    writeFile("# a comment\n\nn 1 2\n");
    REQUIRE(loadGraph(kTmp).has_value());
    CHECK(loadGraph(kTmp)->nodeCount() == 1);
    std::remove(kTmp);
}

TEST_CASE("malformed files are rejected") {
    writeFile("n 1 2\nhello\n");          // unknown line type
    CHECK_FALSE(loadGraph(kTmp).has_value());

    writeFile("n 1\n");                   // missing y
    CHECK_FALSE(loadGraph(kTmp).has_value());

    writeFile("n 0 0\nn 1 1\ne 0 5 3\n"); // edge to a node that doesn't exist
    CHECK_FALSE(loadGraph(kTmp).has_value());

    CHECK_FALSE(loadGraph("build/does_not_exist.txt").has_value());
    std::remove(kTmp);
}
