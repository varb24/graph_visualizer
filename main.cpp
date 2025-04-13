#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

struct Node {
    std::string name;
    sf::CircleShape nCircle;
    std::vector<Node*> adj;
    
    // Constructor for Node
    Node(const std::string& n, const sf::CircleShape& circle) 
        : name(n), nCircle(circle) {}
};

std::vector<Node> testerGraph(int num_circles, float radius) {
    std::vector<Node> circles;

    for (int i = 0; i < num_circles; ++i) {
        sf::CircleShape circle(radius);
        circle.setFillColor(sf::Color(100 + i*30, 150, 200)); // Fixed color calculation
        circle.setOrigin({radius, radius});
        
        // Create Node with name and circle
        Node circleNode("Node " + std::to_string(i), circle);
        circles.push_back(circleNode);
    
    }

    // Add a random circle connection
    circles[0].adj.push_back(&circles[5]);
    circles[0].adj.push_back(&circles[3]);

    // Arrange circles in a 5-column grid
    int columns = 5;
    for (int i = 0; i < circles.size(); ++i) {
        int row = i / columns;
        int col = i % columns;
        
        circles[i].nCircle.setPosition({col * 100.f + 50.f, row * 100.f + 50.f});
    }
    return circles;
}

std::vector<Node> process_graph_from_txt():
    /*
    Requires graph in format, where Wm identifies a weight, and X, and Y identifies verticies with a bi-directional edge. N is on the first line, it is the number of edges.
    Nodes will be referred to by a numerical index.
    X, Y < N
    
    N
    X0 Y0 W0
    X1 Y1 W1
    ...
    
    */
    std::vector<Node> graph;
    std::cin >> n;  // Reads the first number (e.g., 7)
    
    // Fill the graph with empty placeholder nodes, this will make it easier to fill up the graph.
    for (int i = 0; i < n; i++) {
        graph.push_back(Node("", sf::CircleShape))
    }    
    
    // For each subsequent line, read three integers.
    //TODO how am i going to organize the graph creation?
    //Problem : my current data structure has no way to store edge weights.
    for (int i = 0; i < n; i++) {
        int x, y, w;
        std::cin >> x >> y >> w;    
        // Both x and y must have their values modified. add y to x's adj list, and x to y's list.
        Node vertex = Node("", sf::Circle)
        std::cout << "Parsed: " << a << " " << b << " " << c << "\n";
    }

int main() {
    std::ostringstream buffer;
    buffer << std::cin.rdbuf();
    std::string input = buffer.str();

    std::cout << "Full input:\n" << input;

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Connected Circles");
    


    auto circles = testerGraph(30, 15.0); // Use auto or std::vector<Node>
    constexpr float radius = 30.f;
    
    std::optional<size_t> draggedCircle;
    sf::Vector2f dragOffset;
    sf::VertexArray lines(sf::PrimitiveType::Lines);

    while (window.isOpen()) {
        // Event handling
        for (auto event = window.pollEvent(); event; event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (auto mouseDown = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseDown->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos(mouseDown->position);
                    for (size_t i = 0; i < circles.size(); ++i) {
                        sf::Vector2f circleCenter = circles[i].nCircle.getPosition();
                        
                        if (std::hypot(mousePos.x - circleCenter.x, 
                                      mousePos.y - circleCenter.y) < radius) {
                            draggedCircle = i;
                            dragOffset = circleCenter - mousePos;
                            break;
                        }
                    }
                }
            }
            else if (auto mouseUp = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseUp->button == sf::Mouse::Button::Left) {
                    draggedCircle.reset();
                }
            }
            else if (auto mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                if (draggedCircle) {
                    circles[*draggedCircle].nCircle.setPosition(
                        sf::Vector2f(mouseMove->position) + dragOffset);
                }
            }
        }

        // Update lines
        lines.clear();
        for (size_t i = 0; i < circles.size(); ++i) {
            size_t next = (i + 1) % circles.size();
            //lines.append({circles[i].nCircle.getPosition(), sf::Color::Black});
            //lines.append({circles[next].nCircle.getPosition(), sf::Color::Black});
            // Connect from node i to all adj nodes
            for (const auto& connectedNode : circles[i].adj){
                lines.append({circles[i].nCircle.getPosition(), sf::Color::Black});
                lines.append({connectedNode->nCircle.getPosition(), sf::Color::Black});
            }
        }

        // Drawing
        window.clear(sf::Color::White);
        window.draw(lines);
        
        for (size_t i = 0; i < circles.size(); ++i) {
            if (draggedCircle && *draggedCircle == i) {
                circles[i].nCircle.setOutlineThickness(3.f);
                circles[i].nCircle.setOutlineColor(sf::Color::Red);
            } else {
                circles[i].nCircle.setOutlineThickness(0.f);
            }
            window.draw(circles[i].nCircle);
        }
        
        window.display();
    }

    return 0;
}