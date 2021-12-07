// _LIB_GAME_INC_MAP_HPP
#pragma once

#include "core.hpp"
#include "physical.hpp"
#include "object.hpp"

namespace game {
    // some (i guess) useful constants
    // it would be generally a good idea to store
    // game config in some kind of a config
    struct Settings {
        const static int PLAYER_HP = 100;
        const static int PROJECTILE_DAMAGE = 1;
        const static double PROJECTILE_SIZE;
        const static double FRAME_ELAPSED_TIME;
        const static double PLAYER_SIZE;
        const static core::vec2 PLAYER_SPAWNPOINT;
        const static int CRITICAL_HP = 0;
    };

    const core::vec2 Settings::PLAYER_SPAWNPOINT = core::vec2(0.0, 0.0);
    const double Settings::PROJECTILE_SIZE = 0.5;
    const double Settings::FRAME_ELAPSED_TIME = 1.0;
    const double Settings::PLAYER_SIZE = 3.0;

    // map, or global environment, if you wish
    // is supposed to contain all (at least on game logic level)
    // data, i.e. players, obstacles, projectiles...
    class Map;
}  // namespace game
// _LIB_GAME_INC_MAP_HPP
