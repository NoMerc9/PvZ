#ifdef _WIN32
#include <windows.h>
#include <fstream>
#endif

#include "Game.h"

int main() {
#ifdef _WIN32
    SetProcessDPIAware();   // отключаем DPI-масштабирование
#endif

    Game game;
    game.run();
    return 0;
}