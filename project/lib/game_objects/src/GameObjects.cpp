#include "GameObjects.h"

using namespace GameEntities;
using namespace boost::json;

// function for deducing parameters
template<typename T>
void extract(object const &obj, T &t, string_view key) {
    t = value_to<T>(obj.at(key));
}

void GameObject::deserialize(value jv) {
    object ob = jv.as_object();
    extract(ob, type, "type");
    extract(ob, id, "id");

    // ?????
    extract(ob, ob, "Physical");

    // extract to pointer of physical object
}

GameObject &GameObject::operator=(GameObject &&other) {
    model = std::move(other.model);
    other.model = nullptr;
    id = other.id;
    type = other.type;
    return *this;
}

GameObject::GameObject(GameObject && other){
    model = std::move(other.model);
    other.model = nullptr;
    id = other.id;
    type = other.type;
}

value GameObject::serialize() {
    value jv = {
        {"type", type},
        {"id", id},
        {"Physical", {
            // objects of physical
        }}
    };
    return jv;
}

value Player::serialize() {
    value jv = {
        {"hp", hp},
        {"name", name},
        {"GameObject", GameObject::serialize()}
    };
    return jv;
}

Player &Player::operator=(Player &&other) {
    name = std::move(other.name);
    hp = other.hp;
    GameObject::operator=(std::move(other));
    return *this;
}

Player::Player(Player &&other): GameObject(std::move(other)) {
    name = std::move(other.name);
    hp = other.hp;
    GameObject::operator=(std::move(other));
}

void Player::deserialize(value jv) {
    object const &obj = jv.as_object();
    extract(obj, hp, "hp");
    extract(obj, name, "name");

    extract(obj, jv, "GameObject");
    GameObject::deserialize(jv);
}

// Bullet methods definition
//Bullet::Bullet(const Bullet &other) {
//
//}
//Bullet &Bullet::operator=(const Bullet &other) {
//
//}
//value Bullet::serialize() {
//    value jv = {
//        {"hp", hp},
//        {"type", type},
//    };
//    return jv;
//
//}
//int Bullet::deserialize(value jv) {
//    object const& obj = jv.as_object();
//    extract( obj, hp, "hp" );
//    extract( obj, type, "type" );
//
//}
//
//// Terrain methods definition
////Terrain::Terrain(const Terrain &other) {
////
////}
////Terrain &Terrain::operator=(const Terrain &other) {
////
////}
//int Terrain::deserialize(value jv) {
//    object const& obj = jv.as_object();
//    extract( obj, hp, "hp" );
//    extract( obj, type, "type" );
//
//}
//value Terrain::serialize() {
//    value jv = {
//        {"hp", hp},
//        {"type", type},
//    };
//    return jv;
//
//}
//
//// Object methods definition
////Object::Object(const Object &other) {
////
////}
////Object &Object::operator=(const Object &other) {
////
////}
//int Object::deserialize(value jv) {
//    object const& obj = jv.as_object();
//    extract( obj, hp, "hp" );
//    extract( obj, type, "type" );
//
//}
//value Object::serialize() {
//    value jv = {
//        {"hp", hp},
//        {"type", type},
//    };
//    return jv;
//}
