cmake_minimum_required(VERSION 3.5)

project(GpacmanClient LANGUAGES CXX)

set(GFILES gamemodel.cpp gamemodel.hpp gamerender.cpp gamerender.hpp gamecontroller.cpp gamecontroller.hpp gamenetclient.cpp gamenetclient.hpp gameplaymessage.cpp gameplaymessage.hpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -D LOGGING_LEVEL_DEBUG -pthread")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(GpacmanClient main.cpp assets.cpp assets.hpp ${GFILES})

configure_file(fonts/Roboto-Regular.ttf roboto.ttf COPYONLY)
configure_file(POL-star-way-short.wav audio.wav COPYONLY)

target_link_libraries(GpacmanClient sfml-audio sfml-graphics)
