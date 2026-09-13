#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
    sf::Vector2f position;
    float speed = 400.0f;
    int damage = 25;
    bool alive = true;
    sf::Sprite sprite;

public:
    explicit Projectile(sf::Vector2f pos);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    int getDamage() const { return damage; }
    bool isAlive() const { return alive; }
    void kill() { alive = false; }
};
