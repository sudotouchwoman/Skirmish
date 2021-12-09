#ifndef TILE_H
#define TILE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>
#include <vector>
#include <iostream>

#include "core.hpp"

class Tile {
public:
    enum Type {
        HERO_ASM, HERO_CPP, HERO_GIT, HERO_GO, HERO_JAVA, HERO_JAVASCRIPT, HERO_LUA, HERO_PYTHON,
        BULLET_ASM, BULLET_CPP, BULLET_GIT, BULLET_GO, BULLET_JAVA, BULLET_JAVASCRIPT, BULLET_LUA, BULLET_PYTHON,
        ICON_OFF, ICON_ON, BUTTON_OFF, BUTTON_ON,
        SPAWN,
        FLOOR,
        WALL,
        MENU_CURSOR, GAME_CURSOR};
    enum Rotation {DOWN = 0, RIGHT = -90, LEFT = 90, UP = 180};

    std::vector<Tile::Type> collidableTiles = {WALL};
    std::vector<Tile::Type> invisibleTiles = {SPAWN};

    SDL_Rect GetRect() const;
    core::vec2 GetCenter() const;
    Tile(Type tile, int height, int width, core::vec2 position, Tile::Rotation);

private:
    int height;
    int width;
    int type;
    bool isCollidable;
    bool isVisible;
    Rotation rotation;
    core::vec2 position{};
    SDL_Texture* texture;
};

class Font {
public:
    const static std::vector<std::string> heroName;
    const static std::vector<std::string> gameText;

    enum Type {ABBIESHIRE, BIGHAUSTITUL, COLUMBIA, GASALT, OCTIN, SABRIL, SAMSON};
};

#endif //TILE_H
