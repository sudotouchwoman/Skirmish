#include "manager.hpp"

const int Manager::fps = 60;

void Manager::Run() {
    Window& window = Window::Instance();
    std::unique_ptr<Hero> hero(new Hero);
    FPS::SetInverseTimer(5);
    Menu::Load(window,fps, hero);

    FPS::Set(fps);
    FPS::SetInverseTimer(5);
    Window::SetCursor(window.textures.textureImageMap[Tile::GAME_CURSOR]);
    while (!Input::HeroAction(hero)) {
        if (FPS::Update() >= 1) {
            window.ClearSurface();
            Menu::InverseTimer(window, FPS::InverseTimeCheck());

            hero->Render();
            window.Render();
            FPS::Release();
        }
    }
}
