#pragma once
#include "Plant.h"

class Peashooter : public Plant {
public:
    explicit Peashooter(sf::Vector2f pos);
    bool tryShoot();
};