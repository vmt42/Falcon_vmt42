#include "Game.h"
#include <iostream>
#include <string>
#include "../Errors/Errors.h"
#include "../ResourceManager/ResourceManager.h"


void Game::run()
{
    initSystems();

    gameLoop();
}

void Game::initSystems()
{
    Falcon::init();

    m_window.create("Engine", m_screenWidth, m_screenHeight, 0);

    initShaders();
    glewExperimental = GL_TRUE;
    glewInit();

    m_spriteBatch.init();
    m_FPSLimiter.init(60.0f);
}

void Game::initShaders()
{
    m_shaderProgram.compileShaders("src/Shaders/colorShading.vert", "src/Shaders/colorShading.frag");
    m_shaderProgram.addAttribute("vertexPosition");
    m_shaderProgram.addAttribute("vertexColor");
    m_shaderProgram.addAttribute("vertexUV");
    m_shaderProgram.linkShaders();
}

void Game::processInput()
{
    const float CAMERA_SPEED = 10.0f;
    const float SCALE_SPEED = 0.1f;

    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_QUIT:
                m_currentState = Falcon::StateType::EXIT;
                break;
            case SDL_KEYDOWN:
                m_inputManager.pressKey(e.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.releaseKey(e.key.keysym.sym);
            case SDL_MOUSEBUTTONDOWN:
                m_inputManager.pressKey(e.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.releaseKey(e.button.button);
            case SDL_MOUSEMOTION:
                m_inputManager.setMouseCoords(e.motion.x, e.motion.y);
            default:
                break;
        }
    }

    if (m_inputManager.isKeyPressed(SDLK_w))
        m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));

    if (m_inputManager.isKeyPressed(SDLK_s))
        m_camera.setPosition(m_camera.getPosition() - glm::vec2(0.0f, CAMERA_SPEED));

    if (m_inputManager.isKeyPressed(SDLK_a))
        m_camera.setPosition(m_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));

    if (m_inputManager.isKeyPressed(SDLK_d))
        m_camera.setPosition(m_camera.getPosition() - glm::vec2(CAMERA_SPEED, 0.0f));

    if (m_inputManager.isKeyPressed(SDLK_q))
        m_camera.setScale(m_camera.getScale() + SCALE_SPEED);

    if (m_inputManager.isKeyPressed(SDLK_e))
        m_camera.setScale(m_camera.getScale() - SCALE_SPEED);

    if (m_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
    {
        glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
        mouseCoords = m_camera.screenToWorld(mouseCoords);

        glm::vec2 playerPos(0.0f);
        glm::vec2 direction = mouseCoords - playerPos;
        direction = glm::normalize(direction);

        m_projectiles.emplace_back(Falcon::Projectile(playerPos, direction, 10, 1000));
    }

}

void Game::gameLoop()
{
    while (m_currentState != Falcon::StateType::EXIT)
    {
        m_FPSLimiter.begin();
        processInput();
        m_time += 0.01;

        m_camera.update();

        for (auto &projectile : m_projectiles)
        {
            if(projectile.update())
            {
                projectile = m_projectiles.back();
                m_projectiles.pop_back();
            }
        }

        draw();

        m_FPS = m_FPSLimiter.end();
    }
}

void Game::draw()
{
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color buffer and depth buffer

    m_shaderProgram.use();
    glActiveTexture(GL_TEXTURE0);

    GLint textureLocation = m_shaderProgram.getUniformLocation("sampler");
    glUniform1i(textureLocation, 0);

    GLint timeLocation = m_shaderProgram.getUniformLocation("time");
    glUniform1f(timeLocation, m_time);

    // Set camera matrix
    GLint pLocation = m_shaderProgram.getUniformLocation("P");
    glm::mat4 cameraMatrix = m_camera.getCameraMatrix();

    glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

    m_spriteBatch.begin();

    glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    static Falcon::GLTexture texture = Falcon::ResourceManager::getTexture("images/Textures/Enemy_Mushroom1.png");
    Falcon::Color color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;
    m_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);
    //m_spriteBatch.draw(pos + glm::vec4(50, 0, 0, 0), uv, texture.id, 0.0f, color);

    for (auto &projectile : m_projectiles)
    {
        projectile.draw(m_spriteBatch);
    }

    m_spriteBatch.end();

    m_spriteBatch.renderBatch();

    glBindTexture(GL_TEXTURE_2D, 0);

    m_shaderProgram.unuse();

    m_window.swapBuffer(); // Swap buffer

}


