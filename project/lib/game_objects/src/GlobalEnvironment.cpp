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

    void GlobalEnvironment::addObstacle(const float x_top_left, const float y_top_left,
                                        const float x_bottom_right, const float y_bottom_right){

        terrain.emplace_back((x_bottom_right - x_top_left) / 2  + x_top_left,
                             (y_bottom_right - y_top_left) / 2  + y_top_left,
                             (x_bottom_right - x_top_left),
                             (y_bottom_right - y_top_left));
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

        updater(players);
        updater(bullets);
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

        collisionMaker(players, players);
        collisionFullMaker(players, bullets);
        collisionFullMaker(players, terrain);
        collisionFullMaker(bullets, terrain);

        // delete bullets outside range
        for (auto & bullet : bullets){
            const auto &center = bullet.getModel().getGeometry().GetCenter();
            if (center.x < 0 || center.y < 0 || center.x >= map_size || center.y >= map_size)
                bullet.deleted = true;
        }

        // tick messages
        for (auto iter = messages.begin(); iter < messages.end(); ++iter)
            if (!iter->tick())
                messages.erase(iter);

        finishAccess();

        deleteObjects();
    }

    int GlobalEnvironment::onCollision(Player &go1, Bullet &go2,  core::ContactPoint & cp){
        go1.collisionHandler(go2);
        go2.collisionHandler(go1);

        if (go1.isDead()){
            std::string message;
            message += getPlayerById(go2.getIdOwner()).getName() + " DRASTICLY MURDERED " + go1.getName() + "!!!!";
            messages.emplace_back(message, message_time_to_live);
        }
        return 0;
    }
    int GlobalEnvironment::onCollision(Bullet &go1, Terrain &go2, core::ContactPoint & cp){
        go1.collisionHandler(go2);
        go2.collisionHandler(go1);

        return 0;
    }
    int GlobalEnvironment::onCollision(Bullet &go1, Bullet &go2, core::ContactPoint & cp){
        go1.collisionHandler(go2);
        go2.collisionHandler(go1);

        return 0;
    }
    int GlobalEnvironment::onCollision(Player &go1, Player &go2, core::ContactPoint & cp){
        go1.collisionHandler(go2);
        go2.collisionHandler(go1);

        return 0;
    }
    int GlobalEnvironment::onCollision(Player &go1, Terrain &go2, core::ContactPoint & cp){
        go1.collisionHandler(go2);
        go2.collisionHandler(go1);

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

        deleter(players);
        deleter(bullets);
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

    size_t GlobalEnvironment::addPlayer(const ClientServer::RegisterEvent &event) {
        getAccess();

        players.emplace_back(default_player_hp);

        size_t id = players[players.size() - 1].getID();

        players[players.size() - 1].setVanity(event);

        // redesign unordered hashmap
        for (auto it = players.begin(); it < players.end(); ++it)
            id_to_players[it->getID()] = it;

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
            {"Players", vectorSerializer(players)},
            {"Bullets", vectorSerializer(bullets)},
            {"Terrain", vectorSerializer(terrain)},
            {"Messages", vectorSerializer(messages)},
        };

        snapshot_ = serialize(jv);
        finishAccess();
        return 0;
    }

    void GlobalEnvironment::getSnapshot(std::string &ret) {
        getAccess();
        ret = snapshot_;
        finishAccess();
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
        array bullets_, players_, terrain_, messages_;
        extract(objects, players_, "Players");
        extract(objects, bullets_, "Bullets");
        extract(objects, terrain_, "Terrain");
        extract(objects, messages_, "Messages");

        getAccess();
        bullets.resize(bullets_.size());
        players.resize(players_.size());
        terrain.resize(terrain_.size());
        messages.resize(messages_.size());

        for (size_t i = 0; i < players.size(); ++i){
            players[i].deserialize(players_[i]);
            id_to_players[players[i].getID()] = players.begin() + i;
        }
        for (size_t i = 0; i < bullets.size(); ++i)
            bullets[i].deserialize(bullets_[i]);
        finishAccess();
        for (size_t i = 0; i < messages.size(); ++i)
            messages[i].deserialize(messages_[i]);
        finishAccess();
        for (size_t i = 0; i < terrain.size(); ++i)
            terrain[i].deserialize(terrain_[i]);
        finishAccess();
        return 0;
    }

    void GlobalEnvironment::onEvent(size_t player_id, const ClientServer::MoveEvent &me) {
        /// change position of player
        getAccess();

        // find player by id
        auto &player = getPlayerById(player_id);

        // change it geometry
        player.eventHandler(me);

        finishAccess();
    }

    void GlobalEnvironment::onEvent(const size_t player_id, const ClientServer::RotateEvent &ev){
        getAccess();

        // find player who school shooting
        auto &player = getPlayerById(player_id);

        player.angle_ = ev.angle;
        finishAccess();
    }

    void GlobalEnvironment::onEvent(size_t player_id, const ClientServer::ShootEvent &se) {
        /// bullet object creating
        getAccess();

        // find player who school shooting
        auto &player = getPlayerById(player_id);

        // model methods need to be in class constructor.
        GameEntities::Bullet bullet(default_bullet_damage, player_id, 2 /*times ricochet*/ );
        auto &model = bullet.getModel();
        model.getState().velocity = {se.x * bullet_speed * physics_tick, se.y * bullet_speed * physics_tick};
        auto shift_player = player.getModel().getGeometry().GetCenter() +
            core::vec2{se.x * (default_player_radius + default_bullet_radius),
                       se.y * (default_player_radius + default_bullet_radius)};

        model.getGeometry().setCenter(shift_player);
        bullet.texture_id = player.texture_id;

        bullets.emplace_back(std::move(bullet));

        finishAccess();
    }

    Player &GlobalEnvironment::getPlayerById(size_t id) const{
        auto it = id_to_players.find(id);
        if (it != id_to_players.end())
            return *(it->second);
        else
            throw std::runtime_error("Player with this id does not exist!");
    }

    void GlobalEnvironment::onEvent(size_t player_id, const ClientServer::InteractEvent &ie) {
        //do some logic
    }
} // namespace GameEntities