#pragma once

// main game parameters
const int game_frequency = 10; // miliseconds

// main game parameters


#include "GlobalEnvironment.h"
#include "plug.h"

namespace Server {
    class Serializer {
    public:
        Serializer(GlobalEnvironment *);
        void run();
    private:
        GlobalEnvironment *ge;
        PhisicsManager pm;
        CollisionManager cm;

        bool gameEnd();
    };
}