cmake_minimum_required(VERSION 3.16)

project(PoliceCatchThief)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

include_directories(include)

add_executable(PoliceCatchThief
    src/main.cpp
    src/Game.cpp
    src/Route.cpp
    src/Character.cpp
    src/Police.cpp
    src/Thief.cpp
    src/TypingManager.cpp
)

target_link_libraries(PoliceCatchThief
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)