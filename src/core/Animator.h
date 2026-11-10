#pragma once
#include <cstddef>
#include <vector>
#include "core/Step.h"

enum class NodeMark { None, Active, Done };
enum class EdgeMark { None, Considered, Accepted, Rejected };

// What the screen should show after a number of steps.
struct VisualState {
    std::vector<NodeMark> nodes;
    std::vector<EdgeMark> edges;
    std::vector<int> dist;  // -1 means "not reached yet"
};

// Rebuilds the state from scratch by applying the first `count` steps.
VisualState replay(const std::vector<Step>& steps, std::size_t count,
                   std::size_t nodeCount, std::size_t edgeCount);

// Plays a list of steps over time: play/pause and step forward/back.
class Animator {
public:
    void start(std::vector<Step> steps);
    void stop();
    void update(float dt);  // dt = seconds since the last frame
    void togglePlay();  // at the end, playing again restarts from step 0
    void pause() { playing_ = false; }
    void setDelay(float seconds);  // clamped to [0.05, 2] seconds per step
    float delay() const { return delay_; }
    void stepForward();
    void stepBack();

    bool active() const { return !steps_.empty(); }
    bool playing() const { return playing_; }
    std::size_t index() const { return index_; }
    std::size_t size() const { return steps_.size(); }

    VisualState state(std::size_t nodeCount, std::size_t edgeCount) const {
        return replay(steps_, index_, nodeCount, edgeCount);
    }

private:
    std::vector<Step> steps_;
    std::size_t index_ = 0;  // number of steps applied so far
    bool playing_ = false;
    float timer_ = 0.f;
    float delay_ = 0.5f;     // seconds per step
};
