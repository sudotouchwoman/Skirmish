#pragma once

#include "GameObjects.h"

#include <vector>
#include <mutex>
#include <unordered_map>

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

        // collision manager for game logic
        template <class T1, class T2>
        int onCollision(T1 &go, T2 &go2, core::ContactPoint & cp);

        int onEvent(const size_t player_id, const ClientServer::MoveEvent &);
        int onEvent(const size_t player_id, const ClientServer::ShootEvent &);
        int onEvent(const size_t player_id, const ClientServer::InteractEvent &);

        int deleteObjects();
        int addPlayer(GameEntities::Player & pl);

        int generateSnapshot();
        std::string getSnapshot();
        void handleServerResponse(std::string &&);

        void tick();
    private:
        int setSnapshot(std::string &&);
        int getObjectsFromSnapshot();

        std::mutex mutex_;
        std::string snapshot_;

        std::unordered_map <size_t,  std::vector<GameEntities::Player>::iterator> id_to_players;
        std::vector <GameEntities::Player> Players;
        std::vector <GameEntities::Bullet> Bullets;
//        std::vector <GameEntities::Terrain> Terrain;
//        std::vector <GameEntities::Object> Objects;
    };
}
