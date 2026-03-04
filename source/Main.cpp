/*
// Mangrove Project
// Main.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "SFML/Graphics.hpp"

auto main() -> int {
    sf::RenderWindow window;
    const sf::Texture texture("./resources/bomb.png");
    sf::Sprite sprite(texture);
    sprite.setPosition(sf::Vector2f{750, 250});
    sprite.setScale(sf::Vector2f{0.5, 0.5});
    sf::Sprite sprite2(texture);
    window.create(sf::VideoMode({1920, 1080}), "SFML");
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::Black);
        window.draw(sprite2);
        window.draw(sprite);
        window.display();
    }
    return 0;
}
