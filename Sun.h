#pragma once
#include <SFML/Graphics.hpp>

class Sun {
    sf::Vector2f position;
    float lifetime = 8.0f;
    bool collected = false;
    sf::Sprite sprite;

public:
    explicit Sun(sf::Vector2f pos);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool contains(sf::Vector2f mouse);
    bool isCollected() const { return collected; }
    void collect() { collected = true; }
};
