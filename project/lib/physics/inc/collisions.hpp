#pragma once

#include <functional>

#include "core.hpp"
#include "physical.hpp"

namespace physical {
    // helper structure to store information
    // about two colliding objects

    // collision class, wrapper around two colliding
    // objects
    // std::pair (how extraordinary!) of references
    // my question here is about the lifetime of the referenced objects
    // should these be references? how would server update their state if
    // the reference does not posess interface for a game object?
    // dynamic cast will be needed?
    class Collision {
    private:
        std::pair<IPhysicalObject &, IPhysicalObject &> colliding;
        core::ContactPoint where;
    public:
        // one cannot create collision without
        // specifying colliding objects explicitly
        Collision() = delete;
        Collision(IPhysicalObject & a, IPhysicalObject & b) :
            colliding { a, b } {}
        ~Collision() = default;
        void addContactPoint(const core::ContactPoint & point) { where = point; }
        IPhysicalObject & getFirst() { return colliding.first; }
        IPhysicalObject & getSecond() { return colliding.second; }
        const core::ContactPoint & getContactPoint() const { return where; }
    };

    // we need a function or set of functions
    // to detect and resolve upcoming collisions
    // the problem itself is that the shapes do not know
    // anything about the collision info
    // they only can detect intersection fact

    // UPD:
    // the methods for intersection detection
    // were refactored and they now yield
    // the same bool-castable result
    // along with additional information
    // which can be applied to create responsive
    // physical objects

    // TODO:
    // implement map
    // implement players/bullets/walls
    // implement collision resolution engine
    // wrap the latter into IPhysical object
}
