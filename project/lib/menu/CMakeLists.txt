find_package(PkgConfig)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

add_library(menu src/menu.cpp)
target_include_directories(menu PUBLIC inc/)
target_link_libraries(menu sdl2 fps window)