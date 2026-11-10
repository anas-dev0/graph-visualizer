#include "core/Animator.h"
#include <algorithm>
#include <utility>

VisualState replay(const std::vector<Step>& steps, std::size_t count,
                   std::size_t nodeCount, std::size_t edgeCount) {
    VisualState s{std::vector<NodeMark>(nodeCount, NodeMark::None),
                  std::vector<EdgeMark>(edgeCount, EdgeMark::None),
                  std::vector<int>(nodeCount, -1)};

    count = std::min(count, steps.size());
    for (std::size_t i = 0; i < count; ++i) {
        const Step& step = steps[i];
        switch (step.type) {
        case StepType::VisitNode:  s.nodes[step.id] = NodeMark::Active; break;
        case StepType::FinishNode: s.nodes[step.id] = NodeMark::Done; break;
        case StepType::ConsiderEdge:  // never downgrade an accepted/rejected edge
            if (s.edges[step.id] == EdgeMark::None) s.edges[step.id] = EdgeMark::Considered;
            break;
        case StepType::AcceptEdge:  s.edges[step.id] = EdgeMark::Accepted; break;
        case StepType::RejectEdge:  s.edges[step.id] = EdgeMark::Rejected; break;
        case StepType::SetDistance: s.dist[step.id] = step.value; break;
        }
    }
    return s;
}

void Animator::start(std::vector<Step> steps) {
    steps_ = std::move(steps);
    index_ = 0;
    timer_ = 0.f;
    playing_ = true;
}

void Animator::stop() {
    steps_.clear();
    index_ = 0;
    playing_ = false;
}

void Animator::update(float dt) {
    if (!playing_) return;
    timer_ += dt;
    if (timer_ < delay_) return;
    timer_ = 0.f;
    stepForward();
    if (index_ == steps_.size()) playing_ = false;  // reached the end
}

void Animator::togglePlay() {
    if (!playing_ && index_ == steps_.size()) index_ = 0;
    playing_ = !playing_;
}

void Animator::stepForward() {
    if (index_ < steps_.size()) ++index_;
}

void Animator::stepBack() {
    if (index_ > 0) --index_;
}
