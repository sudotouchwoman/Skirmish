#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <string>

#include "texture.hpp"

class Manager;
class Window {
public:
    static Window& Instance();
    static int width;
    static int height;
    Textures textures;
    std::map<int, SDL_Texture*> imageList;

    void Render();
    void ClearSurface();
    void DrawTexture(SDL_Texture *texture, SDL_FRect* rect);
    void DrawHeroTexture(SDL_Texture *texture, SDL_FRect* rect, double angle, SDL_FPoint* center);
    static void SetCursor(const std::string& path);


    Window(const Window &) = delete;
    Window& operator=(const Window &) = delete;

    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

private:
    friend class Menu;
    SDL_Window* window;
    SDL_Renderer* renderer;

    void Init();

    Window();
    ~Window();
};

#endif // WINDOW_HPP