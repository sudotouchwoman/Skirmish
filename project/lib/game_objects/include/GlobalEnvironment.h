#pragma once

#include "GameObjects.h"

#include <vector>
#include <mutex>

namespace GameEntities{
    class GlobalEnvironment{
    public:
        GlobalEnvironment() = default;
        ~GlobalEnvironment() = default;
        GlobalEnvironment(GlobalEnvironment &&) = delete;
        GlobalEnvironment(const GlobalEnvironment &) = delete;
        GlobalEnvironment & operator=(const GlobalEnvironment &) = delete;
        GlobalEnvironment & operator=(GlobalEnvironment &&) = delete;

        int getAccess();
        int finishAccess();
//        int onCollision(Collision);

        int onEvent(const ClientServer::MoveEvent &);
        int onEvent(const ClientServer::ShootEvent &);
        int onEvent(const ClientServer::InteractEvent &);

        int deleteObjects();
        int addPlayer(GameEntities::Player & pl);

        int generateSnapshot();
        const std::string &getSnapshot();
        int setSnapshot(std::string &&);
        int getObjectsFromSnapshot();


    private:
        std::mutex _mutex;
        std::string _snapshot;

        std::vector <GameEntities::Player> Players;
        std::vector <GameEntities::Bullet> Bullets;
        std::vector <GameEntities::Terrain> Terrain;
        std::vector <GameEntities::Object> Objects;
    };
}
