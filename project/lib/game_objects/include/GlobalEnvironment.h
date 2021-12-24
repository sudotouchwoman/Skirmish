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

        // atomic functions for managing multithreading
        int getAccess();
        int finishAccess();

        // collision manager for game logic
        template <class T1, class T2>
        int onCollision(T1 &go, T2 &go2, core::ContactPoint & cp);

        // Server side methods
        void onEvent(const size_t player_id, const ClientServer::MoveEvent &);
        void onEvent(const size_t player_id, const ClientServer::ShootEvent &);
        void onEvent(const size_t player_id, const ClientServer::RotateEvent &);
        void onEvent(const size_t player_id, const ClientServer::InteractEvent &);

        int deleteObjects();
        size_t addPlayer(const ClientServer::RegisterEvent &event);

        void addObstacle(Terrain &&t);

        int generateSnapshot();
        void getSnapshot(std::string &);
        void tick();

        // Client side methods
        void handleServerResponse(std::string &&);

        Player &getPlayerById(size_t id) const;
        auto &getPlayers() const {return players;};
        auto &getBullets() const {return bullets;};
        auto &getTerrain() const {return terrain;};
    private:
        int setSnapshot(std::string &&);
        int getObjectsFromSnapshot();

        std::mutex mutex_;
        std::string snapshot_;

        std::unordered_map <size_t,  std::vector<GameEntities::Player>::iterator> id_to_players;
        std::vector <GameEntities::Player> players;
        std::vector <GameEntities::Bullet> bullets;
        std::vector <GameEntities::Terrain> terrain;
//        std::vector <GameEntities::Object> Objects;
    };
}
