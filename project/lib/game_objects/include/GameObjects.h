#pragma once

#include <boost/json.hpp>
#include "PlayerEvent.h"
#include "physical.hpp"
#include "GameSettings.h"

using namespace boost::json;

namespace GameEntities {

    using physical_uptr = std::unique_ptr<physical::PhysicalObject>;

    class Bullet;
    class Player;
    class Terrain;

    enum class ObjectTypes {
        T_PLAYER,
        T_OBSTACLE,
        T_BULLET,
        T_WEAPON,
        T_VANITY,
        T_ECQUPABLE,
        T_OTHER,
    };

    class GameObject;

    // lightweight object for using on client side only
    class IRenderable {
    public:
        IRenderable() = default;
        IRenderable(const IRenderable &) = default;
        IRenderable &operator=(const IRenderable &) = default;
        virtual ~IRenderable() = default;

        float x_;
        float y_;
        float w_, h_;
        float angle_;
        size_t texture_id;
    };

    class ISerializeble {
    public:
        virtual value serialize() = 0;
        virtual void deserialize(value) = 0;
    };

    class GameObject : public IRenderable, public ISerializeble {
    private:
        static size_t global_id;
    protected:
        ObjectTypes type_;
        size_t id;
        std::unique_ptr<physical::PhysicalObject> model;

        void setDefaultGeometry(const double x, const double y, const double R);
        void setDefaultGeometry(const double x, const double y, const double w, const double h);
        void setDefaultModel(const double vx, const double vy, const double inverse_mass);
    public:
        // only for client purposes
        GameObject() = default;
        // only for client purposes

        static void resetID() { global_id = 1; };
        GameObject(ObjectTypes type) : type_(type), id(global_id++) {};
        GameObject(GameObject &&);
        GameObject(const GameObject &) = delete;
        GameObject &operator=(GameObject &&);
        GameObject &operator=(const GameObject &) = delete;
        virtual ~GameObject() = default;

        ObjectTypes getType() const { return type_; };
        size_t getID() const { return id; };
        void setModel(std::unique_ptr<physical::PhysicalObject> model_) { model = std::move(model_); }
        bool hasModel() const { return static_cast<bool> (model); }
        physical::PhysicalObject &getModel() {
            if (!model) throw std::runtime_error("ERROR BIM BIM BAM BAM");
            return *model;
        };

        value serialize();
        void deserialize(value);
        bool deleted = false;
    };

    class Player : public GameObject {
    public:
        // server side constructors
        Player(const int hp_,
               ObjectTypes type_ = ObjectTypes::T_PLAYER,
               const double x = default_spawn_x,
               const double y = default_spawn_y,
               const double R = default_player_radius,
               const double inverse_mass = default_player_in_mass);

        // client side constructors
        Player() = default;

        ~Player() = default;
        Player(const Player &other) = delete;
        Player &operator=(const Player &other) = delete;
        Player(Player &&other);
        Player &operator=(Player &&other);

        value serialize();
        void deserialize(value);

        void setVanity(const ClientServer::RegisterEvent &event);

        int getHp() { return hp; };

        void collisionHandler(Player const &other);
        void collisionHandler(Bullet const &other);
        void collisionHandler(Terrain const &other);

        void eventHandler(const ClientServer::MoveEvent &ev);
        void eventHandler(const ClientServer::InteractEvent &ev);
    private:
        std::string name;
        int hp;
    };

    class Bullet : public GameObject {
    public:
        // server side constructors
        Bullet(int damage_, ObjectTypes type_ = ObjectTypes::T_BULLET,
               const double inverse_mass = default_bullet_in_mass,
               const double x = default_spawn_x,
               const double y = default_spawn_y,
               const double R = default_bullet_radius);

        // client side constructors
        Bullet() = default;

        ~Bullet() = default;
        Bullet(const Bullet &other) = delete;
        Bullet &operator=(const Bullet &other) = delete;
        Bullet(Bullet &&other);
        Bullet &operator=(Bullet &&other);

        value serialize();
        void deserialize(value);

        int getDamage() const { return damage; };

        void collisionHandler(Player const &other);
        void collisionHandler(Terrain const &other);
        void collisionHandler(Bullet const &other);
    private:
        int damage;
    };

    class Terrain : public GameObject {
    public:
        // server side constructors
        Terrain(
            const double x,
            const double y,
            const double w,
            const double h,
            ObjectTypes type_ = ObjectTypes::T_OBSTACLE,
            const double inverse_mass = default_terrain_in_mass);

        // client side constructors
        Terrain() = default;

        ~Terrain() = default;
        Terrain(const Terrain &other) = delete;
        Terrain &operator=(const Terrain &other) = delete;
        Terrain(Terrain &&other);
        Terrain &operator=(Terrain &&other);

        value serialize();
        void deserialize(value);

        void collisionHandler(Player const &other);
        void collisionHandler(GameEntities::Bullet const &other);
        void collisionHandler(Terrain const &other);
    };

    class Object : public GameObject {
//    public:
//        Object() = default;
//        ~Object() = default;
//        value serialize();
//        int deserialize(value);
//        Object(const Object &other);
//        Object &operator=(const Object &other);
//        Object(Object &&other) = delete;
//        Object &operator=(Object &&other) = delete;
//
//        void ollisionHandler(const std::shared_ptr<GameObject> &other);
//        void eventHandler(const Event &ev);
//    private:
//        int hp;
//        int type = ObjectTypes::tOther;
    };
}
