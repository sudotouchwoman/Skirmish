// _LIB_PHYSICS_INC_MANAGER_HPP
#pragma once
#include <vector>

#include "physical.hpp"

namespace physical
{
    using CollisionSPtr = std::shared_ptr<Collision>;
    using Collisions = std::vector<CollisionSPtr>;
    using PhObjects = std::vector<IPObjectSPtr>;

    class PhysicManager {
    public:
        PhysicManager() = default;
        ~PhysicManager() = default;
        Collisions findCollisions(const PhObjects & objects) const;
    };
} // namespace physical

// _LIB_PHYSICS_INC_MANAGER_HPP
