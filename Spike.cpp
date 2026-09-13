#include "Spike.h"
#include "Constants.h"
#include "TextureManager.h"
#include <iostream>

Spike::Spike(sf::Vector2f pos)
    : Plant(pos, 100, COST_SPIKE, 999.0f, "assets/spike.png", false)
{
}

int Spike::tryHit() {
    if (isBroken()) return 0;
    if (hitCooldown > 0) return 0;

    hitsLeft--;
    hitCooldown = SPIKE_HIT_COOLDOWN;
    flashTimer = SPIKE_FLASH_DURATION;

    return SPIKE_DAMAGE_PER_HIT;
}

void Spike::update(float dt) {
    Plant::update(dt);
    if (hitCooldown > 0) hitCooldown -= dt;
    if (flashTimer > 0)  flashTimer -= dt;
}

void Spike::draw(sf::RenderWindow& window) {
    float progress = flashTimer / SPIKE_FLASH_DURATION;

    if (flashTimer > 0) {
        sprite.setColor(sf::Color(255, 255, 255));
        float squash = 1.0f - 0.15f * progress;
        sprite.setScale({
            PLANT_SCALE * (1.0f + 0.1f * progress),
            PLANT_SCALE * squash
            });
    }
    else {
        sprite.setColor(sf::Color::White);
        sprite.setScale({ PLANT_SCALE, PLANT_SCALE });
    }

    window.draw(sprite);
}