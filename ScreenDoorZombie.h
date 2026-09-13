#pragma once
#include "Zombie.h"

class ScreenDoorZombie : public Zombie {
    int doorHp = 200;          // прочность двери
    sf::Sprite doorSprite;     // отдельный спрайт двери (спереди)

public:
    explicit ScreenDoorZombie(sf::Vector2f pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void takeDamage(int dmg) override;   // урон идёт в дверь, потом в зомби
    bool hasDoor() const { return doorHp > 0; }
};
