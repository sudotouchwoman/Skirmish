// _LIB_GAME_INC_OBJECT_HPP
#pragma once

#include <vector>

#include "core.hpp"
#include "physical.hpp"

namespace game {
    class GameObject;
    class Projectile;
    class Player;
    class Wall;

    // useful typedefs
    using GameObjectUPtr = std::unique_ptr<game::GameObject>;
    using GameObjects = std::vector<GameObjectUPtr>;

    // plug for base game entity class
    class GameObject {

    };

    // plugs for other game objects
    class Projectile: public GameObject {

    };

    class Player: public GameObject {

    };

    class Wall: public GameObject {

    };

    // helper structure to store information
    // about two colliding objects

}  // namespace game
// _LIB_GAME_INC_OBJECT_HPP
