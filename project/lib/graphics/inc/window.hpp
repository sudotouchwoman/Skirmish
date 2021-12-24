#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <string>

#include "texture.hpp"

class Window {
public:
    int width;
    int height;
    Textures texture;
    std::map<size_t, SDL_Texture*> imageList;

    void Render();
    void ClearSurface();
    void DrawTexture(SDL_Texture *texture_, SDL_FRect* rect, float angle, SDL_FPoint* center);
    void DrawText(const std::string &text, const std::string &path, SDL_Color color, int fontSize, float x_, float y_);
    void SetCursor(const std::string& path);

    Window(const Window &other) = delete;
    Window &operator=(const Window &other) = delete;
    Window() : width(1000), height(720), window(nullptr), renderer(nullptr) {Init();}
    ~Window();

private:
    SDL_FRect rect{};
    SDL_Window* window;
    SDL_Renderer* renderer;

    void Init();
    SDL_Texture* LoadImageTexture(const std::string &path);
    std::map<size_t, SDL_Texture*> PreLoadImageTextures();
    void SetRectangle(float x, float y, SDL_Texture* texture_);
};

#endif // WINDOW_HPP