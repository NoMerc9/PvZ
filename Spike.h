#pragma once
#include "Plant.h"
#include "Constants.h"

class Spike : public Plant {
    int hitsLeft = SPIKE_MAX_HITS;   // сколько ударов осталось
    float hitCooldown = 0.0f;        // пауза до следующего удара
    float flashTimer = 0.0f;         // анимация вспышки

public:
    explicit Spike(sf::Vector2f pos);

    // Попытка ударить зомби. Возвращает урон (0 — не время / сломаны)
    int tryHit();

    bool isBroken() const { return hitsLeft <= 0; }

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};