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
}

void Textures::AddImagePath(size_t index, const std::string& path) {
    textureImageMap[index] = path;
}
