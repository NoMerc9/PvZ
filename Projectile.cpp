#include "Projectile.h"
#include "TextureManager.h"

Projectile::Projectile(sf::Vector2f pos) : position(pos),
sprite(TextureManager::get("assets/pea.png"))
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
    sprite.setPosition(position);
    sprite.setScale({ 5.0f, 5.0f });
}

void Projectile::update(float dt) {
    position.x += speed * dt;
    sprite.setPosition(position);
    if (position.x > 1200) alive = false;
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}