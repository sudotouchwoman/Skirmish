#pragma once

#include <boost/json.hpp>
#include "PlayerEvent.h"

using namespace boost::json;

namespace GameEntities {

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
        virtual void collisionHandler(const std::shared_ptr<GameObject> &other) = 0;
        virtual void eventHandler(const ClientServer::Event &ev) = 0;
    };

    class IRenderable {
    public:
        virtual int update() = 0;
        virtual int render() = 0;
        virtual int getPosition() = 0;
    };

    class ISerializeble {
    public:
        virtual value serialize() = 0;
        virtual int deserialize(value) = 0;
    };

    class GameObject : public Logic, public IRenderable, public ISerializeble {
    private:
        int hp;
        int type;
        int id;
        int damage;
        std::unique_ptr<IPhysicalObject> model;
    public:
        GameObject(int hp_, int type_, int id_, int damage_, std::unique_ptr<IPhysicalObject> model_) :model(model_), hp(hp_), type(type_), id(id_), damage(damage_){};
        virtual int getType() { return type; };
        virtual int getHP() { return hp; };
        virtual int getID() { return id; };
        virtual int getDamage() { return damage; };

        bool deleted = false;
        std::unique_ptr<IPhysicalObject> &getModel();
    };

    class Weapon : public GameObject {

    };

    class Player : public GameObject {
    public:
        Player(int hp_, int type_, int id_, int damage_, IPhysicalObject i_physical_object) : GameObject(hp_, type_, id_, damage_, std::make_unique<IPhysicalObject>(i_physical_object)){};
        Player(): GameObject(100, ObjectTypes::tPlayer, 1, 0, std::make_unique<IPhysicalObject>(IPhysicalObject())){};
        ~Player() = default;
        Player(const Player &other);
        Player &operator=(const Player &other);
        Player(Player &&other) = delete;
        Player &operator=(Player &&other) = delete;

        value serialize();
        int deserialize(value);

        void collisionHandler(const std::shared_ptr<GameObject> &other);
        void eventHandler(const ClientServer::Event &ev);
    private:
        std::string Name;
    };

    class Bullet : public GameObject {
    public:
        Bullet(int hp_, int type_, int id_, int damage_, IPhysicalObject i_physical_object) : GameObject(hp_, type_, id_, damage_, std::make_unique<IPhysicalObject>(i_physical_object)){};
        Bullet(): GameObject(100, ObjectTypes::tBullet, 1, 10, std::make_unique<IPhysicalObject>(IPhysicalObject())){};
        ~Bullet() = default;
        value serialize();
        int deserialize(value);
        Bullet(const Bullet &other);
        Bullet &operator=(const Bullet &other);
        Bullet(Bullet &&other) = delete;
        Bullet &operator=(Bullet &&other) = delete;

        void collisionHandler(const std::shared_ptr<GameObject> &other);
        void eventHandler(const ClientServer::Event &ev);

    private:
    };

    class Terrain : public GameObject {
    public:
        Terrain() = default;
        ~Terrain() = default;
        value serialize();
        int deserialize(value);
        Terrain(const Terrain &other);
        Terrain &operator=(const Terrain &other);
        Terrain(Terrain &&other) = delete;
        Terrain &operator=(Terrain &&other) = delete;

        void сollisionHandler(const std::shared_ptr<GameObject> &other);
        void eventHandler(const Event &ev);
    private:
        int hp;
        int type = ObjectTypes::tObstacle;
    };

    class Object : public GameObject {
    public:
        Object() = default;
        ~Object() = default;
        value serialize();
        int deserialize(value);
        Object(const Object &other);
        Object &operator=(const Object &other);
        Object(Object &&other) = delete;
        Object &operator=(Object &&other) = delete;

        void ollisionHandler(const std::shared_ptr<GameObject> &other);
        void eventHandler(const Event &ev);
    private:
        int hp;
        int type = ObjectTypes::tOther;
    };

}
