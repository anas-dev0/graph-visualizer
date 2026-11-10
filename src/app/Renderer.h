#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "core/Animator.h"
#include "core/Graph.h"

constexpr float kNodeRadius = 20.f;

inline sf::Vector2f toVector(const Node& n) { return {n.x, n.y}; }

// Draws the graph and text. Never changes any state.
class Renderer {
public:
    explicit Renderer(const sf::Font& font) : font_(font) {}

    // state == nullptr means edit mode (no algorithm colors).
    void drawGraph(sf::RenderTarget& target, const Graph& graph,
                   const VisualState* state, int startNode) const;
    void drawLine(sf::RenderTarget& target, sf::Vector2f a, sf::Vector2f b,
                  sf::Color color, float thickness) const;
    void drawText(sf::RenderTarget& target, const std::string& text, sf::Vector2f pos,
                  unsigned size, sf::Color color, bool centered = false) const;

private:
    const sf::Font& font_;
};
