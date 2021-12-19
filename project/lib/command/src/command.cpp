#include "command.hpp"

EventManager::EventManager(SDL_Texture* texture, size_t player_id) : quit(false), x(0), y(0), width(0), height(0), angle(0), vector_x(0), vector_y(0) {
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    this->player_id = player_id;
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

bool EventManager::HandleEvents(Client::ConnectionClient& cc, GameEntities::GlobalEnvironment& ge) {
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
                switch (gameEvent.key.keysym.scancode) {
                    case SDL_SCANCODE_W: {
                        moveEvent.movement = ClientServer::Movement::U;
                        cc.sendEvent(moveEvent);
                        break;
                    }
                    case SDL_SCANCODE_S: {
                        moveEvent.movement = ClientServer::Movement::D;
                        cc.sendEvent(moveEvent);
                        break;
                    }
                    case SDL_SCANCODE_A: {
                        moveEvent.movement = ClientServer::Movement::L;
                        cc.sendEvent(moveEvent);
                        break;
                    }
                    case SDL_SCANCODE_D: {
                        moveEvent.movement = ClientServer::Movement::R;
                        cc.sendEvent(moveEvent);
                        break;
                    }
                    default:
                        break;
                }
                break;
            case SDL_MOUSEMOTION: {
                SDL_GetMouseState(&x, &y);
                GetCursorPosition();
                angle = std::atan2(ge.getPlayerById(player_id).getY() - (float) y, ge.getPlayerById(player_id).getX() - (float) x) * 180 / M_PI + 180;
                rotateEvent.angle = angle;
                cc.sendEvent(rotateEvent);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                SDL_GetMouseState(&x, &y);
                GetCursorPosition();
                NormalizeVector(x - ge.getPlayerById(player_id).getX(), y - ge.getPlayerById(player_id).getY());
                shootEvent.x = vector_x;
                shootEvent.y = vector_y;
                //shootEvent.weapon = 1;
                cc.sendEvent(shootEvent);
                break;
            }
            default:
                break;
        }
    } while (SDL_PollEvent(&gameEvent));
    return quit;
}

