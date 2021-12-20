#ifndef MENU_HPP
#define MENU_HPP

#include "fps.hpp"
#include "window.hpp"
#include "GameSettings.h"
#include "ConnectionClient.h"

class Button {
public:
    struct Position {
        float x;
        float y;
    };

    Position position{};
    bool status;

    SDL_FRect rect{};
    SDL_Texture* field;
    SDL_Texture* selected;
    SDL_Texture* text;
    SDL_Texture* heroIcon;

    Button() : status(false), field(nullptr), selected(nullptr), text(nullptr), heroIcon(nullptr) {}
};

class Menu {
public:

    void Load(Client::ConnectionClient& cc);
    void GetTextureRectPosition(SDL_FRect* rect, SDL_Texture* texture, float x, float y);
    void InverseTimer(int second);

    Menu(Window *window, int frames);
    Menu(const Menu &other) = delete;
    Menu &operator=(const Menu &other) = delete;

private:
    bool ready;
    int frames;
    std::string font;
    Window* window;

    SDL_Event menuEvent{};
    SDL_Color color{};

    void HeroButtonLoad(std::vector<Button>& button);
    bool ButtonPressedCheck(std::vector<Button>& button);
    void GetCursorPosition(int& x, int& y);
};

#endif //MENU_HPP
