#include "catch_amalgamated.hpp"
#include "core/Animator.h"

namespace {
// 2 nodes, 1 edge: visit 0, consider edge 0, accept it, visit 1.
const std::vector<Step> kSteps = {
    {StepType::VisitNode, 0},
    {StepType::ConsiderEdge, 0},
    {StepType::AcceptEdge, 0},
    {StepType::VisitNode, 1},
};
}  // namespace

TEST_CASE("replay with zero steps shows nothing") {
    VisualState s = replay(kSteps, 0, 2, 1);
    CHECK(s.nodes == std::vector<NodeMark>{NodeMark::None, NodeMark::None});
    CHECK(s.edges == std::vector<EdgeMark>{EdgeMark::None});
    CHECK(s.dist == std::vector<int>{-1, -1});
}

TEST_CASE("replay applies steps in order") {
    CHECK(replay(kSteps, 2, 2, 1).edges[0] == EdgeMark::Considered);
    CHECK(replay(kSteps, 3, 2, 1).edges[0] == EdgeMark::Accepted);

    VisualState s = replay(kSteps, 99, 2, 1);  // count past the end is clamped
    CHECK(s.nodes == std::vector<NodeMark>{NodeMark::Active, NodeMark::Active});
}

TEST_CASE("ConsiderEdge never downgrades an accepted edge") {
    std::vector<Step> steps = {{StepType::AcceptEdge, 0}, {StepType::ConsiderEdge, 0}};
    CHECK(replay(steps, 2, 0, 1).edges[0] == EdgeMark::Accepted);
}

TEST_CASE("Animator steps stay within bounds") {
    Animator a;
    CHECK_FALSE(a.active());

    a.start(kSteps);
    CHECK(a.active());
    CHECK(a.index() == 0);

    a.stepBack();
    CHECK(a.index() == 0);

    for (int i = 0; i < 10; ++i) a.stepForward();
    CHECK(a.index() == kSteps.size());

    a.stepBack();
    CHECK(a.index() == kSteps.size() - 1);

    a.stop();
    CHECK_FALSE(a.active());
}

TEST_CASE("Animator advances one step per delay while playing") {
    Animator a;
    a.start(kSteps);
    a.update(0.1f);
    CHECK(a.index() == 0);  // not enough time yet
    a.update(0.5f);
    CHECK(a.index() == 1);

    a.togglePlay();  // pause
    a.update(1.0f);
    CHECK(a.index() == 1);
}

TEST_CASE("animation speed is clamped") {
    Animator a;
    a.setDelay(100.f);
    CHECK(a.delay() == 2.f);
    a.setDelay(0.f);
    CHECK(a.delay() == 0.05f);
}

TEST_CASE("playing again after the end restarts from the beginning") {
    Animator a;
    a.start(kSteps);
    a.pause();
    for (int i = 0; i < 10; ++i) a.stepForward();
    a.togglePlay();
    CHECK(a.playing());
    CHECK(a.index() == 0);
}
