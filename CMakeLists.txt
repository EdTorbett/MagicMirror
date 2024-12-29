cmake_minimum_required(VERSION 3.25)
project(MagicMirror)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(Freetype REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Restclient REQUIRED)
find_package(dlib REQUIRED)
find_package(GStreamer REQUIRED COMPONENTS base)
find_package(GStreamerPluginsBase COMPONENTS app audio video pbutils)
find_package(GLIB2)
find_package(GObject)

include_directories(MagicMirror
        ${SDL2_INCLUDE_DIRS}
        ${SDL2TTF_INCLUDE_DIR}
        ${Restclient_INCLUDE_DIR}
        ${GSTREAMER_INCLUDE_DIRS}
        ${GLIB2_INCLUDE_DIR}
        ${GOBJECT_INCLUDE_DIR}
        ${GSTREAMER_APP_INCLUDE_DIR}
)

add_executable(MagicMirror main.cpp
        Renderable.cpp
        Renderable.h
        RenderableText.cpp
        RenderableText.h
        HomeAssistant.cpp
        HomeAssistant.h
        CalendarEntry.cpp
        CalendarEntry.h
        WeatherEntry.cpp
        WeatherEntry.h
        Calendar.cpp
        Calendar.h
        Forecast.cpp
        Forecast.h
        Proximity.cpp
        Proximity.h
        Line.cpp
        Line.h
        RenderableVideo.cpp
        RenderableVideo.h
)

target_link_libraries(MagicMirror PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2TTF_LIBRARY}
        ${FREETYPE_LIBRARIES}
        ${Restclient_LIBRARY}
        ${GSTREAMER_LIBRARY}
        ${GLIB2_LIBRARIES}
        ${GOBJECT_LIBRARIES}
        ${GSTREAMER_APP_LIBRARY}
        nlohmann_json::nlohmann_json
        dlib::dlib
)
