#pragma once

// One event produced by an algorithm. The GUI replays these to animate it.
enum class StepType { VisitNode, FinishNode, ConsiderEdge, AcceptEdge, RejectEdge, SetDistance };

struct Step {
    StepType type;
    int id;         // node id or edge id, depending on the type
    int value = 0;  // new distance (SetDistance only)
};
