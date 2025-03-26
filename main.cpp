#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <cmath>

int main()
{
    int num_circles = 10;

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Connected Circles");
    
    std::vector<sf::CircleShape> circles;
    std::optional<size_t> draggedCircle;
    sf::Vector2f dragOffset;
    
    // Create initial circles
    constexpr float radius = 30.f;
    for (int i = 0; i < num_circles; ++i) {
        sf::CircleShape circle(radius);
        circle.setFillColor(sf::Color(100 + i*30, 150*i, i*200));
        circle.setOrigin({radius, radius}); // Center the origin
        circles.push_back(circle);
    }

    // Arrange circles in a 5-column grid
    int columns = 5;
    for (int i = 0; i < circles.size(); ++i) {
        int row = i / columns;  // Integer division
        int col = i % columns;  // Modulo gives column index
        
        circles[i].setPosition({col * 100.f + 50.f, row * 100.f + 50.f});
    }
    // Vertex array for drawing lines - SFML 3.0 uses LineStrip instead of LinesStrip
    sf::VertexArray lines(sf::PrimitiveType::LineStrip);

    while (window.isOpen())
    {
        // Handle events
        for (auto event = window.pollEvent(); event; event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (auto mouseDown = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseDown->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos(mouseDown->position);
                    for (size_t i = 0; i < circles.size(); ++i) {
                        sf::Vector2f circleCenter = circles[i].getPosition();
                        
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
                    circles[*draggedCircle].setPosition(
                        sf::Vector2f(mouseMove->position) + dragOffset);
                }
            }
        }

        // Update the connecting lines
        lines.clear();
        for (size_t i = 0; i < circles.size(); ++i) {
            // Add line to next circle (creating a loop)
            size_t next = (i + 1) % circles.size();
            
            // Get center positions
            sf::Vector2f pos1 = circles[i].getPosition();
            sf::Vector2f pos2 = circles[next].getPosition();
            
            // SFML 3.0 vertex creation syntax
            lines.append({pos1, sf::Color::Black});
            lines.append({pos2, sf::Color::Black});
        }

        // Drawing
        window.clear(sf::Color::White);
        
        // Draw the connecting lines first (so they appear behind circles)
        window.draw(lines);
        
        // Draw the circles
        for (size_t i = 0; i < circles.size(); ++i) {
            if (draggedCircle && *draggedCircle == i) {
                circles[i].setOutlineThickness(3.f);
                circles[i].setOutlineColor(sf::Color::Red);
            } else {
                circles[i].setOutlineThickness(0.f);
            }
            window.draw(circles[i]);
        }
        
        window.display();
    }

    return 0;
}