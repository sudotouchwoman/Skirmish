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
//        std::vector<Collision> collisions = pm.makePhisics(_ge->getModifyGameObjects());
//        cm.resolveCollisions(collisions, _ge);
        _ge->finishAccess();
        _ge->generateSnapshot();
    }
}