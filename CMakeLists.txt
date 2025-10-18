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
        src/Core/Src/Logger.cpp
        src/Core/Include/Logger.h
        src/Content/Src/Art.cpp
        src/Content/Include/Art.h
        src/Entities/Src/Player/PlayerShip.cpp
        src/Entities/Include/Player/PlayerShip.h
        src/Input/Src/Input.cpp
        src/Input/Include/Input.h
        src/Core/Include/Extensions.h
        src/Entities/Src/Bullets.cpp
        src/Entities/Include/Bullets.h
        src/Core/Src/Quaternion.cpp
        src/Core/Include/Quaternion.h
        src/Entities/Src/Enemies.cpp
        src/Entities/Include/Enemies.h
        src/Entities/Src/Collisions.cpp
        src/Entities/Include/Collisions.h
        src/Entities/Src/Player/PlayerStatus.cpp
        src/Entities/Include/Player/PlayerStatus.h
        src/UserInterface/Src/UserInterface.cpp
        src/UserInterface/Include/UserInterface.h
        src/Content/Src/Sound.cpp
        src/Content/Include/Sound.h
        src/Content/Src/GaussianBlur.cpp
        src/Content/Include/GaussianBlur.h
        src/Entities/Src/BlackHoles.cpp
        src/Entities/Include/BlackHoles.h
        src/Core/Src/SpawnHelper.cpp
        src/Core/Include/SpawnHelper.h
        src/UserInterface/Src/FloatingKillTexts.cpp
        src/UserInterface/Include/FloatingKillTexts.h
        src/Systems/Src/Particles.cpp
        src/Systems/Include/Particles.h
        src/Core/Src/RandomVector.cpp
        src/Core/Include/RandomVector.h
        src/Core/Src/ColorPicker.cpp
        src/Core/Include/ColorPicker.h
        src/Systems/Src/Grid.cpp
        src/Systems/Include/Grid.h
        src/Core/Src/Extensions.cpp
        src/Entities/Src/Nukes.cpp
        src/Entities/Include/Nukes.h
        src/Input/Src/Xbox.cpp
        src/Input/Include/Xbox.h
        src/Input/Src/Dualsense.cpp
        src/Input/Include/Dualsense.h
        src/Input/Src/Nintendo.cpp
        src/Input/Include/Nintendo.h
        src/Input/Src/MouseAndKeyboard.cpp
        src/Input/Include/MouseAndKeyboard.h
        src/Entities/Src/Player/Buffs.cpp
        src/Entities/Include/Player/Buffs.h
        src/UserInterface/Src/LivesAndNukes.cpp
        src/UserInterface/Include/LivesAndNukes.h
        src/UserInterface/Src/Buttons.cpp
        src/UserInterface/Include/Buttons.h
        src/Entities/Src/ShapeKeeper/ShapeKeeper.cpp
        src/Entities/Include/ShapeKeeper/ShapeKeeper.h
        src/Entities/Src/ShapeKeeper/ShapeKeeperCore.cpp
        src/Entities/Include/ShapeKeeper/ShapeKeeperCore.h
        src/Entities/Src/ShapeKeeper/ShapeKeeperHealthBar.cpp
        src/Entities/Include/ShapeKeeper/ShapeKeeperHealthBar.h
        src/Entities/Src/ShapeKeeper/ShapeKeeperBodyPart.cpp
        src/Entities/Include/ShapeKeeper/ShapeKeeperBodyPart.h
        src/Entities/Src/ShapeKeeper/ShapeKeeperHealthContainer.cpp
        src/Entities/Include/ShapeKeeper/ShapeKeeperHealthContainer.h
        src/Entities/Src/ShapeKeeper/LasersAttack.cpp
        src/Entities/Include/ShapeKeeper/LasersAttack.h
        src/Entities/Src/ShapeKeeper/SpriteTrail.cpp
        src/Entities/Include/ShapeKeeper/SpriteTrail.h
)

target_compile_features(ShapeWars PRIVATE cxx_std_20)

target_link_libraries(
        ShapeWars
        PRIVATE SFML::Graphics
        PRIVATE SFML::Audio
        PRIVATE SFML::Window
        PRIVATE SFML::System
)
