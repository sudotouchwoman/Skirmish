#include "map.hpp"

// namespace game {

//     Projectile::Projectile(const double size, const double x, const double y) :
//         MovableObject(Circle(x, y, size)) {}

//     Projectile::Projectile(const double size, const vec2 & spawnpoint) :
//         MovableObject(Circle(spawnpoint, size)) {}

//     Projectile & Projectile::operator=(Projectile && other) {
//         geometry = std::move(other.geometry);
//         geometry_shadow_copy = std::move(other.geometry_shadow_copy);
//         state = other.state;
//         frames_alive = other.frames_alive;
//     }

//     void Projectile::Tick() {
//         // do something, e.g.
//         MovableObject::update(Settings::FRAME_ELAPSED_TIME);
//     }

//     Player::Player(const int HP, const double x, const double y) :
//         MovableObject(
//             Circle(x, y, Settings::DEFAULT_PLAYER_SIZE)
//             ),
//             HP(HP) {}

//     Player::Player(const int HP, const IShape & geometry) :
//         MovableObject(geometry), HP(HP) {}

//     Player::Player(const int HP, const IShape & geometry, const double mass) :
//         MovableObject(geometry, State(mass)), HP(HP) {}

//     Player & Player::operator=(Player && other) {
//         geometry = std::move(other.geometry);
//         geometry_shadow_copy = std::move(other.geometry_shadow_copy);
//         state = other.state;
//         HP = other.HP;
//         MAX_HP = other.MAX_HP;
//     }

//     void Player::onHit(const Projectile & p) {
//         const auto damage_taken = p.getDmg();
//         HP -= damage_taken;
//         if (HP <= Settings::CRITICAL_HP) onDeath();
//     }

// }