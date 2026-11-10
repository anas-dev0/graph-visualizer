#include "app/App.h"
#include <stdexcept>
#include <utility>
#include "core/Algorithms.h"
#include "core/GraphIO.h"

namespace {
const char* kSaveFile = "graph.txt";
const char* kSampleFile = "examples/sample.txt";
}  // namespace

App::App()
    : window_(sf::VideoMode(1000, 700), "Graph Algorithms Visualizer",
              sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(0, 0, 4)) {  // 4x antialiasing
    window_.setFramerateLimit(60);
    if (!font_.loadFromFile("assets/DejaVuSans.ttf"))
        throw std::runtime_error("Could not load assets/DejaVuSans.ttf. Run the app from the project folder.");
    if (auto sample = loadGraph(kSampleFile)) graph_ = *sample;  // start with a demo graph if present
}

void App::run() {
    sf::Clock clock;
    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) handleEvent(event);
        animator_.update(clock.restart().asSeconds());
        render();
    }
}

void App::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        window_.close();
        return;
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F12) {
        screenshotRequested_ = true;  // taken in render(), once the frame is drawn
        return;
    }
    if (animator_.active()) {  // editing is locked while an animation is shown
        if (event.type == sf::Event::KeyPressed) handleAnimationKey(event.key.code);
        return;
    }
    bool wasTyping = input_.typingWeight();  // while typing a weight, digit keys are not shortcuts
    input_.handle(event);
    if (event.type == sf::Event::KeyPressed && !wasTyping) handleEditKey(event.key.code);
}

void App::handleEditKey(sf::Keyboard::Key key) {
    if (input_.dragFrom() != -1) return;  // finish the drag first
    status_.clear();
    int start = input_.startNode();
    switch (key) {
    case sf::Keyboard::Num1: startAlgorithm("BFS", algo::bfs(graph_, start)); break;
    case sf::Keyboard::Num2: startAlgorithm("DFS", algo::dfs(graph_, start)); break;
    case sf::Keyboard::Num3: startAlgorithm("Dijkstra", algo::dijkstra(graph_, start)); break;
    case sf::Keyboard::Num4: startAlgorithm("Kruskal (MST)", algo::kruskal(graph_)); break;
    case sf::Keyboard::C:
        graph_.clear();
        input_.reset();
        break;
    case sf::Keyboard::S:
        status_ = saveGraph(graph_, kSaveFile) ? "Saved to graph.txt" : "Could not save graph.txt";
        break;
    case sf::Keyboard::L:
        if (auto loaded = loadGraph(kSaveFile)) {
            graph_ = *loaded;
            input_.reset();
            status_ = "Loaded graph.txt";
        } else {
            status_ = "Could not load graph.txt (missing or invalid)";
        }
        break;
    default: break;
    }
}

void App::handleAnimationKey(sf::Keyboard::Key key) {
    switch (key) {
    case sf::Keyboard::Space:    animator_.togglePlay(); break;
    case sf::Keyboard::Right:    animator_.pause(); animator_.stepForward(); break;
    case sf::Keyboard::Left:     animator_.pause(); animator_.stepBack(); break;
    case sf::Keyboard::Equal:    // the +/= key
    case sf::Keyboard::Add:      animator_.setDelay(animator_.delay() / 2.f); break;
    case sf::Keyboard::Hyphen:   // the -/_ key
    case sf::Keyboard::Subtract: animator_.setDelay(animator_.delay() * 2.f); break;
    case sf::Keyboard::Escape:   animator_.stop(); break;
    default: break;
    }
}

void App::startAlgorithm(const std::string& name, std::vector<Step> steps) {
    algoName_ = name;
    animator_.start(std::move(steps));  // empty steps (e.g. no nodes) leave the animator inactive
}

void App::render() {
    window_.clear(sf::Color(25, 28, 34));
    const auto& nodes = graph_.nodes();

    // Preview lines for the edge being created (drawn first, so nodes cover them).
    if (input_.dragFrom() != -1)
        renderer_.drawLine(window_, toVector(nodes[input_.dragFrom()]), input_.mouse(),
                           sf::Color(120, 170, 255), 2.f);
    if (input_.typingWeight()) {
        sf::Vector2f a = toVector(nodes[input_.pendingFrom()]);
        sf::Vector2f b = toVector(nodes[input_.pendingTo()]);
        renderer_.drawLine(window_, a, b, sf::Color(120, 170, 255), 2.f);
        renderer_.drawText(window_, input_.weightText() + "_", (a + b) / 2.f + sf::Vector2f(0.f, -16.f),
                           20, sf::Color(255, 220, 120), true);
    }

    if (animator_.active()) {
        VisualState state = animator_.state(nodes.size(), graph_.edges().size());
        renderer_.drawGraph(window_, graph_, &state, input_.startNode());
    } else {
        renderer_.drawGraph(window_, graph_, nullptr, input_.startNode());
    }

    renderer_.drawText(window_, hudText(), {12.f, 10.f}, 16, sf::Color(220, 220, 220));

    if (screenshotRequested_) {
        sf::Texture shot;
        shot.create(window_.getSize().x, window_.getSize().y);
        shot.update(window_);
        shot.copyToImage().saveToFile("screenshot_" + std::to_string(screenshotCount_++) + ".png");
        screenshotRequested_ = false;
    }
    window_.display();
}

std::string App::hudText() const {
    if (animator_.active())
        return algoName_ + "   step " + std::to_string(animator_.index()) + " / " +
               std::to_string(animator_.size()) + (animator_.playing() ? "   [playing]   " : "   [paused]   ") +
               std::to_string(static_cast<int>(animator_.delay() * 1000)) + " ms/step" +
               "\nSpace: play/pause    Left/Right: step    +/-: speed    Esc: back to editing";
    if (input_.typingWeight())
        return "Type the edge weight, then Enter to confirm or Esc to cancel";
    return "Click: add node    Drag node to node: add edge    Right-click: set start node\n"
           "1: BFS    2: DFS    3: Dijkstra    4: Kruskal    S/L: save/load    C: clear    F12: screenshot\n" +
           status_;
}
