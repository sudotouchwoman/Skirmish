#include "GameLoop.h"
#include <chrono>
#include <thread>

using Server::GameLoop;

bool GameLoop::gameEnd() {
    return false;
}

GameLoop::GameLoop(GameEntities::GlobalEnvironment * ge) : _ge(ge){};

void GameLoop::run() {
    while (!gameEnd()) {
        std::this_thread::sleep_for(std::chrono::microseconds(game_frequency));
        _ge->getAccess();
//        pm.makePhisics(_ge->getModifyGameObjects());
        _ge->deleteObjects();   // на этапе обработки коллизий появились уадаленные элементы
        _ge->finishAccess();
        _ge->generateSnapshot();
    }
}