#include "Sun.h"
#include "TextureManager.h"

Sun::Sun(sf::Vector2f pos) : position(pos),
sprite(TextureManager::get("assets/sun.png"))
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
    sprite.setPosition(position);
    sprite.setScale({ 1.1f, 1.1f });
}

void Sun::update(float dt) {
    lifetime -= dt;
    if (lifetime <= 0) collected = true;
}

void Sun::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Sun::contains(sf::Vector2f mouse) {
    return sprite.getGlobalBounds().contains(mouse);
}