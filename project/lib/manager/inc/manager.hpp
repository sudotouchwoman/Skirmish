#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <memory>

#include "GameObjects.h"
#include "GlobalEnvironment.h"
#include "ConnectionClient.h"

#include "fps.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "command.hpp"
#include "menu.hpp"

class GameManager {
public:
    void Run(size_t port_client);
    GameManager() : frames(60), inverseTimeSeconds(1) {}

    GameManager(const GameManager &other) = delete;
    GameManager &operator=(const GameManager &other) = delete;
private:
    Window window;
    FPS fps;
    const int frames;
    int inverseTimeSeconds;
};

#endif //MANAGER_HPP