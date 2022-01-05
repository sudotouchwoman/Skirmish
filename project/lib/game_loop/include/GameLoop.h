#pragma once

#include "GlobalEnvironment.h"
#include "physical.hpp"

namespace Server {
    class GameLoop {
    public:
        GameLoop(GameEntities::GlobalEnvironment *);
        void run();
    private:
        GameEntities::GlobalEnvironment *_ge;

        bool gameEnd();
    };
}