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
        int onCollision(Collision);

        int onEvent(const ClientServer::MoveEvent &);
        int onEvent(const ClientServer::ShootEvent &);
        int onEvent(const ClientServer::InteractEvent &);

//        std::vector<std::shared_ptr<GameEntities::GameObject>> &getModifyGameObjects();

        int deleteObjects();

        int generateSnapshot();
        const std::string &getSnapshot();
        int setSnapshot(std::string &&);
        int getObjectsFromSnapshot();


    private:
        std::mutex _mutex;
        std::string _snapshot;
//        std::vector<std::shared_ptr<GameEntities::GameObject>> _game_objects;

        std::vector <GameEntities::Player> Players;
        std::vector <GameEntities::Bullet> Bullet;
        std::vector <GameEntities::Terrain> Terrain;
        std::vector <GameEntities::Object> Object;
    };
}
