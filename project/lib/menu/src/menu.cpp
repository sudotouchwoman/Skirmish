#include "menu.hpp"

Menu::Menu(Window* window, int frames) {
    this->window = window;
    this->frames = frames;
    ready = false;
    color = {216, 151, 70, 255};
    font = Textures().textureFontMap[Font::OCTIN];
}

void Menu::Load(Client::ConnectionClient& cc) {
    FPS fps;
    SDL_FRect rect;
    SDL_Texture *texture;

    std::vector<Button> heroIcon(textures_num);
    HeroButtonLoad(heroIcon);

    Button playButton;
    playButton.field = window->imageList[Tile::BUTTON_OFF];
    playButton.selected = window->imageList[Tile::BUTTON_ON];
    playButton.text = window->textures.LoadTextTexture(window->renderer,"Play", font, color,36);
    playButton.position.x = (float) window->width / 2;
    playButton.position.y = window->height - 100;

    fps.Set(frames);
    fps.SetInverseTimer(1);
    while (fps.InverseTimeCheck() > 0) {
        window->ClearSurface();
        texture = window->textures.LoadTextTexture(window->renderer,"Skirmish", font, color, 150);
        GetTextureRectPosition(&rect, texture, (float) window->width / 2, (float) window->height / 2);
        window->DrawTexture(texture, &rect, 0, {});
        window->Render();
    }

    while (!ready) {
        if (fps.Update() >= 1) {

            window->ClearSurface();
            texture = window->textures.LoadTextTexture(window->renderer,"Skirmish", font, color,104);
            GetTextureRectPosition(&rect, texture, (float) window->width / 2, 50);
            window->DrawTexture(texture, &rect, 0, {});
            texture = window->textures.LoadTextTexture(window->renderer,"Select hero", font, color,28);
            GetTextureRectPosition(&rect, texture, (float) window->width / 2, 150);
            window->DrawTexture(texture, &rect, 0, {});


            GetTextureRectPosition(&playButton.rect, playButton.field, playButton.position.x, playButton.position.y);
            window->DrawTexture(playButton.field, &playButton.rect, 0, {});
            GetTextureRectPosition(&playButton.rect, playButton.text, playButton.position.x, playButton.position.y);
            window->DrawTexture(playButton.text, &playButton.rect, 0, {});

            for (auto& item: heroIcon) {
                GetTextureRectPosition(&item.rect, item.field, item.position.x, item.position.y);
                window->DrawTexture(item.field, &item.rect, 0, {});

                if (item.status) {
                    window->DrawTexture(item.selected, &item.rect, 0, {});

                    GetTextureRectPosition(&playButton.rect, playButton.selected, playButton.position.x, playButton.position.y);
                    window->DrawTexture(playButton.selected, &playButton.rect, 0, {});
                    GetTextureRectPosition(&playButton.rect, playButton.text, playButton.position.x, playButton.position.y);
                    window->DrawTexture(playButton.text, &playButton.rect, 0, {});
                }

                GetTextureRectPosition(&item.rect, item.heroIcon, item.position.x, item.position.y);
                window->DrawTexture(item.heroIcon, &item.rect, 0, {});

                GetTextureRectPosition(&item.rect, item.text, item.position.x, item.position.y + 75);
                window->DrawTexture(item.text, &item.rect, 0, {});
            }

            while (SDL_PollEvent(&menuEvent)) {
                switch (menuEvent.type) {
                    case SDL_QUIT:
                        exit(EXIT_SUCCESS);

                    case SDL_MOUSEBUTTONDOWN:
                        GetCursorPosition(menuEvent.button.x, menuEvent.button.y);
                        int x = menuEvent.button.x;
                        int y = menuEvent.button.y;
                        for (auto& item: heroIcon) {
                            GetTextureRectPosition(&item.rect, item.field, item.position.x, item.position.y);
                            if(x >= item.rect.x && x <= item.rect.x + item.rect.w
                               && y >= item.rect.y  && y <= (item.rect.y + item.rect.h)) {
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

                        GetTextureRectPosition(&playButton.rect, playButton.field, playButton.position.x, playButton.position.y);
                        if(x >= playButton.rect.x && x <= playButton.rect.x + playButton.rect.w
                           && y >= playButton.rect.y  && y <= playButton.rect.y + playButton.rect.h &&
                                ButtonPressedCheck(heroIcon))
                            ready = true;
                        break;
                }
            }
            window->Render();
        }

        //??
        for (auto& item: heroIcon) {
            if (item.status) {
//                hero->heroIcon = item.heroIcon;
//                hero->bullet.bulletIcon = item.bulletIcon;
            }
        }
    }
}

void Menu::GetCursorPosition(int& x, int& y) {
    int width, height;
    SDL_QueryTexture(window->imageList[Tile::MENU_CURSOR], nullptr, nullptr, &width, &height);
    x += width;
}

bool Menu::ButtonPressedCheck(std::vector<Button>& button) {
    for (auto& item: button) {
        if (item.status)
            return true;
    }
    return false;
}

void Menu::GetTextureRectPosition(SDL_FRect* rect, SDL_Texture* texture, float x, float y) {
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    rect->w = (float) width;
    rect->h = (float) height;
    rect->x = x - (float) width / 2;
    rect->y = y - (float) height / 2;
}

void Menu::HeroButtonLoad(std::vector<Button>& button) {
    int start_x = 0, start_y = window->height / 2;
    for (int i = 0; i < (int) button.size(); i++) {
        start_x += window->width / (textures_num + 1);
        button[i].position.x = start_x,
        button[i].position.y = start_y;
        button[i].field = window->imageList[Tile::ICON_OFF];
        button[i].selected = window->imageList[Tile::ICON_ON];
        button[i].text = window->textures.LoadTextTexture(window->renderer,window->textures.heroName[i], font, color,24);
        button[i].heroIcon = window->imageList[i];
    }
}

void Menu::InverseTimer(int second) {
    SDL_Texture* texture;
    SDL_FRect rect;
    if (second > 0 && second < 4) {
        texture = window->textures.LoadTextTexture(window->renderer,std::to_string(second), font, color, 104);
        Menu::GetTextureRectPosition(&rect, texture, (float) window->width / 2, 150);
        window->DrawTexture(texture, &rect, 0, {});
    } else if (second == 0) {
        texture = window->textures.LoadTextTexture(window->renderer,"Go!", font, color, 104);
        Menu::GetTextureRectPosition(&rect, texture, (float) window->width / 2, 150);
        window->DrawTexture(texture, &rect, 0, {});
    }
}
