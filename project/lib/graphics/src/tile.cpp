#include "tile.hpp"

const std::vector<std::string> Font::heroName = {"Asm", "Cpp", "Git", "Go", "Java", "Javascript", "Lua", "Python"};
const std::vector<std::string> Font::gameText = {"Skirmish", "Select hero", "Play", "3", "2", "1", "Go", "Winner!", "Loser.."};

Tile::Tile(Type tile, int height, int width, core::vec2 position, Rotation rotation) : texture(nullptr) {
    this->type = tile;
    this->height = height;
    this->width = width;
    this->position = position;
    this->rotation = rotation;
    this->isCollidable = false;
    this->isVisible = true;

    for (auto& item : collidableTiles) {
        if (this->type == item) {
            this->isCollidable = true;
        }
    }
    for (auto& item : invisibleTiles) {
        if (this->type == item) {
            this->isVisible = false;
        }
    }
}

SDL_Rect Tile::GetRect() const {
    SDL_Rect rect;
    rect.w = width;
    rect.h = height;
    rect.x = position.x;
    rect.y = position.y;
    return rect;
}

core::vec2 Tile::GetCenter() const {
    int center_x = position.x - width / 2;
    int center_y = position.y - height / 2;
    return core::vec2(center_x, center_y);
}
