find_package(PkgConfig)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

add_library(sdl2 INTERFACE)
target_include_directories(sdl2 INTERFACE ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(sdl2 INTERFACE ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

add_library(tile src/tile.cpp)
target_include_directories(tile PUBLIC inc/)
target_link_libraries(tile sdl2 core)

add_library(texture src/texture.cpp)
target_include_directories(texture PUBLIC inc/)
target_link_libraries(texture tile)

add_library(graphics src/graphics.cpp)
target_include_directories(graphics PUBLIC inc/)
target_link_libraries(graphics texture)

add_library(hero src/hero.cpp)
target_include_directories(hero PUBLIC inc/)
target_link_libraries(hero graphics)

add_library(menu src/menu.cpp)
target_include_directories(menu PUBLIC inc/)
target_link_libraries(menu fps hero)
