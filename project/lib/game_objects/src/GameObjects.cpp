#include "GameObjects.h"
#include "physical.hpp"
#include "Extract.h"

using namespace GameEntities;
using namespace boost::json;



// initialization of static member
size_t GameEntities::GameObject::global_id = 0;

void GameObject::deserialize(value jv) {
    core::vec2 shift;
    object ob = jv.as_object();
    extract(ob, type, "type");
    extract(ob, id, "id");

    float x, y;
    extract(ob, ob, "Physical");
    extract(ob, x, "geometry_x");
    extract(ob, y, "geometry_y");

    setX(x);
    setY(y);
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

void GameObject::setDefaultGeometry(const double x, const double y, const double R) {
    if (not model) throw std::runtime_error("empty model");
    auto geometry = physical::IShapeUPtr(new core::Circle(x, y, R));
    model->setGeometry(std::move(geometry));
}

void GameObject::setDefaultModel(const double vx, const double vy, const double inverse_mass) {
    model = std::make_unique<physical::PhysicalObject>(inverse_mass);
    model->getState().velocity = { vx, vy };
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
}

void Player::deserialize(value jv) {
    object const &obj = jv.as_object();
    extract(obj, hp, "hp");
    extract(obj, name, "name");

    extract(obj, jv, "GameObject");
    GameObject::deserialize(jv);
}

Player::Player(const double x,
               const double y,
               const double R,
               const double vx,
               const double vy,
               const double inverse_mass) : GameObject(ObjectTypes::tPlayer), hp(100) {
    setDefaultModel(vx, vy, inverse_mass);
    setDefaultGeometry(x, y, R);
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
        case Void: velocity = {0, 0}; break;
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
        {"damage", damage},
        {"GameObject", GameObject::serialize()}
    };
    return jv;
}

Bullet::Bullet(const double x,
       const double y,
       const double R,
       const double vx,
       const double vy,
       const double inverse_mass) : GameObject(ObjectTypes::tBullet), damage(10) {
    setDefaultModel(vx, vy, inverse_mass);
    setDefaultGeometry(x, y, R);
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
