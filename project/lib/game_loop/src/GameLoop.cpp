#include "GameLoop.h"
#include <chrono>
#include <thread>

using Server::GameLoop;

bool gameEnd() {
    return false;
}

GameLoop::GameLoop(GlobalEnvironment * ge_) : ge(ge_){};

void GameLoop::run() {
    while (!gameEnd()) {
        std::this_thread::sleep_for(std::chrono::microseconds(game_frequency));
        ge->getAccess();
        std::vector<Collision> collisions = pm.makePhisics(ge);
        cm.resolveCollisions(collisions, ge);
        ge->finishAccess();
    }
}