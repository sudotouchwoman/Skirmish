#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "GameSettings.h"
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
    SDL_FRect rect{};
    Window* window;
    float width, height, offset_x, offset_y;
    size_t player_id;
    const float scale = 40;
    //const float bullet_scale = 37.5;

    void SetRectangle(float x, float y, float size);
    void Render(const GameEntities::Player& player);
    void Render(const GameEntities::Bullet& bullet);
};

#endif //CAMERA_HPP
