#include "window.hpp"

Window::~Window() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void Window::Init() {
    SDL_DisplayMode displayMode;
    if (!SDL_GetDesktopDisplayMode(0, &displayMode)) {
        std::cerr << "could not initialize display mode: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (SDL_Init(SDL_INIT_EVERYTHING)){
        std::cerr << "could not initialize SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Skirmish", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              width, height, SDL_WINDOW_SHOWN); //SDL_WINDOW_RESIZABLE
    if (window == nullptr) {
        std::cerr << "failed to open the window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "failed to create renderer: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    imageList = PreLoadImageTextures();

    SDL_ShowCursor(SDL_ENABLE);
    SetCursor(texture.textureImageMap[Tile::MENU_CURSOR]);

    if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) {
        std::cerr << "failed to image format initialization: " << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1) {
        std::cerr << "failed to text format initialization: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Window::Render() {
    SDL_RenderPresent(renderer);
}

void Window::ClearSurface() {
    SDL_SetRenderDrawColor(renderer, 192,175,175, 1);
    SDL_RenderClear(renderer);
}

void Window::DrawTexture(SDL_Texture *texture_, SDL_FRect* rect, float angle, SDL_FPoint* center) {
    SDL_RenderCopyExF(renderer, texture_, nullptr, rect, angle, center, SDL_FLIP_NONE);
}

SDL_Texture* Window::LoadImageTexture(const std::string& path) {
    SDL_Texture* texture_;
    SDL_Surface* surface;

    surface = IMG_Load(path.c_str());
    if (surface == nullptr) {
        std::cerr << "failed to load image from path " << path << ": " << IMG_GetError() << std::endl;
        return texture_;
    }

    texture_ = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_ == nullptr) {
        std::cerr << "failed to create texture: " << SDL_GetError() << std::endl;
        return texture_;
    }
    SDL_FreeSurface(surface);
    return texture_;
}

std::map<size_t, SDL_Texture*> Window::PreLoadImageTextures() {
    std::map<size_t, SDL_Texture*> textures;
    for (auto& it : texture.textureImageMap) {
        SDL_Texture* texture_ = LoadImageTexture(it.second);
        size_t index = it.first;
        std::cout << it.second << std::endl;
        textures[index] = texture_;
    }
    return textures;
}

void Window::DrawText(const std::string& text, const std::string& path, SDL_Color color, int fontSize, float x_, float y_) {
    TTF_Font *font = TTF_OpenFont(path.c_str(), fontSize);
    if (font == nullptr) {
        std::cerr << "failed to load font from path " << path << ": " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface == nullptr) {
        TTF_CloseFont(font);
        std::cerr << "failed to create text surface: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_Texture *texture_ = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_ == nullptr) {
        std::cerr << "failed to create text texture: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    SetRectangle(x_, y_, texture_);
    DrawTexture(texture_, &rect, 0, {});
}

void Window::SetCursor(const std::string& path) {
    SDL_Surface* surfaceCursor = IMG_Load(path.c_str());
    SDL_Cursor* cursor = SDL_CreateColorCursor(surfaceCursor, 0, 0);
    if (cursor == nullptr) {
        std::cerr << "failed to create cursor: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(surfaceCursor);
    SDL_SetCursor(cursor);
}

void Window::SetRectangle(float x, float y, SDL_Texture* texture_) {
    int width_, height_;
    SDL_QueryTexture(texture_, nullptr, nullptr, &width_, &height_);
    rect.w = (float) width_;
    rect.h = (float) height_;
    rect.x = x - rect.w / 2;
    rect.y = y - rect.h / 2;
}