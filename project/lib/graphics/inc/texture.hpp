#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace Tile {
    enum Type {
        HERO_CPP, HERO_GIT, HERO_JAVA,
        BULLET_CPP, BULLET_GIT, BULLET_JAVA,
        ICON_OFF, ICON_ON, BUTTON_OFF, BUTTON_ON,
        HEALTH_LINE, GREEN, ORANGE, RED,
//        FLOOR,
//        WALL,
        MENU_CURSOR, GAME_CURSOR};
}

namespace Font {
    enum Type {ABBIESHIRE, BIGHAUSTITUL, COLUMBIA, GASALT, OCTIN, SABRIL, SAMSON};
}

class Textures {
public:
    std::vector<std::string> heroName = {"Cpp", "Git", "Java"};
    std::map<size_t, std::string> textureImageMap;
    std::map<size_t, std::string> textureFontMap;
    std::map<size_t, SDL_Texture*> PreLoadImageTextures(SDL_Renderer *renderer);

    Textures();
    SDL_Texture* LoadTextTexture(SDL_Renderer *renderer, const std::string &text, const std::string &path, SDL_Color color, int fontSize);

    Textures(const Textures &other) = delete;
    Textures &operator=(const Textures &other) = delete;

private:
    const std::string IMAGE_DIRECTORY_PATH = (std::string) SDL_GetBasePath() + "lib/textures/";
    const std::string FONT_DIRECTORY_PATH = (std::string) SDL_GetBasePath() + "lib/ttf/";

    void AddImagePath(size_t index, const std::string &path);
    void AddFontPath(size_t index, const std::string &path);

    static SDL_Texture* LoadImageTexture(SDL_Renderer *renderer, const std::string &path);
};

#endif //TEXTURE_H
