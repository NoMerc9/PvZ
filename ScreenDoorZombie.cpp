#include "ScreenDoorZombie.h"
#include "TextureManager.h"
#include "Constants.h"
#include <iostream>

ScreenDoorZombie::ScreenDoorZombie(sf::Vector2f pos)
    : Zombie(pos, 100, 10.0f, "assets/zombie.png"),
    doorSprite(TextureManager::get("assets/screen_door.png"))
{
    // Дверь рисуется перед зомби
    sf::FloatRect b = doorSprite.getLocalBounds();
    doorSprite.setOrigin({ b.size.x / 2.0f, b.size.y });
    doorSprite.setScale({ ZOMBIE_SCALE, ZOMBIE_SCALE });
}

void ScreenDoorZombie::update(float dt) {
    Zombie::update(dt);
    // Дверь двигается вместе с зомби
    doorSprite.setPosition({ position.x, position.y + CELL_H / 2.0f });
}

void ScreenDoorZombie::draw(sf::RenderWindow& window) {
    // Сначала зомби, потом дверь сверху
    Zombie::draw(window);
    if (hasDoor()) {
        window.draw(doorSprite);
    }
}

void ScreenDoorZombie::takeDamage(int dmg) {
    if (doorHp > 0) {
        doorHp -= dmg;
        if (doorHp <= 0) {
            std::cout << "Screen door broke!\n";
        }
        return;   // урон НЕ идёт в зомби, пока дверь цела
    }
    // Двери нет — урон идёт в зомби
    Zombie::takeDamage(dmg);
}