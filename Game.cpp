#include "Game.h"
#include "Constants.h"
#include "Peashooter.h"
#include "Sunflower.h"
#include "Wallnut.h"
#include "Spike.h"
#include "NormalZombie.h"
#include "ConeZombie.h"
#include "BucketZombie.h"
#include "ScreenDoorZombie.h"
#include <iostream>
#include <algorithm>
#include <cmath>

// ==== Вспомогательные функции ====
static bool getCell(int mx, int my, int gridX, int& col, int& row) {
    col = (mx - gridX) / CELL_W;
    row = (my - GRID_TOP) / CELL_H;
    return col >= 0 && col < COLS && row >= 0 && row < ROWS;
}

static sf::Vector2f cellCenter(int col, int row, int gridX) {
    return sf::Vector2f(
        static_cast<float>(gridX + col * CELL_W + CELL_W / 2),
        static_cast<float>(GRID_TOP + row * CELL_H + CELL_H / 2)
    );
}

// ===== Конструктор =====
Game::Game()
    : window(sf::VideoMode({ WINDOW_W, WINDOW_H }), "Plants vs Zombies"),
    zombieSpawnTimer(4.0f),
    sunSpawnTimer(5.0f),
    gen(std::random_device{}()),
    rowDist(0, ROWS - 1),
    zombieTypeDist(0, 9),
    sunText(font, "", 24)
{
    window.setFramerateLimit(60);

    windowedMode = sf::VideoMode({ WINDOW_W, WINDOW_H });
    windowedPos = window.getPosition();

    if (!font.openFromFile("assets/font.ttf")) {
        std::cerr << "Failed to load font\n";
        std::exit(1);
    }

    sunText.setFont(font);
    sunText.setCharacterSize(28);
    sunText.setFillColor(sf::Color::White);
    sunText.setPosition({ 20, 15 });

    // Кнопка лопаты
    shovelButton.setSize({ 50, 40 });
    shovelButton.setFillColor(sf::Color(150, 100, 50));
    shovelButton.setOutlineThickness(2);
    shovelButton.setOutlineColor(sf::Color::White);
}

// ===== Главный цикл =====
void Game::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleEvents();
        update(dt);
        draw();
    }
}

// ===== События =====
void Game::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            updateView();
        }

        if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse->button == sf::Mouse::Button::Left) {
                sf::Vector2f clickPos = window.mapPixelToCoords(mouse->position);

                // ==== Клик по лопате ====
                float shovelX = static_cast<float>(WINDOW_W) - 70;
                if (clickPos.x >= shovelX && clickPos.x <= shovelX + 50 &&
                    clickPos.y >= 5 && clickPos.y <= 45)
                {
                    shovelSelected = !shovelSelected;
                    if (shovelSelected) selectedPlant = -1;
                    continue;
                }

                // ==== Клик по солнцу ====
                bool collected = false;
                for (auto& s : suns) {
                    if (!s.isCollected() && s.contains(clickPos)) {
                        s.collect();
                        sun += SUN_FROM_SKY;
                        collected = true;
                        break;
                    }
                }
                if (collected) continue;

                // ==== Клик по клетке ====
                int gridX = gridLeft(window);
                int col, row;
                if (getCell(static_cast<int>(clickPos.x),
                    static_cast<int>(clickPos.y),
                    gridX, col, row))
                {
                    sf::Vector2f pos = cellCenter(col, row, gridX);

                    // ==== Лопата ====
                    if (shovelSelected) {
                        for (auto it = plants.begin(); it != plants.end(); ++it) {
                            sf::Vector2f pPos = (*it)->getPosition();
                            if (std::abs(pPos.x - pos.x) < 5 &&
                                std::abs(pPos.y - pos.y) < 5)
                            {
                                it = plants.erase(it);
                                shovelSelected = false;
                                break;
                            }
                        }
                        continue;
                    }

                    // ==== Посадка ====
                    bool occupied = false;
                    for (auto& p : plants) {
                        sf::Vector2f pPos = p->getPosition();
                        if (std::abs(pPos.x - pos.x) < 5 &&
                            std::abs(pPos.y - pos.y) < 5) {
                            occupied = true;
                            break;
                        }
                    }

                    if (!occupied) {
                        if (selectedPlant == 0 && sun >= COST_PEASHOOTER) {
                            plants.push_back(std::make_unique<Peashooter>(pos));
                            sun -= COST_PEASHOOTER;
                        }
                        else if (selectedPlant == 1 && sun >= COST_SUNFLOWER) {
                            plants.push_back(std::make_unique<Sunflower>(pos));
                            sun -= COST_SUNFLOWER;
                        }
                        else if (selectedPlant == 2 && sun >= COST_WALLNUT) {
                            plants.push_back(std::make_unique<Wallnut>(pos));
                            sun -= COST_WALLNUT;
                        }
                        else if (selectedPlant == 3 && sun >= COST_SPIKE) {
                            plants.push_back(std::make_unique<Spike>(pos));
                            sun -= COST_SPIKE;
                        }
                    }
                }
            }
        }

        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Num1) selectedPlant = 0;
            if (key->code == sf::Keyboard::Key::Num2) selectedPlant = 1;
            if (key->code == sf::Keyboard::Key::Num3) selectedPlant = 2;
            if (key->code == sf::Keyboard::Key::Num4) selectedPlant = 3;
            if (key->code == sf::Keyboard::Key::Space) {
                shovelSelected = !shovelSelected;
                if (shovelSelected) selectedPlant = -1;
            }
            if (key->code == sf::Keyboard::Key::F11) {
                toggleFullscreen();
            }
        }
    }
}

