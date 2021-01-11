cmake_minimum_required(VERSION 3.5)

project(gpacman_client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES assets.cpp gamemodel.cpp gamerender.cpp gamecontroller.cpp gamenetclient.cpp gameplaymessage.cpp)
set(HEADERS assets.hpp gamecontroller.hpp gamemodel.hpp gamerender.hpp gamenetclient.hpp gameplaymessage.hpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -D LOGGING_LEVEL_DEBUG -pthread")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS program_options thread REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

list(APPEND CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules")

find_package(SFML 2.4 COMPONENTS graphics audio REQUIRED)

add_executable(gpacman_client main.cpp ${SOURCES} ${HEADERS})

configure_file(fonts/Roboto-Regular.ttf roboto.ttf COPYONLY)
configure_file(POL-star-way-short.wav audio.wav COPYONLY)

configure_file(README.md README.md COPYONLY)

target_include_directories (gpacman_client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gpacman_client sfml-audio sfml-graphics ${Boost_LIBRARIES})
