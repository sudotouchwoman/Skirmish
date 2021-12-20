#include "command.hpp"

EventManager::EventManager(Window* window) :
    quit(false), x(0), y(0), width(0), height(0), angle(0), vector_x(0), vector_y(0) {
    this->window = window;
    SDL_QueryTexture(window->imageList[Tile::GAME_CURSOR], nullptr, nullptr, &width, &height);
}

void EventManager::GetCursorPosition() {
    x += width / 2;
    y += height / 2;
}

void EventManager::NormalizeVector(float x_, float y_) {
    auto vector_size = (float) std::sqrt(std::pow(x_, 2) + std::pow(y_, 2));
    vector_x = x_ / vector_size;
    vector_y = y_ / vector_size;
}

bool EventManager::HandleEvents(Client::ConnectionClient& cc, GameEntities::GlobalEnvironment& ge, bool enable) {
    if (!SDL_PollEvent(&gameEvent)) {
        cc.sendEvent();
        return quit;
    }
    do {
        switch (gameEvent.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if (!gameEvent.key.repeat && !enable)
                    buttons[gameEvent.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                if (!gameEvent.key.repeat && !enable)
                    buttons[gameEvent.key.keysym.scancode] = false;
                break;
            case SDL_MOUSEMOTION: {
                SDL_GetMouseState(&x, &y);
                GetCursorPosition();
                angle = std::atan2(window->height / 2 - (float) y, window->width / 2 - (float) x) * 180 / M_PI + 180;
                rotateEvent.angle = angle;
                cc.sendEvent(rotateEvent);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                SDL_GetMouseState(&x, &y);
                GetCursorPosition();
                NormalizeVector(x - window->width / 2, y - window->height / 2);
                shootEvent.x = vector_x;
                shootEvent.y = vector_y;
                cc.sendEvent(shootEvent);
                break;
            }
            default: break;
        }

        if (buttons[SDL_SCANCODE_W] && buttons[SDL_SCANCODE_A]) {
            moveEvent.movement = ClientServer::Movement::UL;
            cc.sendEvent(moveEvent);
        } else if (buttons[SDL_SCANCODE_W] && buttons[SDL_SCANCODE_D]) {
            moveEvent.movement = ClientServer::Movement::UR;
            cc.sendEvent(moveEvent);
        } else if (buttons[SDL_SCANCODE_S] && buttons[SDL_SCANCODE_A]) {
            moveEvent.movement = ClientServer::Movement::DL;
            cc.sendEvent(moveEvent);
        } else if (buttons[SDL_SCANCODE_S] && buttons[SDL_SCANCODE_D]) {
             moveEvent.movement = ClientServer::Movement::DR;
             cc.sendEvent(moveEvent);
        } else if (buttons[SDL_SCANCODE_W]) {
            moveEvent.movement = ClientServer::Movement::U;
            cc.sendEvent(moveEvent);
        } else if (buttons[SDL_SCANCODE_S]) {
            moveEvent.movement = ClientServer::Movement::D;
            cc.sendEvent(moveEvent);
        } else if (buttons[SDL_SCANCODE_A]) {
            moveEvent.movement = ClientServer::Movement::L;
            cc.sendEvent(moveEvent);
        } else if (buttons[SDL_SCANCODE_D]) {
            moveEvent.movement = ClientServer::Movement::R;
            cc.sendEvent(moveEvent);
        } else {
            moveEvent.movement = ClientServer::Movement::Void;
            cc.sendEvent(moveEvent);
        }

    } while (SDL_PollEvent(&gameEvent));
    return quit;
}
