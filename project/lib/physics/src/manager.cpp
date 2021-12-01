#include "manager.hpp"

namespace physical
{
    Collisions PhysicManager::findCollisions(const PhObjects & objects) const {
        Collisions all_collisions;
        const auto end = objects.end();

        for (auto obj_ptr = objects.begin(); obj_ptr != end; ++obj_ptr) {
            const auto a = (*obj_ptr);

            for (auto other_ptr = obj_ptr + 1; other_ptr != end; ++other_ptr) {
                const auto b = (*other_ptr);

                if (a->CollidesWith( *b.get() )) {
                    all_collisions.push_back(
                        std::make_shared<Collision>(a, b)
                        );
                }
            }
        }

        return all_collisions;
    }
}  // namespace physical
