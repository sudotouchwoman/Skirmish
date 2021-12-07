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

    class Projectile: public GameObject {

    };

    class Player: public GameObject {

    };

    class Wall: public GameObject {

    };

    // helper structure to store information
    // about two colliding objects

    // collision class, wrapper around two colliding
    // objects
    // std::pair (how extraordinary!) of references
    // my question here is about the lifetime of the referenced objects
    // should these be references? how would server update their state if
    // the reference does not posess interface for a game object?
    // dynamic cast will be needed?
    template<typename T1, typename T2>
    class Collision {
    private:
        std::pair<T1 &, T2 &> colliding;
        core::ContactPoint where;
    public:
        // one cannot create collision without
        // specifying colliding objects explicitly
        Collision() = delete;
        Collision(T1 & a, T2 & b) :
            colliding { a, b } {}
        ~Collision() = default;
        T1 & getFirst() { return colliding.first; }
        T2 & getSecond() { return colliding.second; }
        void setContactPoint(const core::ContactPoint & point) { where = point; }
        const core::ContactPoint & getContactPoint() const { return where; }
    };
}  // namespace game

