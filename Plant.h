#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Plant {
protected:
    sf::Vector2f position;
    int hp;
    int cost;
    float cooldown;
    float cooldownMax;
    sf::Sprite sprite;

    sf::Clock animClock;
    float baseScale = 1.0f;

public:
    Plant(sf::Vector2f pos, int hp, int cost,
        float cooldownMax, const std::string& texturePath,
        bool centerVertically = true);
    virtual ~Plant() = default;

    virtual void update(float dt);
    virtual void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return position; }
    bool isAlive() const { return hp > 0; }
    void takeDamage(int dmg) { hp -= dmg; }

    bool isReady() const { return cooldown <= 0; }
    void resetCooldown() { cooldown = cooldownMax; }
    int getCost() const { return cost; }
};