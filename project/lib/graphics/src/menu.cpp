#include "menu.hpp"

bool Menu::ready = false;
SDL_Event Menu::menuEvent = {};
SDL_Color Menu::color = {216, 151, 70, 255};
int Menu::iconNumber = 8;
std::string Menu::font = Textures().textureFontMap[Font::OCTIN];

void Menu::Load(Window& window, int fps, std::unique_ptr<Hero>& hero) {
    Textures textures;
    SDL_FRect rect;
    SDL_Texture *texture;

    std::vector<Button> heroIcon(iconNumber);
    HeroButtonLoad(window, heroIcon);

    Button playButton;
    playButton.field = window.imageList[Tile::BUTTON_OFF];
    playButton.selected = window.imageList[Tile::BUTTON_ON];
    playButton.text = Textures::LoadTextTexture(window.renderer,"Play", font, color,36);
    playButton.position.x = (double) Window::width / 2;
    playButton.position.y = 600;

    FPS::Set(fps);
    while (FPS::InverseTimeCheck() > 0) {
        window.ClearSurface();
        texture = Textures::LoadTextTexture(window.renderer,"Skirmish", font, color, 150);
        Menu::GetTextureRectPosition(&rect, texture, (double) Window::width / 2, (double) Window::height / 2);
        window.DrawTexture(texture, &rect);
        window.Render();
    }

    while (!ready) {
        if (FPS::Update() >= 1) {

            window.ClearSurface();

            texture = Textures::LoadTextTexture(window.renderer,"Skirmish", font, color,104);
            GetTextureRectPosition(&rect, texture, (double) Window::width / 2, 50);
            window.DrawTexture(texture, &rect);
            texture = Textures::LoadTextTexture(window.renderer,"Select hero", font, color,28);
            GetTextureRectPosition(&rect, texture, (double) Window::width / 2, 150);
            window.DrawTexture(texture, &rect);


            GetTextureRectPosition(&playButton.fieldRect, playButton.field, playButton.position.x, playButton.position.y);
            window.DrawTexture(playButton.field, &playButton.fieldRect);
            GetTextureRectPosition(&playButton.fieldRect, playButton.text, playButton.position.x, playButton.position.y);
            window.DrawTexture(playButton.text, &playButton.fieldRect);

            for (auto& item: heroIcon) {
                GetTextureRectPosition(&item.fieldRect, item.field, item.position.x, item.position.y);
                window.DrawTexture(item.field, &item.fieldRect);

                if (item.status) {
                    window.DrawTexture(item.selected, &item.fieldRect);

                    GetTextureRectPosition(&playButton.fieldRect, playButton.selected, playButton.position.x, playButton.position.y);
                    window.DrawTexture(playButton.selected, &playButton.fieldRect);
                    GetTextureRectPosition(&playButton.fieldRect, playButton.text, playButton.position.x, playButton.position.y);
                    window.DrawTexture(playButton.text, &playButton.fieldRect);
                }

                GetTextureRectPosition(&item.fieldRect, item.heroIcon, item.position.x, item.position.y);
                window.DrawTexture(item.heroIcon, &item.fieldRect);

                GetTextureRectPosition(&item.fieldRect, item.text, item.position.x, item.position.y + 75);
                window.DrawTexture(item.text, &item.fieldRect);
            }

            while (SDL_PollEvent(&menuEvent)) {
                switch (menuEvent.type) {
                    case SDL_QUIT:
                        exit(EXIT_SUCCESS);

                    case SDL_MOUSEBUTTONDOWN:
                        GetCursorPosition(window, menuEvent.button.x, menuEvent.button.y);
                        double x = menuEvent.button.x;
                        double y = menuEvent.button.y;
                        for (auto& item: heroIcon) {
                            GetTextureRectPosition(&item.fieldRect, item.field, item.position.x, item.position.y);
                            if(x >= item.fieldRect.x && x <= item.fieldRect.x + item.fieldRect.w
                               && y >= item.fieldRect.y  && y <= (item.fieldRect.y + item.fieldRect.h)) {
                                if (item.status) {
                                    item.status = false;
                                    break;
                                }
                                if (!ButtonPressedCheck(heroIcon)) {
                                    item.status = true;
                                    break;
                                }
                                if (ButtonPressedCheck(heroIcon)) {
                                    for (auto& i: heroIcon) {
                                        i.status = false;
                                    }
                                    item.status = true;
                                    break;
                                }
                            }
                        }

                        GetTextureRectPosition(&playButton.fieldRect, playButton.field, playButton.position.x, playButton.position.y);
                        if(x >= playButton.fieldRect.x && x <= playButton.fieldRect.x + playButton.fieldRect.w
                           && y >= playButton.fieldRect.y  && y <= playButton.fieldRect.y + playButton.fieldRect.h &&
                                ButtonPressedCheck(heroIcon))
                            ready = true;
                        break;
                }
            }
            window.Render();
        }
    }

    for (auto& item: heroIcon) {
        if (item.status) {
            hero->heroIcon = item.heroIcon;
            hero->bullet.bulletIcon = item.bullet.bulletIcon;
        }
    }

    window.ClearSurface();
    GetTextureRectPosition(&hero->fieldRect, hero->heroIcon, (double) Window::width / 2, (double) Window::height / 2);
    GetTextureRectPosition(&hero->bullet.fieldRect, hero->bullet.bulletIcon,
                           (double) Window::width / 2, (double) Window::height / 2 - hero->fieldRect.h);
    hero->center = {hero->fieldRect.x, hero->fieldRect.y};
}

void Menu::GetCursorPosition(Window& window, int& x, int& y) {
    int width, height;
    SDL_QueryTexture(window.imageList[Tile::MENU_CURSOR], nullptr, nullptr, &width, &height);
    x += width;
}

bool Menu::ButtonPressedCheck(std::vector<Button>& button) {
    for (auto& item: button) {
        if (item.status)
            return true;
    }
    return false;
}

void Menu::GetTextureRectPosition(SDL_FRect* rect, SDL_Texture* texture, double x, double y) {
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    rect->w = (float) width;
    rect->h = (float) height;
    rect->x = (float) (x - (double) width / 2);
    rect->y = (float) (y - (double) height / 2);
}

void Menu::HeroButtonLoad(Window& window, std::vector<Button>& button) {
    int start_x = 200, start_y = 250;
    for (int i = 0; i < (int) button.size(); i++) {
        button[i].position.x = start_x,
        button[i].position.y = start_y;
        button[i].text = Textures::LoadTextTexture(window.renderer,Font::heroName[i], font, color,24);
        button[i].heroIcon = window.imageList[i];
        button[i].bullet.bulletIcon = window.imageList[i + iconNumber];

        start_x += 200;
        if (i == 3) {
            start_x = 200;
            start_y = 420;
        }
    }
}

void Menu::InverseTimer(Window &window, int second) {
    SDL_Texture* texture;
    SDL_FRect rect;
    if (second > 0 && second < 4) {
        texture = Textures::LoadTextTexture(window.renderer,std::to_string(second), font, color, 104);
        Menu::GetTextureRectPosition(&rect, texture, (double) Window::width / 2, 150);
        window.DrawTexture(texture, &rect);
    } else if (second == 0) {
        texture = Textures::LoadTextTexture(window.renderer,"Go!", font, color, 104);
        Menu::GetTextureRectPosition(&rect, texture, (double) Window::width / 2, 150);
        window.DrawTexture(texture, &rect);
    }
}