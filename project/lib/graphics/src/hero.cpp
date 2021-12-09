#include "hero.hpp"

void Hero::PropertiesUpdate() {
    center = {fieldRect.x, fieldRect.y};
    rotation = 0;
}

void Hero::Render() {
    Window::Instance().DrawHeroTexture(heroIcon, &fieldRect, rotation, &center);
    PropertiesUpdate();
}