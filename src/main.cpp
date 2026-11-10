#include <SFML/Graphics.hpp>

// Temporary: proves SFML builds and links. Replaced by App on Day 2.
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph Algorithms Visualizer");
    sf::CircleShape circle(40.f);
    circle.setPosition(360.f, 260.f);
    circle.setFillColor(sf::Color(80, 160, 255));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed) window.close();

        window.clear(sf::Color(30, 30, 30));
        window.draw(circle);
        window.display();
    }
}
