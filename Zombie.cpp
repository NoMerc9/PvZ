#include "Zombie.h"
#include "TextureManager.h"
#include "Constants.h"
#include <cmath>

Zombie::Zombie(sf::Vector2f pos, int hp, float speed,
    const std::string& texturePath)
    : position(pos), hp(hp), maxHp(hp), speed(speed),
    sprite(TextureManager::get(texturePath))
{
    sf::FloatRect bounds = sprite.getLocalBounds();

    // ✅ Origin — центр спрайта (как было раньше)
    sprite.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
    sprite.setPosition(pos);

    baseScale = ZOMBIE_SCALE;
    sprite.setScale({ baseScale, baseScale });
}

void Zombie::update(float dt) {
    float t = animClock.getElapsedTime().asSeconds();

    // Поедание
    if (targetPlant != nullptr) {
        biteTimer -= dt;
        float chew = 5.0f * std::sin(t * 12.0f);
        sprite.setRotation(sf::degrees(chew));
        return;
    }

    // Ходьба
    float sway = 4.0f * std::sin(t * 5.0f);
    sprite.setRotation(sf::degrees(sway));

    sprite.setPosition(position);   // ← без сдвига Y

    position.x -= speed * dt;
    if (hp <= 0) alive = false;
}

void Zombie::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Zombie::takeDamage(int dmg) {
    hp -= dmg;
    if (hp <= 0) alive = false;
}

bool Zombie::tryBite() {
    if (biteTimer <= 0) {
        biteTimer = ZOMBIE_BITE_COOLDOWN;
        return true;
    }
    return false;
}