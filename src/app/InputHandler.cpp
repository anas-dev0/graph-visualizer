#include "app/InputHandler.h"
#include <algorithm>
#include "app/Renderer.h"  // kNodeRadius

void InputHandler::handle(const sf::Event& event) {
    if (typingWeight()) {
        handleTyping(event);
        return;
    }

    if (event.type == sf::Event::MouseMoved) {
        mouse_ = {static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y)};
        return;
    }
    if (event.type != sf::Event::MouseButtonPressed && event.type != sf::Event::MouseButtonReleased)
        return;

    float x = static_cast<float>(event.mouseButton.x);
    float y = static_cast<float>(event.mouseButton.y);
    int node = graph_.nodeAt(x, y, kNodeRadius);
    bool left = event.mouseButton.button == sf::Mouse::Left;

    if (event.type == sf::Event::MouseButtonPressed) {
        if (!left && node != -1) startNode_ = node;                 // right click
        else if (left && node != -1) dragFrom_ = node;              // start dragging an edge
        else if (left && graph_.nodeAt(x, y, 2 * kNodeRadius) == -1)
            graph_.addNode(x, y);                                    // empty space, not overlapping
    } else if (left && dragFrom_ != -1) {                            // released after a drag
        if (node != -1 && node != dragFrom_) {
            pendingFrom_ = dragFrom_;
            pendingTo_ = node;
            weightText_.clear();
        }
        dragFrom_ = -1;
    }
}

void InputHandler::handleTyping(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        sf::Uint32 c = event.text.unicode;
        if (c >= '0' && c <= '9' && weightText_.size() < 4) weightText_ += static_cast<char>(c);
        else if (c == '\b' && !weightText_.empty()) weightText_.pop_back();
    } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            int weight = weightText_.empty() ? 1 : std::max(1, std::stoi(weightText_));
            graph_.addEdge(pendingFrom_, pendingTo_, weight);  // ignored if the edge already exists
            pendingFrom_ = pendingTo_ = -1;
        } else if (event.key.code == sf::Keyboard::Escape) {
            pendingFrom_ = pendingTo_ = -1;
        }
    }
}

void InputHandler::reset() {
    dragFrom_ = pendingFrom_ = pendingTo_ = -1;
    weightText_.clear();
    startNode_ = 0;
}
