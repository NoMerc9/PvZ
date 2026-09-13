#pragma once
#include "Plant.h"

class Sunflower : public Plant {
public:
    explicit Sunflower(sf::Vector2f pos);
    bool tryProduceSun();
};
