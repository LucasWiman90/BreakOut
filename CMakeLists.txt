cmake_minimum_required(VERSION 3.22)
project(Breakout LANGUAGES CXX)

#Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#These options below should not be used if you have already
#installed SFML via package manager, for anyone else not using these
#see https://github.com/SFML/cmake-sfml-project/blob/master/CMakeLists.txt
#option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

#include(FetchContent)
#FetchContent_Declare(SFML
#    GIT_REPOSITORY https://github.com/SFML/SFML.git
#    GIT_TAG 2.6.x
#    GIT_SHALLOW ON)
#FetchContent_MakeAvailable(SFML)

add_executable(breakout 
    src/main.cpp 
    src/background.cpp
    src/game.cpp
    src/interactions.cpp
    src/ball.cpp
    src/powerup.cpp
    src/brick.cpp
    src/entity.cpp
    src/paddle.cpp)

target_include_directories(breakout PRIVATE include sprites)
target_link_libraries(breakout PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_features(breakout PRIVATE cxx_std_17)