// ===== Обновление =====
void Game::update(float dt) {
    // ==== НЕПРЕРЫВНЫЙ СПАВН ЗОМБИ ====
    zombieSpawnTimer -= dt;
    if (zombieSpawnTimer <= 0) {
        spawnZombie();
        zombieSpawnTimer = ZOMBIE_SPAWN_DELAY;
    }

    // ==== СОЛНЦЕ ====
    sunSpawnTimer -= dt;
    if (sunSpawnTimer <= 0) {
        spawnSun();
        sunSpawnTimer = SUN_SPAWN_DELAY;
    }

    // ==== РАСТЕНИЯ ====
    for (auto& plant : plants) {
        plant->update(dt);

        if (auto* pea = dynamic_cast<Peashooter*>(plant.get())) {
            bool hasZombie = false;
            for (auto& z : zombies) {
                if (!z->isAlive()) continue;
                if (std::abs(z->getPosition().y - pea->getPosition().y) < CELL_H / 2) {
                    hasZombie = true;
                    break;
                }
            }
            if (hasZombie && pea->tryShoot()) {
                projectiles.emplace_back(
                    sf::Vector2f(pea->getPosition().x + 30, pea->getPosition().y)
                );
            }
        }

        if (auto* sf = dynamic_cast<Sunflower*>(plant.get())) {
            if (sf->tryProduceSun()) {
                suns.emplace_back(sf->getPosition());
            }
        }
    }

    // ==== ЗОМБИ ====
    for (auto& z : zombies) {
        if (z->getTargetPlant() != nullptr && !z->getTargetPlant()->isAlive()) {
            z->setTargetPlant(nullptr);
        }

        if (z->getTargetPlant() == nullptr) {
            for (auto& plant : plants) {
                if (!plant->isAlive()) continue;
                if (dynamic_cast<Spike*>(plant.get()) != nullptr) continue;
                if (std::abs(plant->getPosition().y - z->getPosition().y) > CELL_H / 2)
                    continue;

                float dx = z->getPosition().x - plant->getPosition().x;
                if (dx > 0 && dx < CELL_W * 0.7f) {
                    z->setTargetPlant(plant.get());
                    break;
                }
            }
        }

        z->update(dt);

        if (z->getTargetPlant() != nullptr && z->tryBite()) {
            z->getTargetPlant()->takeDamage(ZOMBIE_DAMAGE);
        }

        // Шипы
        for (auto& plant : plants) {
            auto* spike = dynamic_cast<Spike*>(plant.get());
            if (!spike || spike->isBroken()) continue;
            if (std::abs(spike->getPosition().y - z->getPosition().y) > CELL_H / 2)
                continue;

            float dx = std::abs(z->getPosition().x - spike->getPosition().x);
            if (dx < CELL_W * 0.4f) {
                int damage = spike->tryHit();
                if (damage > 0) {
                    z->takeDamage(damage);
                }
            }
        }
    }

    // ==== СНАРЯДЫ ====
    for (auto& p : projectiles) p.update(dt);

    // ==== СОЛНЦА ====
    for (auto& s : suns) s.update(dt);

    // ==== СТОЛКНОВЕНИЯ ====
    for (auto& proj : projectiles) {
        if (!proj.isAlive()) continue;
        for (auto& z : zombies) {
            if (!z->isAlive()) continue;
            if (proj.getBounds().findIntersection(z->getBounds())) {
                z->takeDamage(proj.getDamage());
                proj.kill();
                break;
            }
        }
    }

    // ==== ПРОИГРЫШ ====
    int gridX = gridLeft(window);
    for (auto& z : zombies) {
        if (z->getPosition().x < gridX - 50) {
            std::cout << "Game Over!\n";
            window.close();
            return;
        }
    }

    // ==== УДАЛЕНИЕ МЁРТВЫХ ====
    plants.erase(
        std::remove_if(plants.begin(), plants.end(),
            [](const std::unique_ptr<Plant>& p) {
                if (!p->isAlive()) return true;
                auto* spike = dynamic_cast<Spike*>(p.get());
                if (spike && spike->isBroken()) return true;
                return false;
            }),
        plants.end()
    );

    zombies.erase(
        std::remove_if(zombies.begin(), zombies.end(),
            [](const std::unique_ptr<Zombie>& z) { return !z->isAlive(); }),
        zombies.end()
    );

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const Projectile& p) { return !p.isAlive(); }),
        projectiles.end()
    );

    suns.erase(
        std::remove_if(suns.begin(), suns.end(),
            [](const Sun& s) { return s.isCollected(); }),
        suns.end()
    );
}

