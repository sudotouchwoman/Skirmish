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
            offset_x = player.getX() * scale - width / 2;
            offset_y = player.getY() * scale - height / 2;
            PlayerRender(player);
        } else if (player.getX() * scale >= offset_x && player.getX() * scale <= offset_x + width &&
                   player.getY() * scale >= offset_y  && player.getY() * scale <= offset_y + height)
            Render(player);
    }

//    std::cout << "(" << offset_x << ";" << offset_y << ")" << " "
//              << "(" << offset_x + width << ";" << offset_y << ")" << " "
//              << "(" << offset_x << ";" << offset_y + height << ")" << " "
//              << "(" << offset_x + width << ";" << offset_y + height << ")" << std::endl;
    std::cout << Bullets.size() << std::endl;
    for (auto& bullet: Bullets) {
        if (bullet.getX() * scale >= offset_x && bullet.getX() * scale <= offset_x + width &&
            bullet.getY() * scale >= offset_y  && bullet.getY() * scale <= offset_y + height)
            Render(bullet);
    }
}

void Camera::SetRectangle(float x, float y, float size) {
    rect.w = size;
    rect.h = size;
    rect.x = x - rect.w / 2;
    rect.y = y - rect.h / 2;
}

void Camera::PlayerRender(const GameEntities::Player &player) {
    SDL_Texture* texture = window->imageList[player.getTextureId()];
    SetRectangle(player.getX() * scale - offset_x, player.getY() * scale - offset_y, size_of_player * scale * 2);
    SDL_FPoint center = {rect.w / 2,rect.h / 2};
    window->DrawTexture(texture, &rect, player.getAngle(), &center);
}

void Camera::Render(const GameEntities::Player& player) {
    SDL_Texture* texture = window->imageList[player.getTextureId()];
    SetRectangle(player.getX() * scale, player.getY() * scale, size_of_player * scale * 2);
    SDL_FPoint center = {rect.w / 2,rect.h / 2};
    window->DrawTexture(texture, &rect, player.getAngle(), &center);
}

void Camera::Render(const GameEntities::Bullet& bullet) {
    SDL_Texture* texture = window->imageList[1];
    SetRectangle(bullet.getX() * scale - offset_x, bullet.getY() * scale - offset_y, size_of_bullet * scale * 2);
    SDL_FPoint center = {rect.w / 2, rect.h / 2};
    //std::cout << rect.x << " " << rect.y << std::endl;
    window->DrawTexture(texture, &rect, 0, &center);
}
