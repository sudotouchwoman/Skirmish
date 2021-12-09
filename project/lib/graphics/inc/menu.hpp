#ifndef MENU_H
#define MENU_H

#include <memory>
#include "fps.hpp"
#include "hero.hpp"

class Menu {
public:
    static void Load(Window& window, int fps, std::unique_ptr<Hero>& hero);
    static void GetTextureRectPosition(SDL_FRect* rect, SDL_Texture* texture, double x, double y);
    static void InverseTimer(Window &window, int second);

private:
    static bool ready;
    static SDL_Event menuEvent;
    static SDL_Color color;
    static int iconNumber;
    static std::string font;

    static void HeroButtonLoad(Window& window, std::vector<Button>& button);
    static bool ButtonPressedCheck(std::vector<Button>& button);
    static void GetCursorPosition(Window& window, int& x, int& y);
};

#endif //MENU_H
