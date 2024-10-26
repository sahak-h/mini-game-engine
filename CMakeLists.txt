cmake_minimum_required(VERSION 3.5.0)
project(game VERSION 0.1.0 LANGUAGES C CXX)

set(SDL2_DIR "C:/SDL2/lib/cmake/SDL2")
set(SDL2_IMAGE_DIR "C:/SDL2/lib/cmake/SDL2_image")
set(SDL2_TTF_DIR "C:/SDL2/lib/cmake/SDL2_ttf")
set(SDL2_MIXER_DIR "C:/SDL2/lib/cmake/SDL2_mixer")

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)

add_executable(game src/main.cpp src/gameobject.cpp src/tileset.cpp)

target_include_directories(game PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)

target_link_libraries(game PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)