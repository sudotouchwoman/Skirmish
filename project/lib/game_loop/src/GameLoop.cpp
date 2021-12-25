#include "GameLoop.h"
#include "GameSettings.h"

#include <chrono>
#include <thread>
#include <exception>

using Server::GameLoop;

bool GameLoop::gameEnd() {
    return _ge->gameEnd();
}

GameLoop::GameLoop(GameEntities::GlobalEnvironment * ge) : _ge(ge){
    if (_ge == nullptr)
        throw std::runtime_error("Bad GlobalEnvironment");
};

void GameLoop::run() {
    while (!gameEnd()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(game_tick));
        _ge->tick();
        _ge->generateSnapshot();
    }
    _ge->finishGame();
}