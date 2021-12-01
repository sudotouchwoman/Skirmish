#pragma once

#include <boost/json/src.hpp>

using namespace boost::json;

namespace GameEntities {
    class Logic {
    public:
        virtual int getType() = 0;
        virtual int getHP() = 0;
    };

    class IRenderable {
    public:
        virtual int Render() = 0;
    };

    class ISerializeble {
    public:
        virtual value serialize() = 0;
        virtual int deserialize(value) = 0;
    };

    class GameObject: public Logic, public IRenderable, public ISerializeble {};

    class Player : public GameObject {
    public:
        Player() = default;
        ~Player() = default;
        value serialize();
        int deserialize(value);
        Player(const Player &other);
        Player & operator=(const Player &other);
        Player(Player &&other) = delete;
        Player & operator=(Player &&other) = delete;
    private:
        int hp;
        int type;
        std::string Name;
    };

    class Bullet : public GameObject {
    public:
        Bullet() = default;
        ~Bullet() = default;
        value serialize();
        int deserialize(value);
        Bullet(const Bullet &other);
        Bullet & operator=(const Bullet &other);
        Bullet(Bullet &&other) = delete;
        Bullet & operator=(Bullet &&other) = delete;
    private:
        int hp;
        int type;
    };

    class Terrain : public GameObject {
    public:
        Terrain() = default;
        ~Terrain() = default;
        value serialize();
        int deserialize(value);
        Terrain(const Terrain &other);
        Terrain & operator=(const Terrain &other);
        Terrain(Terrain &&other) = delete;
        Terrain & operator=(Terrain &&other) = delete;
    private:
        int hp;
        int type;
    };

    class Object : public GameObject {
    public:
        Object() = default;
        ~Object() = default;
        value serialize();
        int deserialize(value);
        Object(const Object &other);
        Object & operator=(const Object &other);
        Object(Object &&other) = delete;
        Object & operator=(Object &&other) = delete;
    private:
        int hp;
        int type;
    };

//    void tag_invoke(value_from_tag, value &jv, Player const &p) {
//        jv = {
//            {"id", c.id},
//            {"name", c.name},
//            {"current", c.current}
//        };
//    }
//
//    Player tag_invoke( value_to_tag< Player >, value const& jv )
//    {
//        object const& obj = jv.as_object();
//        return Player {
//            value_to<int>( obj.at( "id" ) ),
//            value_to<std::string>( obj.at( "name" ) ),
//            value_to<bool>( obj.at( "current" ) )
//        };
//    }

}
