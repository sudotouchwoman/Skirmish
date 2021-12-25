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
        int onCollision(Player &go1, Bullet &go2, core::ContactPoint & cp);
        int onCollision(Bullet &go1, Terrain &go2, core::ContactPoint & cp);
        int onCollision(Bullet &go1, Bullet &go2, core::ContactPoint & cp);
        int onCollision(Player &go1, Player &go2, core::ContactPoint & cp);
        int onCollision(Player &go1, Terrain &go2, core::ContactPoint & cp);


        // Server side methods
        void onEvent(const size_t player_id, const ClientServer::MoveEvent &);
        void onEvent(const size_t player_id, const ClientServer::ShootEvent &);
        void onEvent(const size_t player_id, const ClientServer::RotateEvent &);
        void onEvent(const size_t player_id, const ClientServer::InteractEvent &);

        int deleteObjects();
        size_t addPlayer(const ClientServer::RegisterEvent &event);

        void addObstacle(const float x, const float y, const float w, const float h);

        int generateSnapshot();
        void getSnapshot(std::string &);
        void tick();

        // Client side methods
        void handleServerResponse(std::string &&);

        Player &getPlayerById(size_t id) const;
        auto &getPlayers() const {return players;};
        auto &getBullets() const {return bullets;};
        auto &getTerrain() const {return terrain;};
        auto &getMessage() const {return messages;}

        bool gameEnd() const {return num_of_alive == 1 && players.size() != 1; };
        void finishGame() {
            getAccess();
            players.erase(players.begin(), players.end());
            bullets.erase(bullets.begin(), bullets.end());
            terrain.erase(terrain.begin(), terrain.end());
            messages.erase(messages.begin(), messages.end());
            messages.emplace_back("Game End", 1);
            finishAccess();
        }
    private:
        int setSnapshot(std::string &&);
        int getObjectsFromSnapshot();

        std::mutex mutex_;
        std::string snapshot_;

        std::unordered_map <size_t,  std::vector<GameEntities::Player>::iterator> id_to_players;
        std::vector <GameEntities::Player> players;
        std::vector <GameEntities::Bullet> bullets;
        std::vector <GameEntities::Terrain> terrain;
        std::vector <GameEntities::Message> messages;

        // gameover state
        int num_of_alive = 0;
//        std::vector <GameEntities::Object> Objects;
    };
}
