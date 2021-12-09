#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <map>

#include "tile.hpp"

class Textures {
public:
    std::map<int, std::string> textureImageMap;
    std::map<int, std::string> textureFontMap;
    std::map<int, SDL_Texture *> PreLoadImageTextures(SDL_Renderer *renderer);

    Textures();

    static SDL_Texture*
    LoadTextTexture(SDL_Renderer *renderer, const std::string &text, const std::string &path, SDL_Color color,
                    int fontSize);

private:
    const std::string IMAGE_DIRECTORY_PATH = (std::string) SDL_GetBasePath() + "textures/";
    const std::string FONT_DIRECTORY_PATH = (std::string) SDL_GetBasePath() + "ttf/";

    void AddImagePath(int index, const std::string &path);
    void AddFontPath(int index, const std::string &path);

    static SDL_Texture* LoadImageTexture(SDL_Renderer *renderer, const std::string &path);
};

#endif //TEXTURE_H
