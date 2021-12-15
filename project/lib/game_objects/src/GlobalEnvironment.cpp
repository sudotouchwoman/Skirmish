#include "GlobalEnvironment.h"
#include "GameSettings.h"
#include "Extract.h"

using namespace boost::json;

namespace GameEntities {
    int GlobalEnvironment::getAccess() {
        mutex_.lock();
        return 0;
    }

    int GlobalEnvironment::finishAccess() {
        mutex_.unlock();
        return 0;
    }

    void GlobalEnvironment::tick() {
        getAccess();
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
                        collision_details =
                        physical::PhysicalObject::collide(Vector1[i].getModel(), Vector2[j].getModel());
                    if (!collision_details) continue;

                    onCollision(Vector1[i], Vector2[j], collision_details);
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

                    onCollision(elem1, elem2, collision_details);
                }
            }
        };

        collisionMaker(Players, Players);
        collisionFullMaker(Players, Bullets);

        finishAccess();

        deleteObjects();
    }

    template<class T1, class T2>
    int GlobalEnvironment::onCollision(T1 &go1, T2 &go2, core::ContactPoint &cp) {
        getAccess();

        // probably some game logic, moving objects
        // like player to player collision need to be
        // two players get back.

        go1.collisionHandler(go2);
        go2.collisionHandler(go1);

        finishAccess();

        return 0;
    }

    int GlobalEnvironment::deleteObjects() {
        getAccess();

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

        finishAccess();
    }

//int GlobalEnvironment::onCollision(Collision cl){
//    // game logic depending on type
//
//    // calling methods on collision of two passed links of elements
//    return 0;
//}

    size_t GlobalEnvironment::addPlayer() {
        getAccess();

        Players.emplace_back(Player());

        size_t id = Players[Players.size() - 1].getID();
        finishAccess();
        return id;
    }

    int GlobalEnvironment::generateSnapshot() {
        getAccess();

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

        snapshot_ = serialize(jv);
        finishAccess();
        return 0;
    }

    std::string GlobalEnvironment::getSnapshot() {
        return snapshot_;
    }

    void GlobalEnvironment::handleServerResponse(std::string &&server_response) {
        // get acccess not emplement couse atomic
        setSnapshot(std::move(server_response));
        getObjectsFromSnapshot();
    }

    int GlobalEnvironment::setSnapshot(std::string &&new_snapshot) {
        getAccess();

        snapshot_ = new_snapshot;
        finishAccess();
        return 0;
    }

    int GlobalEnvironment::getObjectsFromSnapshot() {
        object objects = parse(snapshot_).as_object();
        array bullets, players;
        extract(objects, players, "Players");
        extract(objects, bullets, "Bullets");

        getAccess();
        Bullets.resize(bullets.size());
        Players.resize(players.size());

        for (size_t i = 0; i < Players.size(); ++i)
            Players[i].deserialize(players[i]);
        for (size_t i = 0; i < Bullets.size(); ++i)
            Bullets[i].deserialize(bullets[i]);
        finishAccess();
        return 0;
    }

    int GlobalEnvironment::onEvent(size_t player_id, const ClientServer::MoveEvent &me) {
        /// change position of player
        getAccess();

        // find player by id
        auto it = Players.begin();
        for (; it != Players.end() && it->getID() != player_id; ++it);

        // change it geometry
        it->eventHandler(me);

        finishAccess();
        return 0;
    }

    int GlobalEnvironment::onEvent(size_t player_id, const ClientServer::ShootEvent &se) {
        /// bullet object creating
        getAccess();

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

        finishAccess();
        return 0;
    }

    int GlobalEnvironment::onEvent(size_t player_id, const ClientServer::InteractEvent &ie) {
        //do some logic
    }
} // namespace GameEntities