#pragma once

#include "GameObjects.h"

#include <vector>
#include <mutex>

class GlobalEnvironment{
public:
    GlobalEnvironment() = default;
    ~GlobalEnvironment() = default;
    GlobalEnvironment(GlobalEnvironment &&);
    GlobalEnvironment(const GlobalEnvironment &);
    GlobalEnvironment & operator=(const GlobalEnvironment &);
    GlobalEnvironment & operator=(GlobalEnvironment &&);

    void updateEnvironment();
    int getAccess();
    int finishAccess();
    std::vector<GameEntities::GameObject> &getModifyGameObjects() { return _game_objects; };

private:
    std::mutex _mutex;
    std::string _snapshot;
    std::vector<GameEntities::GameObject> _game_objects;
};