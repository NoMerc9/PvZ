#include "Wallnut.h"
#include "Constants.h"

Wallnut::Wallnut(sf::Vector2f pos)
    : Plant(pos, 400, 25, 999.0f, "assets/wallnut.png")
{
    // cooldownMax = 999 — орех никогда не «стреляет»
    // hp = 400 — в 4 раза больше обычного растения
    // cost = 50 — как подсолнух
}