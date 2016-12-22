cmake_minimum_required(VERSION 3.5)
project(FalconLatinum)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${FalconLatinum_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
INCLUDE(FindSDL2 REQUIRED)
INCLUDE(FindOpenGL REQUIRED)
INCLUDE(FindSDL2_ttf REQUIRED)
INCLUDE(FindSDL2_mixer REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(GLM REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS}
        $(SDL2_MIXER_INCLUDE_DIR)} ${FalconLatinum_SOURCE_DIR}/source/headers ${FalconLatinum_SOURCE_DIR}/src/Shaders)

set(SOURCE_FILES
        src/MonsterGame/Game.cpp
        src/MonsterGame/Game.h
        #src/MonsterGame/main.cpp
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
        src/MonsterGame/Actor/Actor.cpp
        src/MonsterGame/Actor/Actor.h
        src/MonsterGame/Actor/Human.cpp
        src/MonsterGame/Actor/Human.h
        src/MonsterGame/Actor/Enemy.cpp
        src/MonsterGame/Actor/Enemy.h
        src/MonsterGame/Level/Level.cpp
        src/MonsterGame/Level/Level.h
        src/MonsterGame/Actor/Player.cpp
        src/MonsterGame/Actor/Player.h
        src/MonsterGame/Game.cpp
        src/MonsterGame/Game.h
        src/MonsterGame/Weapon/Weapon.cpp
        src/MonsterGame/Weapon/Weapon.h
        src/MonsterGame/Weapon/RangeWeapon.cpp
        src/MonsterGame/Weapon/RangeWeapon.h
        src/MonsterGame/Weapon/Bullet.cpp
        src/MonsterGame/Weapon/Bullet.h
        src/MonsterGame/Weapon/MeleeWeapon.cpp
        src/MonsterGame/Weapon/MeleeWeapon.h
        src/MonsterGame/Weapon/Slash.cpp
        src/MonsterGame/Weapon/Slash.h
        src/MonsterGame/Decals/Blood.cpp
        src/MonsterGame/Decals/Blood.h
        src/2D/SpriteFont/SpriteFont.cpp
        src/2D/SpriteFont/SpriteFont.h
        src/Audio/AudioEngine.cpp
        src/Audio/AudioEngine.h
        src/2D/Particles/ParticleSystem.cpp
        src/2D/Particles/ParticleSystem.h
        src/2D/Particles/ParticleBatch.cpp
        src/2D/Particles/ParticleBatch.h
        src/SpatialPartitioningTest/MainGame.cpp
        src/SpatialPartitioningTest/MainGame.h
        src/SpatialPartitioningTest/Ball.cpp
        src/SpatialPartitioningTest/Ball.h
        src/SpatialPartitioningTest/BallController.cpp
        src/SpatialPartitioningTest/BallController.h
        src/SpatialPartitioningTest/BallRenderer.cpp
        src/SpatialPartitioningTest/BallRenderer.h
        src/SpatialPartitioningTest/Grid.cpp
        src/SpatialPartitioningTest/Grid.h
        src/SpatialPartitioningTest/main.cpp
        )

add_executable(FalconLatinum ${SOURCE_FILES})
target_link_libraries(FalconLatinum ${SDL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}
        ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARY})
