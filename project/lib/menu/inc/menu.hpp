#ifndef MENU_HPP
#define MENU_HPP

#include "fps.hpp"
#include "window.hpp"
#include "GameSettings.h"

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
    std::string text;
    SDL_Texture* heroIcon;
    int texture_id;

    Button() : status(false), field(nullptr), selected(nullptr), heroIcon(nullptr), texture_id(0) {}
};

class Menu {
public:

    void Load(std::string& name, int& texture_id);
    void InverseTimer(int second);

    Menu(Window *window, int frames);
    Menu(const Menu &other) = delete;
    Menu &operator=(const Menu &other) = delete;

private:
    bool ready;
    int frames, maxNameLength;
    std::string font;
    Window* window;

    SDL_Event menuEvent{};
    SDL_Color color{};

    void HeroButtonLoad(std::vector<Button>& button);
    bool ButtonPressedCheck(std::vector<Button>& button);
    void GetCursorPosition(int& x, int& y);
    void GetTextureRectPosition(SDL_FRect* rect, SDL_Texture* texture, float x, float y);
    void GetTextInputFieldPosition(SDL_FRect* rect, float x, float y, float width_, float height_);
};

#endif //MENU_HPP
