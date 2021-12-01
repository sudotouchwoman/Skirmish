#pragma once

#include "GlobalEnvironment.h"
#include "plug.h"

// main game parameters
const int game_frequency = 10; // miliseconds
// main game parameters

namespace Server {
    class GameLoop {
    public:
        GameLoop(GameEntities::GlobalEnvironment *);
        void run();
    private:
        GameEntities::GlobalEnvironment *_ge;
        PhisicsManager pm;
        CollisionManager cm;

        bool gameEnd();
    };
}