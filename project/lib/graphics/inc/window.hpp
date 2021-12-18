#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <string>

#include "texture.hpp"

class Menu;
class Window {
public:
    int width;
    int height;
    Textures textures;
    std::map<int, SDL_Texture*> imageList;

    void Render();
    void ClearSurface();
    void DrawTexture(SDL_Texture *texture, SDL_FRect* rect, float angle, SDL_FPoint* center);
    void GetTextureRectPosition(SDL_FRect* rect, SDL_Texture* texture, float x, float y);
    void SetCursor(const std::string& path);

    Window(const Window &other) = delete;
    Window &operator=(const Window &other) = delete;
    Window() : width(1000), height(720), window(nullptr), renderer(nullptr) {Init();}
    ~Window();

private:
    friend class Menu;
    SDL_Window* window;
    SDL_Renderer* renderer;

    void Init();
};

#endif // WINDOW_HPP