#include "camera.hpp"

Camera::Camera(Window *window, size_t player_id) : offset_x(0), offset_y(0) {
    this->window = window;
    this->player_id = player_id;
    this->width = (float) window->width;
    this->height = (float) window->height;
    this->color = {216, 151, 70, 255};
    font = window->texture.textFont;
}

void Camera::Update(const std::vector<GameEntities::Player> &Players, const std::vector<GameEntities::Bullet> &Bullets){
    for (auto &player: Players) {
        if (player.getID() == player_id) {
            offset_x = player.getX() * scale - width / 2;
            offset_y = player.getY() * scale - height / 2;
            WindowPlayerInformation(player);
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

void Camera::WindowPlayerInformation(const GameEntities::Player& player) {
    position = "(" + boost::lexical_cast<std::string>(round(player.getX())) + "; "
            + boost::lexical_cast<std::string>(round(player.getY())) + ")";
    window->DrawText(position, font, color, 36, window->width / 2, 25);
    window->DrawText("Kills: " + boost::lexical_cast<std::string>(player.getID()),
            font, color, 36, window->width / 2, 60);
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
    SDL_Texture *texture = window->imageList[player.getTextureId()];
    playerWindowPosition.x = player.getX() * scale - offset_x;
    playerWindowPosition.y = player.getY() * scale - offset_y;
    SetRectangle(playerWindowPosition.x, playerWindowPosition.y, playerTexture.width, playerTexture.height);
    SDL_FPoint center = {rect.w / 2, rect.h / 2};
    window->DrawTexture(texture, &rect, player.getAngle(), &center);

    texture = window->imageList[Tile::HEALTH_LINE];
    SetRectangle(playerWindowPosition.x,playerWindowPosition.y - scale - 8,
                 healthLineTexture.width, healthLineTexture.height);
    window->DrawTexture(texture, &rect, 0, &center);

    texture = GetHealthColor((float) player.getHp() / 100);
    for (int i = 0; i < player.getHp() / 10; ++i) {
        SetRectangle(playerWindowPosition.x - scale - 5 + i * healthStep,playerWindowPosition.y - scale - 8,
                     healthPointTexture.width, healthPointTexture.height);
        window->DrawTexture(texture, &rect, 0, &center);
    }
}

void Camera::Render(const GameEntities::Bullet &bullet) {
    SDL_Texture *texture = window->imageList[bullet.getTextureId() + textures_num];
    SetRectangle(bullet.getX() * scale - offset_x, bullet.getY() * scale - offset_y,
                 size_of_bullet * scale,size_of_bullet * scale);
    SDL_FPoint center = {rect.w / 2, rect.h / 2};
    window->DrawTexture(texture, &rect, 0, &center);
}
