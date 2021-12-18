#include "GameLoop.h"
#include "GameSettings.h"

#include <chrono>
#include <thread>

using Server::GameLoop;

bool GameLoop::gameEnd() {
    return false;
}

GameLoop::GameLoop(GameEntities::GlobalEnvironment * ge) : _ge(ge){};

void GameLoop::run() {
    while (!gameEnd()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(game_tick));
        _ge->getAccess();
        _ge->tick();
        _ge->deleteObjects();   // на этапе обработки коллизий появились удаленные элементы
        _ge->finishAccess();
        _ge->generateSnapshot();
    }
}