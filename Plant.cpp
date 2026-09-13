#include "Plant.h"
#include "TextureManager.h"
#include "Constants.h"
#include <cmath>

Plant::Plant(sf::Vector2f pos, int hp, int cost,
    float cooldownMax, const std::string& texturePath,
    bool centerVertically)
    : position(pos), hp(hp), cost(cost),
    cooldown(cooldownMax), cooldownMax(cooldownMax),
    sprite(TextureManager::get(texturePath))
{
    sf::FloatRect bounds = sprite.getLocalBounds();

    if (centerVertically) {
        sprite.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
        sprite.setPosition(position);
    }
    else {
        sprite.setOrigin({ bounds.size.x / 2.0f, bounds.size.y });
        sprite.setPosition({ position.x, position.y + CELL_H / 2.0f - 5.0f });
    }

    baseScale = PLANT_SCALE;
    sprite.setScale({ baseScale, baseScale });
}

void Plant::update(float dt) {
    if (cooldown > 0) cooldown -= dt;

    // ==== ПУЛЬСАЦИЯ ====
    float t = animClock.getElapsedTime().asSeconds();
    float breathe = 1.0f + 0.04f * std::sin(t * 4.0f);   // ±4%
    sprite.setScale({ baseScale * breathe, baseScale * breathe });
}

void Plant::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}