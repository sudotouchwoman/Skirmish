#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "GameObjects.h"
#include "GlobalEnvironment.h"
#include "window.hpp"

class Camera {
public:
    void Update(const std::vector <GameEntities::Player>& Players, const std::vector <GameEntities::Bullet>& Bullets);
    Camera(Window* window, size_t player_id);

    Camera(const Camera &other) = delete;
    Camera &operator=(const Camera &other) = delete;

private:
    SDL_FRect* rect{};
    Window* window;
    float width, height, offset_x, offset_y;
    size_t player_id;

    void GetRectangle(SDL_Texture* texture, float x, float y) const;
    void Render(const GameEntities::Player& player) const;
    void Render(const GameEntities::Bullet& bullet) const;
};

#endif //CAMERA_HPP
