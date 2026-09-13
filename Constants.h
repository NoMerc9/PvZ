#pragma once
#include <SFML/Graphics.hpp>

// Окно
constexpr int WINDOW_W = 1000;
constexpr int WINDOW_H = 600;

// Газон
constexpr int COLS = 11;
constexpr int ROWS = 5;
constexpr int CELL_W = 90;
constexpr int CELL_H = 100;
constexpr int GRID_TOP = 60;

// Игровые константы
constexpr int START_SUN = 50;
constexpr float ZOMBIE_SPAWN_DELAY = 8.0f;
constexpr float SUN_SPAWN_DELAY = 7.0f;
constexpr int SUN_FROM_SKY = 25;

// Стоимость растений
constexpr int COST_PEASHOOTER = 50;
constexpr int COST_SUNFLOWER = 25;
constexpr int COST_WALLNUT = 25;
constexpr int COST_SPIKE = 50;

// Масштаб спрайтов
constexpr float PLANT_SCALE = 0.7f;
constexpr float ZOMBIE_SCALE = 0.6f;

// Шипы
constexpr int SPIKE_MAX_HITS = 15;
constexpr int SPIKE_DAMAGE_PER_HIT = 25;
constexpr float SPIKE_HIT_COOLDOWN = 0.5f;
constexpr float SPIKE_FLASH_DURATION = 0.15f;

// Зомби едят растения
constexpr int ZOMBIE_DAMAGE = 25;
constexpr float ZOMBIE_BITE_COOLDOWN = 1.0f;

// ==== Вспомогательные функции ====
inline int gridLeft(sf::RenderWindow& /*window*/) {
    return 0;
}
