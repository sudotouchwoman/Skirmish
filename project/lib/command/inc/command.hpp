#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <SDL.h>
#include <iostream>
#include <memory>

#include "window.hpp"
#include "PlayerEvent.h"
#include "GameObjects.h"
#include "GlobalEnvironment.h"
#include "ConnectionClient.h"

class EventManager {
public:
    bool HandleEvents(Client::ConnectionClient& cc, GameEntities::GlobalEnvironment& ge);
    explicit EventManager(Window* window);
private:
    Window* window;
    bool quit;
    ClientServer::MoveEvent moveEvent{};
    ClientServer::ShootEvent shootEvent{};
    ClientServer::RotateEvent rotateEvent{};
    SDL_Event gameEvent{};
    int x, y, width, height;
    float angle, vector_x, vector_y;
    bool buttons[SDL_NUM_SCANCODES] {};

    void GetCursorPosition();
    void NormalizeVector(float x_, float y_);
};

#endif //COMMAND_HPP