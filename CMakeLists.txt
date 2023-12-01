cmake_minimum_required(VERSION 3.25)
project(MagicMirror)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(Freetype REQUIRED)

include_directories(MagicMirror ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR})
add_executable(MagicMirror main.cpp
        RenderableText.cpp
        RenderableText.h)

target_link_libraries(MagicMirror PRIVATE
        ${SDL2_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${SDL2TTF_LIBRARY}
        ${FREETYPE_LIBRARIES}
)
