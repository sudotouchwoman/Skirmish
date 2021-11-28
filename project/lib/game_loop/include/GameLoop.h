#pragma once

// main game parameters
const int game_frequency = 10; // miliseconds

// main game parameters


#include "GlobalEnvironment.h"
#include "plug.h"

namespace Server {
    class GameLoop {
    public:
        GameLoop(GlobalEnvironment *);
        void run();
    private:
        GlobalEnvironment *ge;
        PhisicsManager pm;
        CollisionManager cm;

        bool gameEnd();
    };
}