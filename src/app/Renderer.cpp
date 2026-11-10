#include "app/Renderer.h"
#include <cmath>

namespace {

sf::Color nodeColor(NodeMark mark) {
    switch (mark) {
    case NodeMark::Active: return sf::Color(255, 165, 0);  // orange: discovered
    case NodeMark::Done:   return sf::Color(60, 180, 90);  // green: finished
    default:               return sf::Color(70, 85, 105);  // untouched
    }
}

struct EdgeStyle { sf::Color color; float thickness; };

EdgeStyle edgeStyle(EdgeMark mark) {
    switch (mark) {
    case EdgeMark::Considered: return {sf::Color(240, 210, 60), 3.f};  // yellow: looked at
    case EdgeMark::Accepted:   return {sf::Color(60, 200, 90), 6.f};   // green: part of the result
    case EdgeMark::Rejected:   return {sf::Color(80, 80, 80), 2.f};    // dark: rejected
    default:                   return {sf::Color(150, 150, 150), 2.f};
    }
}

}  // namespace

void Renderer::drawGraph(sf::RenderTarget& target, const Graph& graph,
                         const VisualState* state, int startNode) const {
    const auto& nodes = graph.nodes();
    const auto& edges = graph.edges();

    // Edges first, so nodes are drawn on top of them.
    for (size_t i = 0; i < edges.size(); ++i) {
        EdgeStyle style = edgeStyle(state ? state->edges[i] : EdgeMark::None);
        sf::Vector2f a = toVector(nodes[edges[i].u]);
        sf::Vector2f b = toVector(nodes[edges[i].v]);
        drawLine(target, a, b, style.color, style.thickness);
        drawText(target, std::to_string(edges[i].w), (a + b) / 2.f + sf::Vector2f(0.f, -14.f),
                 16, sf::Color::White, true);
    }

    for (size_t i = 0; i < nodes.size(); ++i) {
        sf::Vector2f pos = toVector(nodes[i]);
        bool isStart = static_cast<int>(i) == startNode;

        sf::CircleShape circle(kNodeRadius);
        circle.setOrigin(kNodeRadius, kNodeRadius);
        circle.setPosition(pos);
        circle.setFillColor(nodeColor(state ? state->nodes[i] : NodeMark::None));
        circle.setOutlineThickness(isStart ? 4.f : 2.f);
        circle.setOutlineColor(isStart ? sf::Color(80, 160, 255) : sf::Color(200, 200, 200));
        target.draw(circle);

        drawText(target, std::to_string(i), pos, 16, sf::Color::White, true);
        if (state && state->dist[i] != -1)
            drawText(target, "d=" + std::to_string(state->dist[i]),
                     pos + sf::Vector2f(0.f, -kNodeRadius - 14.f), 15, sf::Color(255, 220, 120), true);
    }
}

// SFML lines are always 1px, so a thick line is a thin rotated rectangle.
void Renderer::drawLine(sf::RenderTarget& target, sf::Vector2f a, sf::Vector2f b,
                        sf::Color color, float thickness) const {
    sf::Vector2f d = b - a;
    sf::RectangleShape line({std::sqrt(d.x * d.x + d.y * d.y), thickness});
    line.setOrigin(0.f, thickness / 2.f);
    line.setPosition(a);
    line.setRotation(std::atan2(d.y, d.x) * 180.f / 3.14159265f);
    line.setFillColor(color);
    target.draw(line);
}

void Renderer::drawText(sf::RenderTarget& target, const std::string& text, sf::Vector2f pos,
                        unsigned size, sf::Color color, bool centered) const {
    sf::Text label(text, font_, size);
    label.setFillColor(color);
    if (centered) {
        sf::FloatRect b = label.getLocalBounds();
        label.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    }
    label.setPosition(pos);
    target.draw(label);
}
