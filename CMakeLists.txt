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

add_executable(ShapeWars src/main.cpp
        src/Favicon/app.rc
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
        src/Content/Src/Sound.cpp
        src/Content/Include/Sound.h
        src/Content/Src/GaussianBlur.cpp
        src/Content/Include/GaussianBlur.h
        src/Entities/Src/BlackHoles.cpp
        src/Entities/Include/BlackHoles.h
        src/Core/Src/SpawnHelper.cpp
        src/Core/Include/SpawnHelper.h
        src/GameState/UI/Src/FloatingKillTexts.cpp
        src/GameState/UI/Include/FloatingKillTexts.h
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
        src/Input/Src/Buttons.cpp
        src/Input/Include/Buttons.h
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
        src/Entities/Src/ShapeKeeper/LasersBeams.cpp
        src/Entities/Include/ShapeKeeper/LasersBeams.h
        src/Entities/Src/ShapeKeeper/SpriteTrail.cpp
        src/Entities/Include/ShapeKeeper/SpriteTrail.h
        src/GameState/Include/IGameState.h
        src/GameState/Src/StartMenu.cpp
        src/GameState/Include/StartMenu.h
        src/GameState/Src/PauseMenu.cpp
        src/GameState/Include/PauseMenu.h
        src/GameState/Src/GamePlay.cpp
        src/GameState/Include/GamePlay.h
        src/GameRoot.h
        src/GameState/UI/Src/GamePlayHUD.cpp
        src/GameState/UI/Include/GamePlayHUD.h
        src/GameState/UI/Src/GamePlayControlArea.cpp
        src/GameState/UI/Include/GamePlayControlArea.h
        src/GameState/Src/OptionsMenu.cpp
        src/GameState/Include/OptionsMenu.h
        src/GameState/Src/ActiveMenuOptionIndicator.cpp
        src/GameState/Include/ActiveMenuOptionIndicator.h
        src/GameState/Src/HighscoreArea.cpp
        src/GameState/Include/HighscoreArea.h
        src/GameState/Src/MenuOption.cpp
        src/GameState/Include/MenuOption.h
        src/GameState/Src/Background.cpp
        src/GameState/Include/Background.h
)

# Copy data
set(DATA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Content/Data")
set(DATA_DEST_DIR "$<TARGET_FILE_DIR:ShapeWars>/Content/Data")
add_custom_command(
        TARGET ShapeWars POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DATA_DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${DATA_SOURCE_DIR} ${DATA_DEST_DIR}
        COMMENT "Copying data to $<TARGET_FILE_DIR:ShapeWars>/Content/Data"
)

# Copy fonts
set(FONT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Content/Font")
set(FONT_DEST_DIR "$<TARGET_FILE_DIR:ShapeWars>/Content/Font")
add_custom_command(
        TARGET ShapeWars POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${FONT_DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${FONT_SOURCE_DIR} ${FONT_DEST_DIR}
        COMMENT "Copying fonts to $<TARGET_FILE_DIR:ShapeWars>/Content/Font"
)

# Copy images
set(IMAGES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Content/Images")
set(IMAGES_DEST_DIR "$<TARGET_FILE_DIR:ShapeWars>/Content/Images")
add_custom_command(
        TARGET ShapeWars POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${IMAGES_DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${IMAGES_SOURCE_DIR} ${IMAGES_DEST_DIR}
        COMMENT "Copying images to $<TARGET_FILE_DIR:ShapeWars>/Content/Images"
)

# Copy shaders
set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Content/Shaders")
set(SHADER_DEST_DIR "$<TARGET_FILE_DIR:ShapeWars>/Content/Shaders")
add_custom_command(
        TARGET ShapeWars POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_SOURCE_DIR} ${SHADER_DEST_DIR}
        COMMENT "Copying shaders to $<TARGET_FILE_DIR:ShapeWars>/Content/Shaders"
)

# Copy sounds
set(SOUND_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Content/Sound")
set(SOUND_DEST_DIR "$<TARGET_FILE_DIR:ShapeWars>/Content/Sound")
add_custom_command(
        TARGET ShapeWars POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SOUND_DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOUND_SOURCE_DIR} ${SOUND_DEST_DIR}
        COMMENT "Copying sound to $<TARGET_FILE_DIR:ShapeWars>/Content/Sound"
)


target_compile_features(ShapeWars PRIVATE cxx_std_20)

target_link_libraries(
        ShapeWars
        PRIVATE SFML::Graphics
        PRIVATE SFML::Audio
        PRIVATE SFML::Window
        PRIVATE SFML::System
)

## Add linker flags to statically link libgcc and libstdc++
#target_link_options(
#        ShapeWars
#        PRIVATE "-static-libgcc"
#        PRIVATE "-static-libstdc++"
#)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE_BUILD)
endif()
