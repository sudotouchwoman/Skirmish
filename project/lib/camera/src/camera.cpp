#include "camera.hpp"

Camera::Camera(Window *window, size_t player_id) : offset_x(0), offset_y(0) {
    this->window = window;
    this->player_id = player_id;
    this->width = (float) window->width;
    this->height = (float) window->height;
    this->color = {216, 151, 70, 255};
    font = window->texture.textFont;
}

void Camera::Update(const std::vector<GameEntities::Player> &Players,
                    const std::vector<GameEntities::Bullet> &Bullets,
                    const std::vector<GameEntities::Terrain> &Terrain,
                    const std::vector<GameEntities::Message>& Message) {
    for (auto &player: Players) {
        if (player.getID() == player_id) {
            offset_x = player.x_ * scale - width / 2;
            offset_y = player.y_ * scale - height / 2;
        }
        if (player.x_ * scale >= offset_x && player.x_ * scale <= offset_x + width &&
            player.y_ * scale >= offset_y && player.y_ * scale <= offset_y + height)
            Render(player);
    }
    for (auto &bullet: Bullets) {
        if (bullet.x_ * scale >= offset_x && bullet.x_ * scale <= offset_x + width &&
            bullet.y_ * scale >= offset_y && bullet.y_ * scale <= offset_y + height)
            Render(bullet);
    }
    for (auto &terrain: Terrain) {
        Render(terrain);
    }
    for (auto &message: Message) {
        Render(message);
    }
    for (auto &player: Players) {
        if (player.getID() == player_id)
            WindowPlayerInformation(player);
    }
}

void Camera::SetRectangle(float x, float y, float width_, float height_) {
    rect.w = width_;
    rect.h = height_;
    rect.x = x - rect.w / 2;
    rect.y = y - rect.h / 2;
}

void Camera::WindowPlayerInformation(const GameEntities::Player& player) {
    position = "(" + boost::lexical_cast<std::string>(round(player.x_)) + "; "
            + boost::lexical_cast<std::string>(round(player.y_)) + ")";
    window->DrawText(position, font, color, 36, window->width / 2, 25);
}

SDL_Texture* Camera::GetHealthColor(float health) {
    if (health > 2 * healthThreshold)
        return window->imageList[Tile::GREEN];
    else if (health > healthThreshold && health < 2 * healthThreshold)
        return window->imageList[Tile::ORANGE];
    else
        return window->imageList[Tile::RED];
}

void Camera::Render(const GameEntities::Player &player) {
    SDL_Texture *texture = window->imageList[player.texture_id];
    playerWindowPosition.x = player.x_ * scale - offset_x;
    playerWindowPosition.y = player.y_ * scale - offset_y;
    SetRectangle(playerWindowPosition.x, playerWindowPosition.y, playerTexture.width, playerTexture.height);
    SDL_FPoint center = {rect.w / 2, rect.h / 2};
    window->DrawTexture(texture, &rect, player.angle_, &center);

    texture = window->imageList[Tile::HEALTH_LINE];
    SetRectangle(playerWindowPosition.x,playerWindowPosition.y - scale - 8,
                 healthLineTexture.width, healthLineTexture.height);
    window->DrawTexture(texture, &rect, 0, &center);

    texture = GetHealthColor((float) player.getHp() / 100);
    for (int i = 0; i < player.getHp() / 10; ++i) {
        SetRectangle(playerWindowPosition.x - scale - 5 + i * healthStep, playerWindowPosition.y - scale - 8,
                     healthPointTexture.width, healthPointTexture.height);
        window->DrawTexture(texture, &rect, 0, &center);
    }
}

void Camera::Render(const GameEntities::Bullet &bullet) {
    SDL_Texture *texture = window->imageList[bullet.texture_id + textures_num];
    SetRectangle(bullet.x_ * scale - offset_x, bullet.y_ * scale - offset_y,
                 size_of_bullet * scale,size_of_bullet * scale);
    SDL_FPoint center = {rect.w / 2, rect.h / 2};
    window->DrawTexture(texture, &rect, 0, &center);
}

void Camera::Render(const GameEntities::Terrain& terrain) {
    SDL_Texture *texture = window->imageList[Tile::WALL];
    SetRectangle(terrain.x_ * scale - offset_x, terrain.y_ * scale - offset_y,
                 terrain.w_ * scale, terrain.h_ * scale);
    SDL_FPoint center = {rect.w / 2, rect.h / 2};
    window->DrawTexture(texture, &rect, 0, &center);
}

void Camera::Render(const GameEntities::Message& message) {
    window->DrawText(message.getMessage(), font, color, 150, window->width / 2, window->height / 2);
}
