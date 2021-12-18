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
        if (player.getX() >= offset_x && player.getX() <= offset_x + width &&
            player.getY() >= offset_y  && player.getY() <= offset_y + height)
            Render(player);
    }
    for (auto& bullet: Bullets) {
        if (bullet.getX() >= offset_x && bullet.getX() <= offset_x + width &&
            bullet.getY() >= offset_y  && bullet.getY() <= offset_y + height)
            Render(bullet);
    }
}

void Camera::GetRectangle(SDL_Texture* texture, float x, float y) const {
    int width_, height_;
    SDL_QueryTexture(texture, nullptr, nullptr, &width_, &height_);
    rect->w = (float) width_;
    rect->h = (float) height_;
    rect->x = x - rect->w / 2;
    rect->y = y - rect->h / 2;
}

void Camera::Render(const GameEntities::Player& player) const {
    SDL_Texture* texture = nullptr; //через player_id из хеш-таблицы со стороны клиента
    SDL_FPoint center = {player.getX(), player.getY()};
    GetRectangle(texture, player.getX(), player.getY());
    window->DrawTexture(texture, rect, player.getAngle(), &center);
}

void Camera::Render(const GameEntities::Bullet& bullet) const {
    SDL_Texture* texture = nullptr; //через player_id из хеш-таблицы со стороны клиента
    GetRectangle(texture, bullet.getX(), bullet.getY());
    window->DrawTexture(texture, rect, 0, {});
}
