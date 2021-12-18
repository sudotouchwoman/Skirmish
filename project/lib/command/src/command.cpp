#include "command.hpp"

EventManager::EventManager(SDL_Texture* texture) : quit(false), x(0), y(0), width(0), height(0) {
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
}

void EventManager::GetCursorPosition() {
    x += width / 2;
    y += height / 2;
}

bool EventManager::HandleEvents(Client::ConnectionClient& connectionClient) {
    while (SDL_PollEvent(&gameEvent)) {
        switch (gameEvent.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (gameEvent.key.keysym.scancode) {
                    case SDL_SCANCODE_W: {
                        moveEvent.movement = ClientServer::Movement::U;
                        connectionClient.sendEvent(moveEvent);
                        break;
                    }
                    case SDL_SCANCODE_S: {
                        moveEvent.movement = ClientServer::Movement::D;
                        connectionClient.sendEvent(moveEvent);
                        break;
                    }
                    case SDL_SCANCODE_A: {
                        moveEvent.movement = ClientServer::Movement::L;
                        connectionClient.sendEvent(moveEvent);
                        break;
                    }
                    case SDL_SCANCODE_D: {
                        moveEvent.movement = ClientServer::Movement::R;
                        connectionClient.sendEvent(moveEvent);
                        break;
                    }
                    default:
                        break;
                }
                break;
            case SDL_MOUSEMOTION: {
                SDL_GetMouseState(&x, &y);
                GetCursorPosition();
                rotateEvent.x = x;
                rotateEvent.y = y;
                connectionClient.sendEvent(rotateEvent);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                SDL_GetMouseState(&x, &y);
                GetCursorPosition();
                shootEvent.x = x;
                shootEvent.y = y;
                shootEvent.weapon = 0;
                connectionClient.sendEvent(shootEvent);
                break;
            }
            default:
                break;
        }
    }
    return quit;
}

