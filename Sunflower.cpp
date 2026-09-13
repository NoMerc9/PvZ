#include "Sunflower.h"
#include "Constants.h"

Sunflower::Sunflower(sf::Vector2f pos)
    : Plant(pos, 100, COST_SUNFLOWER, 8.0f, "assets/sunflower.png")
{
}

bool Sunflower::tryProduceSun() {
    if (isReady()) {
        resetCooldown();
        return true;
    }
    return false;
}