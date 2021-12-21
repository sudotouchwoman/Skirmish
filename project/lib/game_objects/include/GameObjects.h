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
        IRenderable(float x_ = 0, float y_ = 0, float angle_ = 0, size_t id_ = 1) : x(x_), y(y_), angle(angle_), texture_id(id_){};
        IRenderable(IRenderable &&);
        IRenderable(const IRenderable &) = delete;
        IRenderable &operator=(IRenderable &&);
        IRenderable &operator=(const IRenderable &) = delete;
        virtual ~IRenderable() = default;


        virtual int update() = 0;
        virtual int render() = 0;
        float getX() const { return x; };
        float getY() const { return y; };
        size_t getTextureId() const { return texture_id; };
        float getAngle() const { return angle; };
        void setX(float x_) {x = x_; };
        void setY(float y_) {y = y_; };
        void setAngle(float angle_) {angle = angle_; };
        void setTextureId(size_t texture_id_) {texture_id = texture_id_; };
    private:
        float x;
        float y;
        float angle;
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
        void setDefaultModel(const double vx, const double vy, const double inverse_mass);

    public:
        static void resetID() { global_id = 1; };
        GameObject(ObjectTypes type, float x = 0, float y = 0) : type_(type), id(global_id++), IRenderable(x, y) {};
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
        Player(int hp_, ObjectTypes type_) : hp(hp_), GameObject(type_) {}
        Player(const double x = default_spawn_x,
               const double y = default_spawn_y,
               const double R = default_player_radius,
               const double inverse_mass = 1);

        // client side constructors
        Player(int hp_, ObjectTypes type_, std::string &&name_, float x, float y) : GameObject(type_, x, y), hp(hp_), name(std::move(name_)) {};

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
        void setVanity(const ClientServer::RegisterEvent &event);

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
        Bullet(int damage_, ObjectTypes type_) : damage(damage_), GameObject(type_) {}
        Bullet(const double x = default_spawn_x,
               const double y = default_spawn_y,
               const double R = default_bullet_radius,
               const double inverse_mass = 1);

        // client side constructors
        Bullet(ObjectTypes type_, int damage_, float x, float y) : GameObject(type_, x, y), damage(damage_) {};

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
}
