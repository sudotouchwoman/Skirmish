#include "camera.hpp"

Camera::Camera(Window *window, size_t player_id) : offset_x(0), offset_y(0) {
    this->window = window;
    this->player_id = player_id;
    this->width = (float) window->width;
    this->height = (float) window->height;
}

void
Camera::Update(const std::vector<GameEntities::Player> &Players, const std::vector<GameEntities::Bullet> &Bullets) {
    for (auto &player: Players) {
        if (player.getID() == player_id) {
            offset_x = player.getX() * scale - width / 2;
            offset_y = player.getY() * scale - height / 2;

        }
        if (player.getX() * scale >= offset_x && player.getX() * scale <= offset_x + width &&
            player.getY() * scale >= offset_y && player.getY() * scale <= offset_y + height)
            Render(player);
    }
    for (auto &bullet: Bullets) {
        if (bullet.getX() * scale >= offset_x && bullet.getX() * scale <= offset_x + width &&
            bullet.getY() * scale >= offset_y && bullet.getY() * scale <= offset_y + height)
            Render(bullet);
    }
}

void Camera::SetRectangle(float x, float y, float width_, float height_) {
    rect.w = width_;
    rect.h = height_;
    rect.x = x - rect.w / 2;
    rect.y = y - rect.h / 2;
}

void Camera::Render(const GameEntities::Player &player) {
    SDL_Texture *texture = window->imageList[player.getTextureId()];
    SetRectangle(player.getX() * scale - offset_x, player.getY() * scale - offset_y, size_of_player * scale * 2,
                 size_of_player * scale * 2);
    SDL_FPoint center = {rect.w / 2, rect.h / 2};
    window->DrawTexture(texture, &rect, player.getAngle(), &center);

    texture = window->imageList[Tile::HEALTH_LINE];
    SetRectangle(player.getX() * scale - offset_x - 1, player.getY() * scale - offset_y - rect.h / 2 - 8,
                 size_of_player * scale * 2 - 2, size_of_player * scale / 4);
    window->DrawTexture(texture, &rect, 0, &center);
    int health = player.getHp() / 10;
    if (health > 6)
        texture = window->imageList[Tile::GREEN];
    else if (health > 3 && health < 7)
        texture = window->imageList[Tile::ORANGE];
    else
        texture = window->imageList[Tile::RED];

    for (int i = 0; i < health; ++i) {
        SetRectangle(player.getX() * scale - offset_x - 40 + 5 + static_cast<float>(i) * 7.5,
                     player.getY() * scale - offset_y - 40 - 8, size_of_player * scale / 4 - 2,
                     size_of_player * scale / 4 - 2);
        window->DrawTexture(texture, &rect, 0, &center);
    }
}

void Camera::Render(const GameEntities::Bullet &bullet) {
    SDL_Texture *texture = window->imageList[bullet.getTextureId() + textures_num];
    SetRectangle(bullet.getX() * scale - offset_x, bullet.getY() * scale - offset_y, size_of_bullet * scale,
                 size_of_bullet * scale);
    SDL_FPoint center = {rect.w / 2, rect.h / 2};
    window->DrawTexture(texture, &rect, 0, &center);
}
