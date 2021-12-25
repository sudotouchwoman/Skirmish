#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "GameSettings.h"
#include "GameObjects.h"
#include "GlobalEnvironment.h"
#include "window.hpp"
#include "boost/lexical_cast.hpp"

class Camera {
public:
    void Update(const std::vector <GameEntities::Player>& Players,
                const std::vector <GameEntities::Bullet>& Bullets,
                const std::vector<GameEntities::Terrain>& Terrain,
                const std::vector<GameEntities::Message>& Message);
    Camera(Window* window, size_t player_id);

    Camera(const Camera &other) = delete;
    Camera &operator=(const Camera &other) = delete;

private:
    struct WindowPosition {
        float x;
        float y;
    };
    WindowPosition playerWindowPosition{};

    struct ObjectRenderSizeParameters {
        float width;
        float height;
    };

    SDL_FRect rect{};
    SDL_Color color{};
    Window* window;
    float width, height, offset_x, offset_y;
    size_t player_id;
    std::string font, position;

    const float scale = 40;
    const float healthThreshold = 1. / 3;
    const float healthStep = 10;
    ObjectRenderSizeParameters playerTexture = {2 * size_of_player * scale, 2 * size_of_player * scale};
    ObjectRenderSizeParameters healthLineTexture = {103, 12};
    ObjectRenderSizeParameters healthPointTexture = {healthStep, healthStep};

    void SetRectangle(float x, float y, float width_, float height_);
    void Render(const GameEntities::Player& player);
    void Render(const GameEntities::Bullet& bullet);
    void Render(const GameEntities::Terrain& terrain);
    void Render(const GameEntities::Message& message);
    void WindowPlayerInformation(const GameEntities::Player& player);
    SDL_Texture* GetHealthColor(float health);
};

#endif //CAMERA_HPP