// ===== Отрисовка =====
void Game::draw() {
    window.clear(sf::Color(100, 180, 80));
    drawGrid();

    for (auto& p : plants)      p->draw(window);
    for (auto& z : zombies)     z->draw(window);
    for (auto& p : projectiles) p.draw(window);
    for (auto& s : suns)        s.draw(window);

    drawHud();
    window.display();
}

// ===== Сетка =====
void Game::drawGrid() {
    int gridX = gridLeft(window);

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            sf::RectangleShape cell(sf::Vector2f(CELL_W - 2, CELL_H - 2));
            cell.setPosition(sf::Vector2f(
                static_cast<float>(gridX + col * CELL_W + 1),
                static_cast<float>(GRID_TOP + row * CELL_H + 1)
            ));
            if ((row + col) % 2 == 0)
                cell.setFillColor(sf::Color(90, 160, 70));
            else
                cell.setFillColor(sf::Color(110, 190, 90));
            window.draw(cell);
        }
    }
}

// ===== HUD =====
void Game::drawHud() {
    const float windowWidth = static_cast<float>(WINDOW_W);

    sf::RectangleShape hud(sf::Vector2f(windowWidth, 50));
    hud.setFillColor(sf::Color(40, 40, 40, 220));
    window.draw(hud);

    sunText.setString("Sun: " + std::to_string(sun));
    window.draw(sunText);

    sf::Text info(font, "", 22);
    info.setFillColor(sf::Color::White);

    std::string name;
    if (selectedPlant == 0) name = "Peashooter (50)";
    else if (selectedPlant == 1) name = "Sunflower (25)";
    else if (selectedPlant == 2) name = "Wallnut (25)";
    else if (selectedPlant == 3) name = "Spike (50)";
    else name = "-";

    info.setString("Selected: [1] Pea [2] Sun [3] Nut [4] Spike - " + name);

    sf::FloatRect infoBounds = info.getLocalBounds();
    info.setPosition({ (windowWidth - infoBounds.size.x) / 2.0f, 15 });
    window.draw(info);

    sf::Text zombieText(font,
        "Zombies: " + std::to_string(zombies.size()),
        22);
    zombieText.setFillColor(sf::Color::White);

    sf::FloatRect zBounds = zombieText.getLocalBounds();
    zombieText.setPosition({ windowWidth - zBounds.size.x - 80, 15 });
    window.draw(zombieText);

    // ==== Кнопка лопаты ====
    float shovelX = windowWidth - 70;
    shovelButton.setPosition({ shovelX, 5 });
    shovelButton.setFillColor(shovelSelected
        ? sf::Color(220, 160, 60)
        : sf::Color(120, 80, 40));
    window.draw(shovelButton);

    sf::Text shovelText(font, "SHOVEL", 14);
    shovelText.setFillColor(sf::Color::White);
    shovelText.setPosition({ shovelX + 2, 16 });
    window.draw(shovelText);
}

