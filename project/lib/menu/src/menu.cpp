#include "menu.hpp"

Menu::Menu(Window* window, int frames) {
    this->window = window;
    this->frames = frames;
    ready = false;
    maxNameLength = 12;
    this->color = {216, 151, 70, 255};
    this->font = window->texture.textFont;
}

void Menu::Load(std::string& name, int& texture_id) {
    FPS fps;
    SDL_FRect rect;

    std::vector<Button> heroIcon(textures_num);
    HeroButtonLoad(heroIcon);

    Button playButton;
    playButton.field = window->imageList[Tile::BUTTON_OFF];
    playButton.selected = window->imageList[Tile::BUTTON_ON];
    playButton.position.x = window->width / 2;
    playButton.position.y = window->height - window->height / 5;
    playButton.text = "PLay";

    fps.Set(frames);
    fps.SetInverseTimer(1);
    while (fps.InverseTimeCheck() > 0) {
        window->ClearSurface();
        window->DrawText("Skirmish", font, color, 150, (float) window->width / 2, (float) window->height / 2);
        window->Render();
    }

    while (!ready) {
        if (fps.Update() >= 1) {

            window->ClearSurface();
            window->DrawText("Skirmish", font, color, 104, (float) window->width / 2, 50);
            window->DrawText("Enter your name and select hero", font, color, 28, (float) window->width / 2, 150);

            GetTextInputFieldPosition(&rect, window->width / 2, 200, 300, 50);
            window->DrawTexture(window->imageList[Tile::HEALTH_LINE], &rect, 0, {});
            if (name.length() > 0)
                window->DrawText(name, font, color, 36, window->width / 2, 200);

            GetTextureRectPosition(&playButton.rect, playButton.field, playButton.position.x, playButton.position.y);
            window->DrawTexture(playButton.field, &playButton.rect, 0, {});
            window->DrawText(playButton.text, font, color,36, playButton.position.x, playButton.position.y);

            for (auto& item: heroIcon) {
                GetTextureRectPosition(&item.rect, item.field, item.position.x, item.position.y);
                window->DrawTexture(item.field, &item.rect, 0, {});

                if (item.status) {
                    window->DrawTexture(item.selected, &item.rect, 0, {});

                    GetTextureRectPosition(&playButton.rect, playButton.selected, playButton.position.x, playButton.position.y);
                    window->DrawTexture(playButton.selected, &playButton.rect, 0, {});
                    window->DrawText(playButton.text, font, color,36, playButton.position.x, playButton.position.y);
                }

                GetTextureRectPosition(&item.rect, item.heroIcon, item.position.x, item.position.y);
                window->DrawTexture(item.heroIcon, &item.rect, 0, {});
                window->DrawText(item.text, font, color,36, item.position.x, item.position.y + 75);
            }

            SDL_StartTextInput();
            while (SDL_PollEvent(&menuEvent)) {
                switch (menuEvent.type) {

                    case SDL_QUIT:
                        exit(EXIT_SUCCESS);
                        break;
                    case SDL_TEXTINPUT:
                        if (name.length() < maxNameLength)
                            name += menuEvent.text.text;
                        break;
                    case SDL_KEYDOWN:
                        if (menuEvent.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && name.length() > 0)
                            name = name.substr(0, name.length() - 1);
                        break;
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
        SDL_StopTextInput();

        for (auto& item: heroIcon) {
            if (item.status) {
                texture_id = item.texture_id;
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

void Menu::GetTextInputFieldPosition(SDL_FRect *rect, float x, float y, float width_, float height_) {
    rect->w = width_;
    rect->h = height_;
    rect->x = x - width_ / 2;
    rect->y = y - height_ / 2;
}

void Menu::HeroButtonLoad(std::vector<Button>& button) {
    int start_x = 0, start_y = window->height / 2;
    for (int i = 0; i < (int) button.size(); i++) {
        start_x += window->width / (textures_num + 1);
        button[i].position.x = start_x,
        button[i].position.y = start_y;
        button[i].field = window->imageList[Tile::ICON_OFF];
        button[i].selected = window->imageList[Tile::ICON_ON];
        button[i].text = window->texture.heroName[i];
        button[i].heroIcon = window->imageList[i];
        button[i].texture_id = i;
    }
}

void Menu::InverseTimer(int second) {
    if (second > 0 && second < 4)
        window->DrawText(std::to_string(second), font, color, 104, (float) window->width / 2, 150);
    else if (second == 0)
        window->DrawText("Go!", font, color, 104, (float) window->width / 2, 150);
}
