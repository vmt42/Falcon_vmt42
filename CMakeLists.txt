cmake_minimum_required(VERSION 3.5)
project(FalconLatinum)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${FalconLatinum_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
INCLUDE(FindSDL2 REQUIRED)
INCLUDE(FindOpenGL REQUIRED)
INCLUDE(FindSDL2_ttf REQUIRED)
INCLUDE(FindSDL2_mixer REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(GLM REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS}
        $(SDL2_MIXER_INCLUDE_DIR)} ${FalconLatinum_SOURCE_DIR}/source/headers ${FalconLatinum_SOURCE_DIR}/src/Shaders)

set(SOURCE_FILES
    src/Game/Game.cpp
    src/Game/Game.h
    src/Game/main.cpp
    src/States/StateManager.cpp
    src/States/StateManager.h
    src/2D/Sprite.cpp
    src/2D/Sprite.h
    src/GLSL/ShaderProgram.cpp
    src/GLSL/ShaderProgram.h
    src/Errors/Errors.h
    src/Errors/Errors.cpp
    src/2D/Vertex.h
    src/InputOutput/IOManager.cpp
    src/InputOutput/IOManager.h
    src/2D/picoPNG.h
    src/2D/picoPNG.cpp
    src/2D/GLTexture.h
    src/2D/ImageLoader/ImageLoader.cpp
    src/2D/ImageLoader/ImageLoader.h
    src/ResourceManager/ResourceManager.cpp
    src/ResourceManager/ResourceManager.h
    src/ResourceManager/TextureCache.cpp
    src/ResourceManager/TextureCache.h
    src/Window/Window.cpp
    src/Window/Window.h
    src/Init/Falcon.cpp
    src/Init/Falcon.h
    src/2D/Camera/Camera2D.cpp
    src/2D/Camera/Camera2D.h
    src/2D/SpriteBatch/SpriteBatch.cpp
    src/2D/SpriteBatch/SpriteBatch.h
    src/InputOutput/InputManager.cpp
    src/InputOutput/InputManager.h
    src/Timing/Timing.cpp
    src/Timing/Timing.h
    src/Object/Projectile.cpp
    src/Object/Projectile.h
    src/Game/Actor/Actor.cpp
    src/Game/Actor/Actor.h
    src/Game/Actor/Human.cpp
    src/Game/Actor/Human.h
    src/Game/Actor/Enemy.cpp
    src/Game/Actor/Enemy.h
    src/Game/Level/Level.cpp
    src/Game/Level/Level.h
    src/Game/Actor/Player.cpp
    src/Game/Actor/Player.h
    src/Game/Game.cpp
    src/Game/Game.h
    src/Game/Weapon/Weapon.cpp
    src/Game/Weapon/Weapon.h
    src/Game/Weapon/RangeWeapon.cpp
    src/Game/Weapon/RangeWeapon.h
    src/Game/Weapon/Bullet.cpp
    src/Game/Weapon/Bullet.h
    src/Game/Weapon/MeleeWeapon.cpp
    src/Game/Weapon/MeleeWeapon.h
    src/Game/Weapon/Slash.cpp
    src/Game/Weapon/Slash.h
    src/Game/Decals/Blood.cpp
    src/Game/Decals/Blood.h src/2D/SpriteFont/SpriteFont.cpp src/2D/SpriteFont/SpriteFont.h src/Audio/AudioEngine.cpp src/Audio/AudioEngine.h)

add_executable(FalconLatinum ${SOURCE_FILES})
target_link_libraries(FalconLatinum ${SDL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}
        ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARY})
