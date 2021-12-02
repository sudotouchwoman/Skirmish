#pragma once
#include <vector>

#include "core.hpp"
#include "physical.hpp"

using namespace physical;
using namespace core;

namespace game
{
    struct Settings {
        const static int DEFAULT_PLAYER_HP = 100;
        const static int DEFAULT_PROJECTILE_DAMAGE = 1;
        const static double DEFAULT_PROJECTILE_SIZE;
        const static double FRAME_ELAPSED_TIME;
        const static double DEFAULT_PLAYER_SIZE;
        const static vec2 DEFAULT_PLAYER_SPAWNPOINT;
        const static int CRITICAL_HP = 0;
    };

    const vec2 Settings::DEFAULT_PLAYER_SPAWNPOINT = vec2(0.0, 0.0);
    const double Settings::DEFAULT_PROJECTILE_SIZE = 0.5;
    const double Settings::FRAME_ELAPSED_TIME = 1.0;
    const double Settings::DEFAULT_PLAYER_SIZE = 3.0;


    class Projectile;
    class Player;
    class Map;
    class Wall;

    class Projectile : public IPhysicalObject {
    private:
        size_t frames_alive = 0;
        int damage = Settings::DEFAULT_PROJECTILE_DAMAGE;
    public:
        Projectile() = default;
        Projectile(const double size, const double x, const double y);
        Projectile(const double size, const vec2 & spawnpoint);
        ~Projectile() {}

        Projectile & operator=(Projectile &&); // borrow resources
        void Tick();  // do something, e.g. update geometry
        void setDmg(const int dmg) { damage = dmg; }
        const int getDmg() const { return damage; }
    };

    class Player : public IPhysicalObject {
    private:
        int HP = Settings::DEFAULT_PLAYER_HP;
        int MAX_HP = Settings::DEFAULT_PLAYER_HP;
        // more properties here...
    public:
        Player() = default;
        Player(const int HP, const double x, const double y);
        Player(const int HP, const IShape & geometry);
        Player(const int HP, const IShape & geometry, const double mass);
        ~Player() {}

        Player & operator=(Player &&);
        void Tick();
        void onHit(const Projectile & p);
        void onDeath();
    };

    class Wall : public IPhysicalObject {
    public:
        Wall();
        Wall(
            const double x,
            const double y,
            const double w,
            const double h
            );
        Wall(const IShape & geometry);
    };

}  // namespace game

