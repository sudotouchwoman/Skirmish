find_package(PkgConfig)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

add_library(sdl2 INTERFACE)
target_include_directories(sdl2 INTERFACE ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(sdl2 INTERFACE ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

add_library(texture src/texture.cpp)
target_include_directories(texture PUBLIC inc/)
target_link_libraries(texture sdl2 core)

add_library(window src/window.cpp)
target_include_directories(window PUBLIC inc/)
target_link_libraries(window texture)
