#include "command.hpp"

bool Input::quit = false;
SDL_Event Input::gameEvent = {};
int Input::x = 0, Input::y = 0;

bool Input::HeroAction(const std::unique_ptr<Hero>& hero) {
    while (SDL_PollEvent(&gameEvent)) {
        switch (gameEvent.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (gameEvent.key.keysym.scancode) {
                    case SDL_SCANCODE_W: {
                        Up command;
                        command.Move(hero->fieldRect.x, hero->fieldRect.y);
                        break;
                    }
                    case SDL_SCANCODE_S: {
                        Down command;
                        command.Move(hero->fieldRect.x, hero->fieldRect.y);
                        break;
                    }
                    case SDL_SCANCODE_A: {
                        Left command;
                        command.Move(hero->fieldRect.x, hero->fieldRect.y);
                        break;
                    }
                    case SDL_SCANCODE_D: {
                        Right command;
                        command.Move(hero->fieldRect.x, hero->fieldRect.y);
                        break;
                    }
                    default:
                        break;
                }
                break;
            case SDL_MOUSEMOTION: {
                SDL_GetMouseState(&x, &y);
                core::vec2 cursorPosition = {static_cast<double>(x), static_cast<double>(y)};
                GetCursorPosition();
                Rotation command;
                command.Rotate(hero->position, cursorPosition, hero->rotation);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                SDL_GetMouseState(&x, &y);
                core::vec2 cursorPosition = {static_cast<double>(x), static_cast<double>(y)};
                GetCursorPosition();
                Fire command;
                command.Shoot(hero, cursorPosition);
                break;
            }
            default:
                break;
        }
    }
    return quit;
}

void Input::GetCursorPosition() {
    int width, height;
    SDL_QueryTexture(Window::Instance().imageList[Tile::MENU_CURSOR], nullptr, nullptr, &width, &height);
    x += width / 2;
    y += height / 2;
}

