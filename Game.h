#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>

#include "Plant.h"
#include "Zombie.h"
#include "Projectile.h"
#include "Sun.h"

class Game {
    sf::RenderWindow window;

    // Fullscreen
    bool isFullscreen = false;
    sf::VideoMode windowedMode;
    sf::Vector2i windowedPos;

    // Лопата
    bool shovelSelected = false;
    sf::RectangleShape shovelButton;

    // Игровые объекты
    std::vector<std::unique_ptr<Plant>> plants;
    std::vector<std::unique_ptr<Zombie>> zombies;
    std::vector<Projectile> projectiles;
    std::vector<Sun> suns;

    int sun = 50;
    float zombieSpawnTimer;
    float sunSpawnTimer;
    int selectedPlant = 0;

    std::mt19937 gen;
    std::uniform_int_distribution<> rowDist;
    std::uniform_int_distribution<> zombieTypeDist;

    sf::Font font;
    sf::Text sunText;

    sf::Clock clock;

    void handleEvents();
    void update(float dt);
    void draw();
    void drawGrid();
    void drawHud();
    void spawnZombie();
    void spawnSun();
    void toggleFullscreen();
    void updateView();

public:
    Game();
    void run();
};