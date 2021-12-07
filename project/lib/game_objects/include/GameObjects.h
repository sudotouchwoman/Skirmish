#pragma once

#include <boost/json.hpp>
#include "plug.h"
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
//        virtual void eventHandler(const ClientServer::Eve &ev) = 0;
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
        virtual void deserialize(value) = 0;
    };

    class GameObject : public Logic, public IRenderable, public ISerializeble {
    private:
        int type;
        int id;
        std::unique_ptr<PhysicalObject> model;

    public:
        GameObject(int type_, int id_) : type(type_), id(id_){};
        int getType() { return type; };
        int getID() { return id; };
        void setModel(std::unique_ptr<PhysicalObject> & model_) {model = std::move(model_);}

        value serialize();
        void deserialize(value);
        bool deleted = false;
        std::unique_ptr<PhysicalObject> &getModel();
    };

    class Player : public GameObject {
    public:
        Player(int hp_, int type_, int id_) : hp(hp_), GameObject(type_, id_) {}
        Player(): GameObject(ObjectTypes::tPlayer, 1), hp(100) {}
        ~Player() = default;
        Player(const Player &other);
        Player &operator=(const Player &other);
        Player(Player &&other) = delete;
        Player &operator=(Player &&other) = delete;

        value serialize();
        void deserialize(value);

        int update(){return 0;};
        int render(){return 0;};
        int getPosition(){return 0;};

        int getHp();

        void collisionHandler(const std::shared_ptr<GameObject> &other);
//        void eventHandler(const ClientServer::Event &ev);
    private:
        std::string name;
        int hp;
    };

    class Bullet : public GameObject {
    public:
        Bullet(int hp_, int type_, int id_, int damage_, PhysicalObject i_physical_object) : GameObject(hp_, type_, id_, damage_, std::make_unique<PhysicalObject>(i_physical_object)){};
        Bullet(): GameObject(ObjectTypes::tBullet, 1, 10, std::make_unique<PhysicalObject>(PhysicalObject())){};
        ~Bullet() = default;
        Bullet(const Bullet &other);
        Bullet &operator=(const Bullet &other);
        Bullet(Bullet &&other) = delete;
        Bullet &operator=(Bullet &&other) = delete;

        value serialize();
        int deserialize(value);

        void collisionHandler(const std::shared_ptr<GameObject> &other);
        void eventHandler(const ClientServer::Event &ev);

    private:
        int damage;
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
