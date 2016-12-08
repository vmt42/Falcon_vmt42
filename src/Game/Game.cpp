#include "Game.h"
#include <iostream>
#include <string>
#include <random>
#include "../Errors/Errors.h"
#include "../ResourceManager/ResourceManager.h"
#include "Weapon/RangeWeapon.h"
#include "Weapon/MeleeWeapon.h"

Game::~Game()
{
    for (auto &level : m_levels)
    {
        delete level;
    }

    for (auto &human : m_humans)
    {
        delete human;
    }

    for (auto &enemy : m_enemies)
    {
        delete enemy;
    }
}

void Game::run()
{
    initSystems();
    initLevel();

    Falcon::Music music = m_audioEngine.loadMusic("media/Sounds/mystic_theme.mp3");
    music.play(-1);
    gameLoop();
}

void Game::initSystems()
{
    Falcon::init();

    m_audioEngine.init();

    m_window.create("EC0H", m_screenWidth, m_screenHeight, 0);
    initShaders();
    glewExperimental = GL_TRUE;
    glewInit();

    m_camera.init(m_screenWidth, m_screenHeight);
    m_hudCamera.init(m_screenWidth, m_screenHeight);
    m_hudCamera.setPosition(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));
    m_actorSpriteBatch.init();
    m_decalsSpriteBatch.init();
    m_FPSLimiter.init(9999999999.0f);
    m_uiSpriteBatch.init();
    m_spriteFont = new Falcon::SpriteFont("media/Fonts/pixel_font.ttf", 12);
}

void Game::initLevel()
{
    m_levels.emplace_back(new Level("levels/lvl1.txt"));
    //m_levels.emplace_back(new Level("levels/test.lua"));

    m_player = new Player(m_levels[m_curLVL]->getPlayerStartPos(), 5.0f, &m_inputManager, &m_camera, &m_bullets, &m_slashes);
    m_humans.emplace_back(m_player);

    std::mt19937 RNG;
    std::uniform_int_distribution<int> randomXPos(2, m_levels[m_curLVL]->getWidth() - 2);
    std::uniform_int_distribution<int> randomYPos(2, m_levels[m_curLVL]->getHeight() - 2);


    for (auto i = 0; i < 200; i++)
    {
        glm::vec2 pos(randomXPos(RNG) * TILE_WIDTH, randomYPos(RNG) * TILE_HEIGHT);
        m_humans.emplace_back(new Human(pos, 0.5f));
    }

    for (auto &humanStartPos : m_levels[m_curLVL]->getHumanStartPos())
    {
        m_humans.emplace_back(new Human(humanStartPos, 0.0f));
    }

    for (auto &enemyStartPos : m_levels[m_curLVL]->getEnemyStartPos())
    {
        m_enemies.emplace_back(new Enemy(enemyStartPos, 1.0f));
    }

    m_player->addRangeWeapon(new RangeWeapon(10, 30.0f, 20.0f, 1.0f, 5.0f, m_audioEngine.loadSoundEffect("media/Sounds/shoot.ogg")));
    m_player->addRangeWeapon(new RangeWeapon(60, 4.0f, 20.0f, 20.0f, 40.0f, m_audioEngine.loadSoundEffect("media/Sounds/shoot.ogg")));
    m_player->addRangeWeapon(new RangeWeapon(1, 4.0f, 20.0f, 1, 20.0f, m_audioEngine.loadSoundEffect("media/Sounds/shoot.ogg")));
    m_player->addMeleeWeapon(new MeleeWeapon(20, 3.0f));
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
    const int MAX_LOGIC_STEPS = 6;
    const float TARGETED_FPS = 60.0f;
    const float MS_PER_SECOND = 1000.0f;
    const float DESIRED_FRAMETIME = MS_PER_SECOND / TARGETED_FPS;
    const float MAX_DELTA_TIME = 1.0f;

    float prevTicks = SDL_GetTicks();

    while (m_currentState != Falcon::StateType::EXIT)
    {
        m_FPSLimiter.begin();

        float newTicks = SDL_GetTicks();
        float frameTime = newTicks - prevTicks;
        prevTicks = newTicks;
        float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

        processInput();

        m_camera.setPosition(m_player->getPosition());

        m_camera.update();
        m_hudCamera.update();

        int i = 0;
        while (totalDeltaTime > 0.0f && i < MAX_LOGIC_STEPS)
        {
            float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
            updateActors(deltaTime);
            updateBullets(deltaTime);
            totalDeltaTime -= deltaTime;
            i++;
        }


        m_time += 0.01;
        draw();

        m_FPS = m_FPSLimiter.end();
        //std::cout << m_FPS << "\n";
    }
}

