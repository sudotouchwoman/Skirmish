#include "texture.hpp"

Textures::Textures() {
    AddImagePath(Tile::HERO_CPP, IMAGE_DIRECTORY_PATH + "hero/cpp.png");
    AddImagePath(Tile::HERO_GIT, IMAGE_DIRECTORY_PATH + "hero/git.png");
    AddImagePath(Tile::HERO_JAVA, IMAGE_DIRECTORY_PATH + "hero/java.png");

    AddImagePath(Tile::BULLET_CPP, IMAGE_DIRECTORY_PATH + "bullet/cpp.png");
    AddImagePath(Tile::BULLET_GIT, IMAGE_DIRECTORY_PATH + "bullet/git.png");
    AddImagePath(Tile::BULLET_JAVA, IMAGE_DIRECTORY_PATH + "bullet/java.png");

    AddImagePath(Tile::ICON_OFF, IMAGE_DIRECTORY_PATH + "button/icon_off.png");
    AddImagePath(Tile::ICON_ON, IMAGE_DIRECTORY_PATH + "button/icon_on.png");
    AddImagePath(Tile::BUTTON_OFF, IMAGE_DIRECTORY_PATH + "button/button_off.png");
    AddImagePath(Tile::BUTTON_ON, IMAGE_DIRECTORY_PATH + "button/button_on.png");

    AddImagePath(Tile::HEALTH_LINE, IMAGE_DIRECTORY_PATH + "health/health.png");
    AddImagePath(Tile::GREEN, IMAGE_DIRECTORY_PATH + "health/green.png");
    AddImagePath(Tile::ORANGE, IMAGE_DIRECTORY_PATH + "health/orange.png");
    AddImagePath(Tile::RED, IMAGE_DIRECTORY_PATH + "health/red.png");

    //AddImagePath(Tile::FLOOR, IMAGE_DIRECTORY_PATH + "map/floor.png");
    //AddImagePath(Tile::WALL, IMAGE_DIRECTORY_PATH + "map/wall.png");

    AddImagePath(Tile::MENU_CURSOR, IMAGE_DIRECTORY_PATH + "cursor/menu.png");
    AddImagePath(Tile::GAME_CURSOR, IMAGE_DIRECTORY_PATH + "cursor/game.png");

    AddFontPath(Font::ABBIESHIRE, FONT_DIRECTORY_PATH + "Abbieshire.ttf");
    AddFontPath(Font::BIGHAUSTITUL, FONT_DIRECTORY_PATH + "Bighaustitul.ttf");
    AddFontPath(Font::COLUMBIA, FONT_DIRECTORY_PATH + "Columbia.ttf");
    AddFontPath(Font::GASALT, FONT_DIRECTORY_PATH + "Gasalt.ttf");
    AddFontPath(Font::OCTIN, FONT_DIRECTORY_PATH + "Octin.ttf");
    AddFontPath(Font::SABRIL, FONT_DIRECTORY_PATH + "Sabril.ttf");
    AddFontPath(Font::SAMSON, FONT_DIRECTORY_PATH + "Samson.ttf");
}

SDL_Texture* Textures::LoadTextTexture(SDL_Renderer *renderer, const std::string& text, const std::string& path, SDL_Color color, int fontSize) {
    TTF_Font *font = TTF_OpenFont(path.c_str(), fontSize);
    if (font == nullptr){
        std::cerr << "failed to load font from path " << path << ": " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface == nullptr){
        TTF_CloseFont(font);
        std::cerr << "failed to create text surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr){
        std::cerr << "failed to create text texture: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    return texture;
}

void Textures::AddFontPath(size_t index, const std::string& path) {
    textureFontMap[index] = path;
}

void Textures::AddImagePath(size_t index, const std::string& path) {
    textureImageMap[index] = path;
}


SDL_Texture* Textures::LoadImageTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* surface;

    surface = IMG_Load(path.c_str());
    if (surface == nullptr) {
        std::cerr << "failed to load image from path " << path << ": " << IMG_GetError() << std::endl;
        return texture;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        std::cerr << "failed to create texture: " << SDL_GetError() << std::endl;
        return texture;
    }
    SDL_FreeSurface(surface);
    return texture;
}

std::map<size_t, SDL_Texture*> Textures::PreLoadImageTextures(SDL_Renderer* renderer) {
    std::map<size_t, SDL_Texture*> textures;
    for (auto& it : textureImageMap) {
        SDL_Texture* texture = LoadImageTexture(renderer, it.second);
        size_t index = it.first;
        std::cout << it.second << std::endl;
        textures[index] = texture;
    }
    return textures;
}