// ===== Спавн зомби =====
void Game::spawnZombie() {
    int row = rowDist(gen);
    float y = static_cast<float>(GRID_TOP + row * CELL_H + CELL_H / 2);
    float spawnX = static_cast<float>(WINDOW_W) + 50;
    sf::Vector2f pos(spawnX, y);

    int type = zombieTypeDist(gen);
    if (type < 4) {
        zombies.push_back(std::make_unique<NormalZombie>(pos));
    }
    else if (type < 6) {
        zombies.push_back(std::make_unique<ConeZombie>(pos));
    }
    else if (type < 8) {
        zombies.push_back(std::make_unique<BucketZombie>(pos));
    }
    else {
        zombies.push_back(std::make_unique<ScreenDoorZombie>(pos));
    }
}

// ===== Спавн солнца =====
void Game::spawnSun() {
    int gridX = gridLeft(window);
    std::uniform_int_distribution<> xDist(gridX, gridX + COLS * CELL_W - 50);
    std::uniform_int_distribution<> yDist(GRID_TOP, GRID_TOP + ROWS * CELL_H - 50);
    suns.emplace_back(sf::Vector2f(
        static_cast<float>(xDist(gen)),
        static_cast<float>(yDist(gen))
    ));
}

// ===== Fullscreen =====
void Game::toggleFullscreen() {
    isFullscreen = !isFullscreen;

    if (isFullscreen) {
        windowedMode = sf::VideoMode({ WINDOW_W, WINDOW_H });
        windowedPos = window.getPosition();

        window.create(sf::VideoMode::getDesktopMode(),
            "Plants vs Zombies",
            sf::Style::None);
        window.setPosition({ 0, 0 });
    }
    else {
        window.create(windowedMode,
            "Plants vs Zombies",
            sf::Style::Default);
        window.setPosition(windowedPos);
    }

    window.setFramerateLimit(60);
    updateView();
}

// ===== View (letterbox) =====
void Game::updateView() {
    sf::Vector2u windowSize = window.getSize();
    float windowRatio = static_cast<float>(windowSize.x) / windowSize.y;
    float viewRatio = static_cast<float>(WINDOW_W) / WINDOW_H;

    float sizeX = 1.0f;
    float sizeY = 1.0f;
    float posX = 0.0f;
    float posY = 0.0f;

    if (windowRatio > viewRatio) {
        sizeX = viewRatio / windowRatio;
        posX = (1.0f - sizeX) / 2.0f;
    }
    else {
        sizeY = windowRatio / viewRatio;
        posY = (1.0f - sizeY) / 2.0f;
    }

    sf::View view(sf::FloatRect({ 0, 0 }, { WINDOW_W, WINDOW_H }));
    view.setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));
    window.setView(view);
}