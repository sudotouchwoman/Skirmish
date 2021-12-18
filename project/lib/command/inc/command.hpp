#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <SDL.h>
#include <iostream>
#include <memory>

#include "PlayerEvent.h"
#include "ConnectionClient.h"

class EventManager {
public:
    bool HandleEvents(Client::ConnectionClient& connectionClient);
    explicit EventManager(SDL_Texture* texture);
private:
    bool quit;
    ClientServer::MoveEvent moveEvent{};
    ClientServer::ShootEvent shootEvent{};
    ClientServer::RotateEvent rotateEvent{};
    SDL_Event gameEvent{};
    int x, y, width, height;

    void GetCursorPosition();
};

#endif //COMMAND_HPP