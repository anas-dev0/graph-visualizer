#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "core/Graph.h"

// Turns mouse and keyboard events into graph edits:
//   left click on empty space -> add node
//   left drag node to node    -> new edge, then type its weight (Enter = confirm, Esc = cancel)
//   right click on a node     -> choose the start node for algorithms
class InputHandler {
public:
    explicit InputHandler(Graph& graph) : graph_(graph) {}
    void handle(const sf::Event& event);
    void reset();  // forget all editing state (used after clearing the graph)

    int dragFrom() const { return dragFrom_; }  // node being dragged from, or -1
    sf::Vector2f mouse() const { return mouse_; }
    bool typingWeight() const { return pendingFrom_ != -1; }
    int pendingFrom() const { return pendingFrom_; }
    int pendingTo() const { return pendingTo_; }
    const std::string& weightText() const { return weightText_; }
    int startNode() const { return startNode_; }

private:
    void handleTyping(const sf::Event& event);

    Graph& graph_;
    int dragFrom_ = -1;
    int pendingFrom_ = -1;  // edge waiting for its weight
    int pendingTo_ = -1;
    std::string weightText_;
    sf::Vector2f mouse_;
    int startNode_ = 0;
};
