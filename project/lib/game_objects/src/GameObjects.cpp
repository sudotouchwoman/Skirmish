#include "GameObjects.h"
#include "physical.hpp"

using namespace GameEntities;
using namespace boost::json;

// function for deducing parameters
template<typename T>
void extract(object const &obj, T &t, string_view key) {
    t = value_to<T>(obj.at(key));
}

void GameObject::deserialize(value jv) {
    core::vec2 shift;
    object ob = jv.as_object();
    extract(ob, type, "type");
    extract(ob, id, "id");

    extract(ob, ob, "Physical");
    extract(ob, shift.x, "geometry_x");
    extract(ob, shift.y, "geometry_y");

    model->getGeometry().shift(shift);
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
    const auto & geometry = model->getGeometry().GetCenter();

    value jv = {
        {"type", type},
        {"id", id},
        {"Physical", {
            {"geometry_x", geometry.x},
            {"geometry_y", geometry.y},
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


void Player::eventHandler(const ClientServer::MoveEvent &ev){
    using namespace ClientServer;
    core::vec2 velocity;
    switch(ev.movement){
        case U: velocity = {0, 1}; break;
        case L: velocity = {-1, 0}; break;
        case R: velocity = {1, 0}; break;
        case D: velocity = {0, -1}; break;
        case UL: velocity = {-1, 1}; break;
        case UR: velocity = {1, 1}; break;
        case DL: velocity = {-1, -1}; break;
        case DR: velocity = {1, -1}; break;
        default: break;
    }

    model->getState().velocity = velocity;
}


void Player::collisionHandler(Player const &other){
}

void Player::collisionHandler(GameEntities::Bullet const &other){
    hp -= other.getDamage();
}

void Player::eventHandler(const ClientServer::InteractEvent &ev){
    // пока что void realization
}

// Bullet methods definition
Bullet::Bullet(Bullet &&other) : GameObject(std::move(other)) {
    damage = other.damage;
}

Bullet &Bullet::operator=(Bullet &&other) {
    damage = other.damage;
    GameObject::operator=(std::move(other));
}

value Bullet::serialize() {
    value jv = {
        {"hp", damage},
        {"GameObject", GameObject::serialize()}
    };
    return jv;
}

void Bullet::collisionHandler(Player const &other){
    deleted = true;
}

void Bullet::collisionHandler(GameEntities::Bullet const &other){
}

void Bullet::deserialize(value jv) {
    object const &obj = jv.as_object();
    extract(obj, damage, "damage");

    extract(obj, jv, "GameObject");
    GameObject::deserialize(jv);
}

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