void Game::updateActors(float deltaTime)
{
    //Update humans
    for (auto &human : m_humans)
    {
        human->update(m_levels[m_curLVL]->getLevelData(), m_humans, m_enemies, deltaTime);
    }

    //Update human collisions
    for (auto i = 0; i < m_humans.size(); i++)
    {
        for (int j = i + 1; j < m_humans.size(); j++)
        {
            m_humans[i]->actorCollision(m_humans[j]);
        }
    }

    //Update enemies
    for (auto &enemy : m_enemies)
    {
        enemy->update(m_levels[m_curLVL]->getLevelData(), m_humans, m_enemies, deltaTime);
    }

    //Update enemy collisions
    for (auto i = 0; i < m_enemies.size(); i++)
    {
        //Collide with other enemies
        for (auto j = i + 1; j < m_enemies.size(); j++)
        {
            m_enemies[i]->actorCollision(m_enemies[j]);
        }

        //Colide with humans
        for (auto j = 1; j < m_humans.size();)
        {
            if (m_enemies[i]->actorCollision(m_humans[j]))
            {
                m_enemies.emplace_back(new Enemy(m_humans[j]->getPosition(), 1.0f));
                delete m_humans[j];
                m_humans.erase(m_humans.begin() + j);
            }
            else
            {
                j++;
            }
        }

        //Collide with player
        /*if (m_enemies[i]->actorCollision(m_player))
            Falcon::fatalError("END OF GAME");*/
    }

}
void Game::updateBullets(float deltaTime)
{
    // Collide slashes with actors
    for (auto i = 0; i < m_slashes.size(); i++)
    {
        for (auto j = 0; j < m_enemies.size();)
        {
            if (m_slashes[i].actorCollision(m_enemies[j]))
            {

                m_bloodDecals.emplace_back(m_enemies[j]->getPosition());
                if (m_enemies[j]->addDamage(m_slashes[i].getDamage()))
                {
                    delete m_enemies[j];
                    m_enemies.erase(m_enemies.begin() + j);
                    m_player->addPoints(10);
                }
                else
                {
                    j++;
                }
            }
            else
            {
                j++;
            }
        }
    }

    // Update slashes
    for (auto i = 0; i < m_slashes.size();)
    {
        if(m_slashes[i].update(deltaTime))
        {
            m_slashes.erase(m_slashes.begin() + i);
        }
        else
        {
            i++;
        }
    }

    // Update and collide with world
    for (auto i = 0; i < m_bullets.size();)
    {
        // If true, collision with wall
        if (m_bullets[i].update(m_levels[m_curLVL]->getLevelData(), deltaTime))
        {
            m_bullets.erase(m_bullets.begin() + i);
        }
        else
        {
            i++;
        }
    }

    // Collide bullets with actors TODO: MAKE JUST ONE ACTORS VECTOR OR MAKE INTERFACES LIKE Drawable and etc.
    for (auto i = 0; i < m_bullets.size(); i++)
    {
        for (auto j = 0; j < m_enemies.size();)
        {
            // Check collision
            if (m_bullets[i].actorCollision(m_enemies[j]))
            {
                // Add blood decal
                m_bloodDecals.emplace_back(m_enemies[j]->getPosition());
                // Returns true, when enemy is dead
                if (m_enemies[j]->addDamage(m_bullets[i].getDamage()))
                {
                    delete m_enemies[j];
                    m_enemies.erase(m_enemies.begin() + j);
                    m_player->addPoints(10);
                }
                else
                {
                    j++;
                }
                // Remove bullet
                m_bullets.erase(m_bullets.begin() + i);
                i--;
                break;
            }
            else
            {
                j++;
            }
        }
    }
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

    //Draw all decals
    m_decalsSpriteBatch.begin();
    for (auto &decal : m_bloodDecals)
    {
        decal.draw(m_decalsSpriteBatch);
    }
    m_decalsSpriteBatch.end();
    m_decalsSpriteBatch.renderBatch();

    //Begin drawing actors
    m_actorSpriteBatch.begin();
    const glm::vec2 agentDims(ACTOR_RADIUS * 2);

    for (auto &human : m_humans)
    {
        if (m_camera.isBoxVisible(human->getPosition(), agentDims))
        {
            human->draw(m_actorSpriteBatch);
        }

    }

    for (auto &enemy : m_enemies)
    {
        if (m_camera.isBoxVisible(enemy->getPosition(), agentDims))
        {
            enemy->draw(m_actorSpriteBatch);
        }
    }

    for (auto& bullet : m_bullets)
    {
        bullet.draw(m_actorSpriteBatch);
    }

    for (auto& slash : m_slashes)
    {
        slash.draw(m_actorSpriteBatch);
    }

    m_actorSpriteBatch.end();
    m_actorSpriteBatch.renderBatch();

    drawHUD();
    m_shaderProgram.use();
    m_window.swapBuffer(); // Swap buffer

}

void Game::drawHUD()
{
    char buffer[256];
    // Set camera matrix
    GLint pLocation = m_shaderProgram.getUniformLocation("P");
    glm::mat4 cameraMatrix = m_hudCamera.getCameraMatrix();
    glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
    m_uiSpriteBatch.begin();

    sprintf(buffer, "POINTS: %d", m_player->getPoints());
    m_spriteFont->draw(m_uiSpriteBatch, buffer, glm::vec2(0, 0),
                       glm::vec2(2.0f), 0.0f, Falcon::Color(255, 255, 255, 255));
    m_spriteFont->draw(m_uiSpriteBatch, std::to_string(m_FPS).c_str(), glm::vec2(0, m_screenHeight - 32),
                       glm::vec2(2.0f), 0.0f, Falcon::Color(255, 255, 255, 255));

    m_uiSpriteBatch.end();
    m_uiSpriteBatch.renderBatch();
}


