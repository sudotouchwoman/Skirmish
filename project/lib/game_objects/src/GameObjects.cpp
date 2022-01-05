#include "GameObjects.h"
#include "physical.hpp"
#include "GameSettings.h"
#include "Extract.h"

using namespace boost::json;

namespace GameEntities {
// initialization of static member
    size_t GameObject::global_id = 0;

    void GameObject::deserialize(value jv) {
        object ob = jv.as_object();
        int type;
        extract(ob, type, "type");
        type_ = static_cast<ObjectTypes>(type);

        extract(ob, id, "id");

        extract(ob, ob, "Physical");
        extract(ob, x_, "x");
        extract(ob, y_, "y");
        extract(ob, texture_id, "t_id");
        extract(ob, angle_, "angle");
        extract(ob, w_, "width");
        extract(ob, h_, "height");
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

    void GameObject::setDefaultGeometry(const double x, const double y, const double w, const double h) {
        if (not model) throw std::runtime_error("empty model");
        auto geometry = physical::IShapeUPtr(new core::AABB(x, y, w, h));
        model->setGeometry(std::move(geometry));
    }

    value GameObject::serialize() {
        const auto &geometry = model->getGeometry().GetCenter();

        value jv = {
            {"type", static_cast<int>(type_)},
            {"id", id},
            {"Physical", {
                {"x", geometry.x},
                {"y", geometry.y},
                {"t_id", texture_id},
                {"angle", angle_},
                {"width", model->getGeometry().getBoundingRect().getWidth()},
                {"height", model->getGeometry().getBoundingRect().getHeight()},
            }}
        };
        return jv;
    }

// Players #############################################################################################################

    value Player::serialize() {
        value jv = {
            {"hp", hp},
            {"name", name},
            {"dead", dead},
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
        extract(obj, dead, "dead");

        extract(obj, jv, "GameObject");
        GameObject::deserialize(jv);
    }

    void Player::setVanity(const ClientServer::RegisterEvent &event) {
        name = event.name;
        if (event.id_texture <= textures_num && event.id_texture >= 0)
            texture_id = event.id_texture;
        else
            texture_id = 1;
    }

    Player::Player(const int hp_,
                   ObjectTypes type_,
                   const double x,
                   const double y,
                   const double R,
                   const double inverse_mass) : GameObject(type_), hp(hp_) {
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

    void Player::collisionHandler(Terrain const &other) {
    }

    void Player::collisionHandler(GameEntities::Bullet const &other) {
        hp -= other.getDamage();
        if (hp <= 0)
            dead = true;
    }

    void Player::eventHandler(const ClientServer::InteractEvent &ev) {
        // пока что void realization
    }

// Bullet ##########################################################ж###################################################

    Bullet::Bullet(Bullet &&other) : GameObject(std::move(other)) {
        damage = other.damage;
        ttl = other.ttl;
        id_owner = other.id_owner;
    }

    Bullet &Bullet::operator=(Bullet &&other) {
        damage = other.damage;
        ttl = other.ttl;
        id_owner = other.id_owner;
        GameObject::operator=(std::move(other));
        return *this;
    }

    value Bullet::serialize() {
        value jv = {
            {"damage", damage},
            {"GameObject", GameObject::serialize()}
        };
        return jv;
    }

    Bullet::Bullet(int damage_, size_t owner, int ttl_, ObjectTypes type_,
    const double inverse_mass,
    const double x,
    const double y,
    const double R) : damage(damage_), id_owner(owner), ttl(ttl_), GameObject(type_) {
        setDefaultModel(default_spawn_vx, default_spawn_vy, inverse_mass);
        setDefaultGeometry(x, y, R);
    }

    void Bullet::collisionHandler(Player const &other) {
        deleted = true;
    }

    void Bullet::collisionHandler(Terrain const &other) {
        if ((ttl -= 1) == 0)
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


// Terrain #############################################################################################################

    Terrain::Terrain(
        const double x,
        const double y,
        const double w,
        const double h,
        ObjectTypes type_,
        const double inverse_mass) : GameObject(type_) {
        setDefaultModel(default_spawn_vx, default_spawn_vy, inverse_mass);
        setDefaultGeometry(x, y, w, h);
    }

    Terrain::Terrain(Terrain &&other): GameObject(std::move(other)){}

    Terrain & Terrain::operator=(Terrain &&other) {
        GameObject::operator=(std::move(other));
        return *this;
    }

    value Terrain::serialize() {
        value jv = {
            {"GameObject", GameObject::serialize()}
        };
        return jv;
    }

    void Terrain::deserialize(value jv) {
        object const &obj = jv.as_object();

        extract(obj, jv, "GameObject");
        GameObject::deserialize(jv);
    }

    void Terrain::collisionHandler(const Player &other) {}

    void Terrain::collisionHandler(const Terrain &other) {}

    void Terrain::collisionHandler(const GameEntities::Bullet &other) {}

// Message #############################################################################################################

    value Message::serialize(){
        value jv = {
            {"message", message}
        };
        return jv;
    }

    void Message::deserialize(value jv){
        object const &obj = jv.as_object();
        extract(obj, message, "message");
    }
} // namespace GameEntities