#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Plant;

class Zombie {
protected:
    sf::Vector2f position;
    int hp;
    int maxHp;
    float speed;
    bool alive = true;
    sf::Sprite sprite;

    float biteTimer = 0.0f;
    Plant* targetPlant = nullptr;

    sf::Clock animClock;
    float baseScale = 1.0f;

public:
    Zombie(sf::Vector2f pos, int hp, float speed,
        const std::string& texturePath);
    virtual ~Zombie() = default;

    virtual void update(float dt);
    virtual void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return position; }
    bool isAlive() const { return alive; }
    virtual void takeDamage(int dmg);
    void kill() { alive = false; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }

    void setTargetPlant(Plant* p) { targetPlant = p; }
    Plant* getTargetPlant() const { return targetPlant; }
    bool tryBite();
};
