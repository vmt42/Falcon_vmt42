#include "Game.h"
#include <iostream>
#include <string>
#include <random>
#include "../Errors/Errors.h"
#include "../ResourceManager/ResourceManager.h"
#include "Weapon/RangeWeapon.h"


Game::~Game()
{
    for (auto &level : m_levels)
        delete level;

    for (auto &human : m_humans)
        delete human;

    for (auto &enemy : m_enemies)
        delete enemy;
}

void Game::run()
{
    initSystems();
    initLevel();
    gameLoop();
}

void Game::initSystems()
{
    Falcon::init();

    m_window.create("Simple Game", m_screenWidth, m_screenHeight, 0);
    initShaders();
    glewExperimental = GL_TRUE;
    glewInit();


    m_camera.init(m_screenWidth, m_screenHeight);
    m_actorSpriteBatch.init();
    m_FPSLimiter.init(60.0f);
}

void Game::initLevel()
{
    m_levels.push_back(new Level("levels/lvl1.txt"));

    m_levels[m_curLVL]->init();

    m_player = new Player();
    m_player->init(m_levels[m_curLVL]->getPlayerStartPos(), 5.0f, &m_inputManager, &m_camera, &m_bullets);
    m_humans.emplace_back(m_player);

    std::cout << (int)m_levels[m_curLVL]->getWidth() - 1 << std::endl;
    std::cout << (int)m_levels[m_curLVL]->getHeight() - 1 << std::endl;
    std::mt19937 RNG;
    std::uniform_int_distribution<int> randomXPos(2, (int)m_levels[m_curLVL]->getWidth() - 2);
    std::uniform_int_distribution<int> randomYPos(2, (int)m_levels[m_curLVL]->getHeight() - 2);


    for (int i = 0; i < 100; i++)
    {
        m_humans.emplace_back(new Human());
        glm::vec2 pos(randomXPos(RNG) * TILE_WIDTH, randomYPos(RNG) * TILE_HEIGHT);
        m_humans.back()->init(pos, 0.5f);
    }

    for (auto &humanStartPos : m_levels[m_curLVL]->getHumanStartPos())
    {
        m_humans.emplace_back(new Human());
        m_humans.back()->init(humanStartPos, 0.0f);
    }

    for (auto &enemyStartPos : m_levels[m_curLVL]->getEnemyStartPos())
    {
        m_enemies.emplace_back(new Enemy());
        m_enemies.back()->init(enemyStartPos, 1.0f);
    }

    m_player->addRangeWeapon(new RangeWeapon(30, 30.0f, 20.0f, 1.0f, 10.0f));
    m_player->addRangeWeapon(new RangeWeapon(60, 4.0f, 20.0f, 20.0f, 40.0f));
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
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_inputManager.pressKey(e.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.releaseKey(e.button.button);
                break;
            case SDL_MOUSEMOTION:
                m_inputManager.setMouseCoords(e.motion.x, e.motion.y);
                break;
            default:
                break;
        }
    }
}

void Game::gameLoop()
{
    while (m_currentState != Falcon::StateType::EXIT)
    {
        m_FPSLimiter.begin();
        processInput();

        m_camera.setPosition(m_player->getPosition());
        m_camera.update();


        updateActors();
        updateBullets();

        m_time += 0.01;
        draw();

        m_FPS = m_FPSLimiter.end();
    }
}

void Game::updateActors()
{
    //Update humans
    for (auto &human : m_humans)
    {
        human->update(m_levels[m_curLVL]->getLevelData(), m_humans, m_enemies);
    }

    //Update human collisions
    for (int i = 0; i < m_humans.size(); i++)
    {
        for (int j = i + 1; j < m_humans.size(); j++)
        {
            m_humans[i]->actorCollision(m_humans[j]);
        }
    }

    //Update enemies
    for (auto &enemy : m_enemies)
    {
        enemy->update(m_levels[m_curLVL]->getLevelData(), m_humans, m_enemies);
    }

    //Update enemy collisions
    for (int i = 0; i < m_enemies.size(); i++)
    {
        //Collide with other enemies
        for (int j = i + 1; j < m_enemies.size(); j++)
        {
            m_enemies[i]->actorCollision(m_enemies[j]);
        }
        //Colide with humans
        for (int j = 1; j < m_humans.size(); j++)
        {
            if (m_enemies[i]->actorCollision(m_humans[j]))
            {
                m_enemies.emplace_back(new Enemy());
                m_enemies.back()->init(m_humans[j]->getPosition(), 1.0f);
                delete m_humans[j];
                m_humans[j] = m_humans.back();
                m_humans.pop_back();
            }
        }

        //Collide with player
        if (m_enemies[i]->actorCollision(m_player))
            Falcon::fatalError("END OF GAME");
    }

}
void Game::updateBullets()
{
    for (auto& bullet : m_bullets)
        bullet.update(m_humans, m_enemies);
}

void Game::draw()
{
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color buffer and depth buffer
    glActiveTexture(GL_TEXTURE0);
    m_shaderProgram.use();

    GLint textureLocation = m_shaderProgram.getUniformLocation("sampler");
    glUniform1i(textureLocation, 0);

    GLint timeLocation = m_shaderProgram.getUniformLocation("time");
    glUniform1f(timeLocation, m_time);

    // Set camera matrix
    GLint pLocation = m_shaderProgram.getUniformLocation("P");
    glm::mat4 cameraMatrix = m_camera.getCameraMatrix();

    glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

    //Draw level
    m_levels[m_curLVL]->draw();

    //Begin drawing actors
    m_actorSpriteBatch.begin();

    for (auto &human : m_humans)
        human->draw(m_actorSpriteBatch);

    for (auto &enemy : m_enemies)
        enemy->draw(m_actorSpriteBatch);

    for (auto& bullet : m_bullets)
        bullet.draw(m_actorSpriteBatch);

    m_actorSpriteBatch.end();
    m_actorSpriteBatch.renderBatch();

    m_shaderProgram.use();
    m_window.swapBuffer(); // Swap buffer

}


