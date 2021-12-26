#include "GameObjects.h"
#include "physical.hpp"
#include "GameSettings.h"
#include "Extract.h"

using namespace boost::json;

namespace GameEntities {
// initialization of static member
    size_t GameEntities::GameObject::global_id = 0;

    IRenderable::IRenderable(IRenderable &&other) {
        x = other.x;
        y = other.y;
        angle = other.angle;
        texture_id = other.texture_id;
    }

    IRenderable &IRenderable::operator=(IRenderable &&other) {
        x = other.x;
        y = other.y;
        angle = other.angle;
        texture_id = other.texture_id;
    }

    void GameObject::deserialize(value jv) {
        object ob = jv.as_object();
        int type;
        extract(ob, type, "type");
        type_ = static_cast<ObjectTypes>(type);

        extract(ob, id, "id");

        float x, y;
        size_t texture_id;
        float angle;
        extract(ob, ob, "Physical");
        extract(ob, x, "x");
        extract(ob, y, "y");
        extract(ob, texture_id, "t_id");
        extract(ob, angle, "angle");

        setX(x);
        setY(y);
        setAngle(angle);
        setTextureId(texture_id);
    }

    GameObject &GameObject::operator=(GameObject &&other) {
        // may cause memory leak try:
        // delete model;
        model = std::move(other.model);
        other.model = nullptr;
        deleted = other.deleted;
        id = other.id;
        type_ = other.type_;
        IRenderable::operator=(std::move(other));
        return *this;
    }

    GameObject::GameObject(GameObject &&other): IRenderable(std::move(other)) {
        model = std::move(other.model);
        other.model = nullptr;
        id = other.id;
        type_ = other.type_;
    }

    void GameObject::setDefaultGeometry(const double x, const double y, const double R) {
        if (not model) throw std::runtime_error("empty model");
        auto geometry = physical::IShapeUPtr(new core::Circle(x, y, R));
        model->setGeometry(std::move(geometry));
    }

    void GameObject::setDefaultModel(const double vx, const double vy, const double inverse_mass) {
        model = std::make_unique<physical::PhysicalObject>(inverse_mass);
        model->getState().velocity = {vx, vy};
    }

    value GameObject::serialize() {
        const auto &geometry = model->getGeometry().GetCenter();

        value jv = {
            {"type", static_cast<int>(type_)},
            {"id", id},
            {"Physical", {
                {"x", geometry.x},
                {"y", geometry.y},
                {"t_id", getTextureId()},
                {"angle", getAngle()},
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

    Player::Player(Player &&other) : GameObject(std::move(other)) {
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

    void Player::setVanity(const ClientServer::RegisterEvent &event) {
        name = event.name;
        if (event.id_texture <= textures_num && event.id_texture >= 0)
            setTextureId(event.id_texture);
        else
            setTextureId(1);
    }

    Player::Player(const double x,
                   const double y,
                   const double R,
                   const double inverse_mass) : GameObject(ObjectTypes::T_PLAYER), hp(100) {
        setDefaultModel(default_spawn_vx, default_spawn_vy, inverse_mass);
        setDefaultGeometry(x, y, R);
    }

    void Player::eventHandler(const ClientServer::MoveEvent &ev) {
        using namespace ClientServer;
        core::vec2 velocity;
        float side_velocity = player_speed * physics_tick;
        switch (ev.movement) {
            case U: velocity = {0, -side_velocity}; break;
            case L: velocity = {-side_velocity, 0}; break;
            case R: velocity = {side_velocity, 0}; break;
            case D: velocity = {0, side_velocity}; break;
            case UL: velocity = {-side_velocity, -side_velocity}; break;
            case UR: velocity = {side_velocity, -side_velocity}; break;
            case DL: velocity = {-side_velocity, side_velocity}; break;
            case DR: velocity = {side_velocity, side_velocity}; break;
            case Void: velocity = {0, 0}; break;
            default: break;
        }

        model->getState().velocity = velocity;
    }

    void Player::collisionHandler(Player const &other) {

    }

    void Player::collisionHandler(GameEntities::Bullet const &other) {
        hp -= other.getDamage();
    }

    void Player::eventHandler(const ClientServer::InteractEvent &ev) {
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
                   const double inverse_mass) : GameObject(ObjectTypes::T_BULLET), damage(10) {
        setDefaultModel(default_spawn_vx, default_spawn_vy, inverse_mass);
        setDefaultGeometry(x, y, R);
    }

    void Bullet::collisionHandler(Player const &other) {
        deleted = true;
    }

    void Bullet::collisionHandler(GameEntities::Bullet const &other) {
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

} // namespace GameEntities