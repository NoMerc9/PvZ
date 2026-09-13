#include "Peashooter.h"
#include "Constants.h"

Peashooter::Peashooter(sf::Vector2f pos)
    : Plant(pos, 100, COST_PEASHOOTER, 1.5f, "assets/peashooter.png")
{
}

bool Peashooter::tryShoot() {
    if (isReady()) {
        resetCooldown();
        return true;
    }
    return false;
}