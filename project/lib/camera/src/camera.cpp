#include "camera.hpp"

Camera::Camera(Window* window, size_t player_id) : offset_x(0), offset_y(0) {
    this->window = window;
    this->player_id = player_id;
    this->width = (float) window->width;
    this->height = (float) window->height;
}

void Camera::Update(const std::vector<GameEntities::Player>& Players, const std::vector<GameEntities::Bullet>& Bullets) {
    for (auto& player: Players) {
        if (player.getID() == player_id) {
            offset_x = player.getX() - width / 2;
            offset_y = player.getY() - height / 2;
        }
    }

    for (auto& player: Players) {
        std::cout << player.getX() << " " << player.getY() << std::endl;
        if (player.getX() >= offset_x && player.getX() <= offset_x + width && player.getY() >= offset_y  && player.getY() <= offset_y + height)
            Render(player);
    }
    for (auto& bullet: Bullets) {
        if (bullet.getX() >= offset_x && bullet.getX() <= offset_x + width && bullet.getY() >= offset_y  && bullet.getY() <= offset_y + height)
            Render(bullet);
    }
}

void Camera::SetRectangle(float x, float y, float size) {
    rect.w = size;
    rect.h = size;
    rect.x = x - rect.w / 2;
    rect.y = y - rect.h / 2;
}

void Camera::Render(const GameEntities::Player& player) {
    SDL_Texture* texture = window->imageList[player.getTextureId()];
    SetRectangle(player.getX(), player.getY(), size_of_player);
    SDL_FPoint center = {rect.w / 2, rect.h / 2};
    window->DrawTexture(texture, &rect, player.getAngle(), &center);
}

void Camera::Render(const GameEntities::Bullet& bullet) {
    SDL_Texture* texture = window->imageList[1];
    SetRectangle(bullet.getX(), bullet.getY(), size_of_bullet);
    SDL_FPoint center = {rect.w / 2, rect.h / 2};
    window->DrawTexture(texture, &rect, 0, &center);
}
