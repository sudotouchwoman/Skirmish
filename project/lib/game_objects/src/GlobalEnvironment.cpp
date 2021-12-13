#include "GlobalEnvironment.h"
#include "GameSettings.h"
#include "Extract.h"

using namespace boost::json;
using GameEntities::GlobalEnvironment;

int GlobalEnvironment::getAccess() {
    _mutex.lock();
    return 0;
}

int GlobalEnvironment::finishAccess() {
    _mutex.unlock();
    return 0;
}

void GlobalEnvironment::tick() {
    // lambda for elements deletion
    auto updater = [](auto &Vector) {
        for (auto &elem: Vector) {
            if (elem.hasModel()) {
                elem.getModel().update(physics_tick);
            }
        }
    };

    updater(Players);
    updater(Bullets);
//    updater(Terrain);
//    updater(Objects);

    auto collisionMaker = [this](auto &Vector1, auto &Vector2) {
        for (size_t i = 0; i < Vector1.size(); ++i) {
            if (!Vector1[i].hasModel()) continue;
            for (size_t j = i; j < Vector2.size(); ++j) {
                if (!Vector2[j].hasModel()) continue;
                auto
                    collision_details = physical::PhysicalObject::collide(Vector1[i].getModel(), Vector2[j].getModel());
                if (!collision_details) continue;

                this->onCollision(Vector1[i], Vector2[j], collision_details);
            }
        }
    };

    auto collisionFullMaker = [this](auto &Vector1, auto &Vector2) {
        for (auto &elem1: Vector1) {
            if (!elem1.hasModel()) continue;
            for (auto &elem2: Vector2) {
                if (!elem2.hasModel()) continue;
                auto collision_details = physical::PhysicalObject::collide(elem1.getModel(), elem2.getModel());
                if (!collision_details) continue;

                this->onCollision(elem1, elem2, collision_details);
            }
        }
    };

    collisionMaker(Players, Players);
    collisionFullMaker(Players, Bullets);
}

template<class T1, class T2>
int GlobalEnvironment::onCollision(T1 &go1, T2 &go2, core::ContactPoint &cp) {
    // probably some game logic, moving objects
    // like player to player collision need to be
    // two players get back.

    go1.collisionHandler(go2);
    go2.collisionHandler(go1);
    return 0;
}

int GlobalEnvironment::deleteObjects() {

    // lambda for elements deletion
    auto deleter = [](auto &Vector) {
        for (auto &elem: Vector) {
            if (elem.deleted) {
                elem = std::move(Vector[Vector.size() - 1]);
                Vector.pop_back();
            }
        }
    };

    deleter(Players);
    deleter(Bullets);
//    deleter(Terrain);
//    deleter(Objects);
}

//int GlobalEnvironment::onCollision(Collision cl){
//    // game logic depending on type
//
//    // calling methods on collision of two passed links of elements
//    return 0;
//}

int GlobalEnvironment::addPlayer(GameEntities::Player &pl) {
    Players.emplace_back(std::move(pl));
    return 0;
}

int GlobalEnvironment::generateSnapshot() {
    /// not effective realization!
    auto vectorSerializer = [](auto &Vector) {
        array arr;
        for (auto &elem: Vector) {
            arr.push_back(elem.serialize());
        }
        return std::move(arr);
    };

    value jv = {
        {"Players", vectorSerializer(Players)},
        {"Bullets", vectorSerializer(Bullets)},
//        {"Terrain", vectorSerializer(Players)},
//        {"Objects", vectorSerializer(Players)},
    };

    _snapshot = serialize(jv);
    return 0;
}

std::string GlobalEnvironment::getSnapshot() const{
    return _snapshot;
}

void GlobalEnvironment::handleServerResponse(std::string &&server_response){
    setSnapshot(std::move(server_response));
    getObjectsFromSnapshot();
}

int GlobalEnvironment::setSnapshot(std::string &&new_snapshot) {
    _snapshot = new_snapshot;
    return 0;
}

int GlobalEnvironment::getObjectsFromSnapshot() {
    object objects = parse(_snapshot).as_object();
    array bullets, players;
    extract(objects, players, "Players");
    extract(objects, bullets, "Bullets");
    Bullets.resize(bullets.size());
    Players.resize(players.size());

    for (size_t i = 0; i < Players.size(); ++i)
        Players[i].deserialize(players[i]);
    for (size_t i = 0; i < Bullets.size(); ++i)
        Bullets[i].deserialize(bullets[i]);
    return 0;
}

int GlobalEnvironment::onEvent(size_t player_id, const ClientServer::MoveEvent &me) {
    /// change position of player

    // find player by id
    auto it = Players.begin();
    for (; it != Players.end() && it->getID() != player_id; ++it);

    // change it geometry
    it->eventHandler(me);
    return 0;
}

int GlobalEnvironment::onEvent(size_t player_id, const ClientServer::ShootEvent &se) {
    /// bullet object creating

    // find player who school shooting
    auto it = Players.begin();
    for (; it != Players.end() && it->getID() != player_id; ++it);

    // model methods need to be in class constructor.
    GameEntities::Bullet bullet;
    auto &model = bullet.getModel();
    model.getState().velocity = {se.x * bullet_speed, se.y * bullet_speed};
    auto shift_player = it->getModel().getGeometry().GetCenter() +
        core::vec2{se.x * (default_player_radius + default_bullet_radius),
                   se.y * (default_player_radius + default_bullet_radius)};

    model.getGeometry().shift(shift_player);

    Bullets.emplace_back(std::move(bullet));
    return 0;
}

int GlobalEnvironment::onEvent(size_t player_id, const ClientServer::InteractEvent &ie) {
    //do some logic
}