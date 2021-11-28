#include "Serializer.h"
#include <chrono>
#include <thread>

using Server::Serializer;

bool gameEnd() {
    return false;
}

Serializer::Serializer(GlobalEnvironment * ge_) : ge(ge_){};

void Serializer::run() {
    while (!gameEnd()) {
        std::this_thread::sleep_for(std::chrono::microseconds(game_frequency));
        ge->getAccess();
        std::vector<Collision> collisions = pm.makePhisics(ge->getModifyGameObjects());
        cm.resolveCollisions(collisions, ge);
        ge->finishAccess();
    }
}