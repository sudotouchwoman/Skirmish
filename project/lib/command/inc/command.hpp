#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <SDL.h>
#include <iostream>
#include <memory>
#include <cmath>

#include "PlayerEvent.h"
#include "GameObjects.h"
#include "GlobalEnvironment.h"
#include "ConnectionClient.h"

class EventManager {
public:
    bool HandleEvents(Client::ConnectionClient& cc, GameEntities::GlobalEnvironment& ge);
    explicit EventManager(SDL_Texture* texture, size_t player_id);
private:
    bool quit;
    size_t player_id;
    ClientServer::MoveEvent moveEvent{};
    ClientServer::ShootEvent shootEvent{};
    ClientServer::RotateEvent rotateEvent{};
    SDL_Event gameEvent{};
    int x, y, width, height;
    float angle;

    void GetCursorPosition();
};

#endif //COMMAND_HPP