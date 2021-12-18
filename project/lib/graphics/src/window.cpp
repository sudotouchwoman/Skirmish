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
    imageList = textures.PreLoadImageTextures(renderer);

    SDL_ShowCursor(SDL_ENABLE);
    SetCursor(textures.textureImageMap[Tile::MENU_CURSOR]);

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

void Window::DrawTexture(SDL_Texture *texture, SDL_FRect* rect, float angle, SDL_FPoint* center) {
    SDL_RenderCopyExF(renderer, texture, nullptr, rect, angle, center, SDL_FLIP_NONE);
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