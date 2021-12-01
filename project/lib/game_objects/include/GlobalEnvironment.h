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

        void updateEnvironment();
        int getAccess();
        int finishAccess();
        std::vector<std::shared_ptr<GameEntities::GameObject>> &getModifyGameObjects();

        int generateSnapshot();
        const std::string &getSnapshot();
        int setSnapshot(std::string &&);
        int getObjectsFromSnapshot();
    private:
        std::mutex _mutex;
        std::string _snapshot;
        std::vector<std::shared_ptr<GameEntities::GameObject>> _game_objects;
    };
}
