#pragma once

#include <boost/json/src.hpp>
#include "plug.h"
#include <vector>
#include <mutex>

class GlobalEnvironment{
public:
    void updateEnvironment();
    void getSnapshot();
    int getAccess();
    int finishAccess();
private:
    std::mutex _mutex;
    std::vector<GameObject> _game_objects;
};