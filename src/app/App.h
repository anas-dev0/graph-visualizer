#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "app/InputHandler.h"
#include "app/Renderer.h"
#include "core/Animator.h"
#include "core/Graph.h"

// Owns everything and runs the main loop: events -> update -> draw.
class App {
public:
    App();  // throws std::runtime_error if the font can't be loaded
    void run();

private:
    void handleEvent(const sf::Event& event);
    void handleEditKey(sf::Keyboard::Key key);
    void handleAnimationKey(sf::Keyboard::Key key);
    void startAlgorithm(const std::string& name, std::vector<Step> steps);
    void render();
    std::string hudText() const;

    // Order matters: members are constructed top to bottom.
    sf::RenderWindow window_;
    sf::Font font_;
    Graph graph_;
    InputHandler input_{graph_};
    Renderer renderer_{font_};
    Animator animator_;
    std::string algoName_;
    std::string status_;                // e.g. "Saved to graph.txt", shown in the help text
    bool screenshotRequested_ = false;  // F12
    int screenshotCount_ = 0;
};
