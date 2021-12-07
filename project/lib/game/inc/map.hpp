#pragma once

#include "core.hpp"
#include "physical.hpp"
#include "object.hpp"

using namespace physical;
using namespace core;

namespace game {
    struct Settings {
        const static int PLAYER_HP = 100;
        const static int PROJECTILE_DAMAGE = 1;
        const static double PROJECTILE_SIZE;
        const static double FRAME_ELAPSED_TIME;
        const static double PLAYER_SIZE;
        const static vec2 PLAYER_SPAWNPOINT;
        const static int CRITICAL_HP = 0;
    };

    const vec2 Settings::PLAYER_SPAWNPOINT = vec2(0.0, 0.0);
    const double Settings::PROJECTILE_SIZE = 0.5;
    const double Settings::FRAME_ELAPSED_TIME = 1.0;
    const double Settings::PLAYER_SIZE = 3.0;

    class Map;

    // class Projectile : public PhysicalObject {
    // private:
    //     size_t frames_alive = 0;
    //     int damage = Settings::PROJECTILE_DAMAGE;
    // public:
    //     Projectile() = default;
    //     Projectile(const double size, const double x, const double y);
    //     Projectile(const double size, const vec2 & spawnpoint);
    //     ~Projectile() {}

    //     Projectile & operator=(Projectile &&); // borrow resources
    //     void Tick();  // do something, e.g. update geometry
    //     void setDmg(const int dmg) { damage = dmg; }
    //     const int getDmg() const { return damage; }
    // };

    // class Player : public PhysicalObject {
    // private:
    //     int HP = Settings::PLAYER_HP;
    //     int MAX_HP = Settings::PLAYER_HP;
    //     // more properties here...
    // public:
    //     Player() = default;
    //     Player(const int HP, const double x, const double y);
    //     Player(const int HP, const IShape & geometry);
    //     Player(const int HP, const IShape & geometry, const double mass);
    //     ~Player() {}

    //     Player & operator=(Player &&);
    //     void Tick();
    //     void onHit(const Projectile & p);
    //     void onDeath();
    // };

    // class Wall : public PhysicalObject {
    // public:
    //     Wall();
    //     Wall(
    //         const double x,
    //         const double y,
    //         const double w,
    //         const double h
    //         );
    //     Wall(const IShape & geometry);
    // };

}  // namespace game

