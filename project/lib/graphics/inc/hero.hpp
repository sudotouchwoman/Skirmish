#ifndef HERO_H
#define HERO_H

#include "core.hpp"
#include "graphics.hpp"

class Entity {
public:
    core::vec2 position{};
    SDL_FRect fieldRect{};
    SDL_FPoint center{};
    double speed;

    Entity() : speed(0) {}
    virtual ~Entity() = default;
};

class Bullet : public Entity {
public:
    SDL_Texture* bulletIcon;
    core::vec2 shootDirection{};

    Bullet() : bulletIcon(nullptr) {}
    ~Bullet() override = default;
};

class Hero : public Entity {
public:
    double rotation;
    SDL_Texture* heroIcon;
    Bullet bullet;

    void Render();
    void PropertiesUpdate();
    Hero() : rotation(0), heroIcon(nullptr) {}
    ~Hero() override = default;
};

class Button : public Hero {
public:
    bool status;
    SDL_Texture* field;
    SDL_Texture* selected;
    SDL_Texture* text;

    Button() : status(false), field(nullptr), selected(nullptr), text(nullptr) {
        field = Window::Instance().imageList[Tile::ICON_OFF];
        selected = Window::Instance().imageList[Tile::ICON_ON];
    }
    ~Button() override = default;
};

#endif //HERO_H
