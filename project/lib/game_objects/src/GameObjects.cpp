#include "GameObjects.h"

using namespace GameEntities;
using namespace boost::json;

// function for deducing parameters
template <typename T>
void extract( object const& obj, T& t, string_view key )
{
    t = value_to<T>( obj.at( key ) );
}


// Player methods definition
//Player::Player(const Player &other) {
//
//}
//Player &Player::operator=(const Player &other) {
//
//}
value Player::serialize() {
    value jv = {
        {"hp", hp},
        {"type", type},
    };
    return jv;
}

int Player::deserialize(value jv) {
    object const& obj = jv.as_object();
    extract( obj, hp, "hp" );
    extract( obj, type, "type" );
}

// Bullet methods definition
//Bullet::Bullet(const Bullet &other) {
//
//}
//Bullet &Bullet::operator=(const Bullet &other) {
//
//}
value Bullet::serialize() {
    value jv = {
        {"hp", hp},
        {"type", type},
    };
    return jv;

}
int Bullet::deserialize(value jv) {
    object const& obj = jv.as_object();
    extract( obj, hp, "hp" );
    extract( obj, type, "type" );

}

// Terrain methods definition
//Terrain::Terrain(const Terrain &other) {
//
//}
//Terrain &Terrain::operator=(const Terrain &other) {
//
//}
int Terrain::deserialize(value jv) {
    object const& obj = jv.as_object();
    extract( obj, hp, "hp" );
    extract( obj, type, "type" );

}
value Terrain::serialize() {
    value jv = {
        {"hp", hp},
        {"type", type},
    };
    return jv;

}

// Object methods definition
//Object::Object(const Object &other) {
//
//}
//Object &Object::operator=(const Object &other) {
//
//}
int Object::deserialize(value jv) {
    object const& obj = jv.as_object();
    extract( obj, hp, "hp" );
    extract( obj, type, "type" );

}
value Object::serialize() {
    value jv = {
        {"hp", hp},
        {"type", type},
    };
    return jv;
}
