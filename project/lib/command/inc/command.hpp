#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <SDL.h>
#include <iostream>
#include <memory>

#include "hero.hpp"

class Command
{
public:
    virtual void Move(float& center_x, float& center_y) {};
    virtual void Rotate(core::vec2& heroPosition, core::vec2& cursorPosition, double& rotation) {};
    virtual void Shoot(const std::unique_ptr<Hero>& hero, core::vec2& cursorPosition) {};
    virtual ~Command() = default;
};

class Up : public Command {
public:
    void Move(float& center_x, float& center_y) override {}
    ~Up() override = default;
};

class Down : public Command {
public:
    void Move(float& center_x, float& center_y) override {}
    ~Down() override = default;
};

class Left : public Command {
public:
    void Move(float& center_x, float& center_y) override {}
    ~Left() override = default;
};

class Right : public Command {
public:
    void Move(float& center_x, float& center_y) override {}
    ~Right() override = default;
};

class Rotation : public Command {
public:
    void Rotate(core::vec2& heroPosition, core::vec2& cursorPosition, double& rotation) override {}
    ~Rotation() override = default;
};

class Fire : public Command {
public:
    void Shoot(const std::unique_ptr<Hero>& hero, core::vec2& cursorPosition) override {}
    ~Fire() override = default;
};

class Input {
public:
    static bool HeroAction(const std::unique_ptr<Hero>& hero);

private:
    static bool quit;
    static SDL_Event gameEvent;
    static int x, y;

    static void GetCursorPosition();
};

#endif //COMMAND_HPP