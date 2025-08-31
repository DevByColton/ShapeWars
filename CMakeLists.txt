cmake_minimum_required(VERSION 3.28)
project(ShapeWars LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

#file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/Content" DESTINATION "${CMAKE_BINARY_DIR}/bin")

add_executable(ShapeWars src/main.cpp
        src/GameRoot.cpp
        src/GameRoot.h
        src/Logger/Logger.cpp
        src/Logger/Logger.h
        src/Content/Art.cpp
        src/Content/Art.h
        src/Entities/Src/PlayerShip.cpp
        src/Entities/Include/PlayerShip.h
        src/Input/Input.cpp
        src/Input/Input.h
        src/System/Include/Extensions.h
        src/Entities/Src/Bullets.cpp
        src/Entities/Include/Bullets.h
        src/System/Src/Quaternion.cpp
        src/System/Include/Quaternion.h
        src/Entities/Src/Enemies.cpp
        src/Entities/Include/Enemies.h
        src/Entities/Src/Collisions.cpp
        src/Entities/Include/Collisions.h
        src/PlayerStatus/PlayerStatus.cpp
        src/PlayerStatus/PlayerStatus.h
        src/UserInterface/UserInterface.cpp
        src/UserInterface/UserInterface.h
        src/Content/Sound.cpp
        src/Content/Sound.h
)

target_compile_features(ShapeWars PRIVATE cxx_std_20)

target_link_libraries(
        ShapeWars
        PRIVATE SFML::Graphics
        PRIVATE SFML::Audio
        PRIVATE SFML::Window
        PRIVATE SFML::System
)
