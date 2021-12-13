#pragma once

#include <boost/json.hpp>
#include "PlayerEvent.h"
#include "physical.hpp"

using namespace boost::json;

namespace GameEntities {

    using PhobjUPtr = std::unique_ptr<physical::PhysicalObject>;

    class Bullet;
    class Player;

    enum ObjectTypes {
        tPlayer,
        tObstacle,
        tBullet,
        tWeapon,
        tVanity,
        tEcqupable,
        tOther,
    };

    class GameObject;

    class Logic {
    private:
//        virtual void collisionHandler(const std::shared_ptr<GameObject> &other) = 0;
    };

    // lightweight object for using on client side only
    class IRenderable {
    public:
        IRenderable(float x_ = 0, float y_ = 0) : x(x_), y(y_) {};
        virtual int update() = 0;
        virtual int render() = 0;
        float getX() const { return x; };
        float getY() const { return y; };
        void setX(float x_) {x = x_; };
        void setY(float y_) {y = y_; };
    private:
        float x;
        float y;
    };

    class ISerializeble {
    public:
        virtual value serialize() = 0;
        virtual void deserialize(value) = 0;
    };

    class GameObject : public Logic, public IRenderable, public ISerializeble {
    private:
        static size_t global_id;
    protected:
        int type;
        int id;
        std::unique_ptr<physical::PhysicalObject> model;

        void setDefaultGeometry(const double x, const double y, const double R);
        void setDefaultModel(const double vx, const double vy, const double inverse_mass);

    public:
        static void resetID() { global_id = 1; };
        GameObject(int type_, float x = 0, float y = 0) : type(type_), id(global_id++), IRenderable(x, y) {};
        GameObject(GameObject &&);
        GameObject(const GameObject &) = delete;
        GameObject &operator=(GameObject &&);
        GameObject &operator=(const GameObject &) = delete;

        int getType() const { return type; };
        int getID() const { return id; };
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
        Player(int hp_, int type_) : hp(hp_), GameObject(type_) {}
        Player(const double x = 0,
               const double y = 0,
               const double R = 4,
               const double vx = 0,
               const double vy = 0,
               const double inverse_mass = 1);

        // client side constructors
        Player(int hp_, int type_, std::string &&name_, float x, float y) : GameObject(type_, x, y), hp(hp_), name(std::move(name_)) {};

        ~Player() = default;
        Player(const Player &other) = delete;
        Player &operator=(const Player &other) = delete;
        Player(Player &&other);
        Player &operator=(Player &&other);

        value serialize();
        void deserialize(value);

        int update() { return 0; };
        int render() { return 0; };
        int getPosition() { return 0; };

        int getHp() { return hp; };

        void collisionHandler(Player const &other);
        void collisionHandler(GameEntities::Bullet const &other);

        void eventHandler(const ClientServer::MoveEvent &ev);
        void eventHandler(const ClientServer::InteractEvent &ev);
    private:
        std::string name;
        int hp;
    };

    class Bullet : public GameObject {
    public:
        // server side constructors
        Bullet(int damage_, int type_) : damage(damage_), GameObject(type_) {}
        Bullet(const double x = 0,
               const double y = 0,
               const double R = 1,
               const double vx = 0,
               const double vy = 0,
               const double inverse_mass = 1);

        // client side constructors
        Bullet(int type_, int damage_, float x, float y) : GameObject(type_, x, y), damage(damage_) {};

        ~Bullet() = default;
        Bullet(const Bullet &other) = delete;
        Bullet &operator=(const Bullet &other) = delete;
        Bullet(Bullet &&other);
        Bullet &operator=(Bullet &&other);

        value serialize();
        void deserialize(value);

        int update() { return 0; };
        int render() { return 0; };
        int getPosition() { return 0; };

        int getDamage() const { return damage; };

        void collisionHandler(Player const &other);
        void collisionHandler(GameEntities::Bullet const &other);
    private:
        int damage;
    };

    class Terrain : public GameObject {
//    public:
//        Terrain() = default;
//        ~Terrain() = default;
//        value serialize();
//        int deserialize(value);
//        Terrain(const Terrain &other);
//        Terrain &operator=(const Terrain &other);
//        Terrain(Terrain &&other) = delete;
//        Terrain &operator=(Terrain &&other) = delete;
//
//        void сollisionHandler(const std::shared_ptr<GameObject> &other);
//        void eventHandler(const Event &ev);
//    private:
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
        Collision(T1 &a, T2 &b) :
            colliding{a, b} {}
        ~Collision() = default;
        T1 &getFirst() { return colliding.first; }
        T2 &getSecond() { return colliding.second; }
        void setContactPoint(const core::ContactPoint &point) { where = point; }
        const core::ContactPoint &getContactPoint() const { return where; }
    };
}
