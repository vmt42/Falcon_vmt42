#include <GL/glew.h>
#include "IMainGame.h"
#include "../Timing/Timing.h"
#include "IGameScreen.h"
#include "ScreenList.h"
#include "../Init/Falcon.h"

namespace Falcon
{
    IMainGame::IMainGame()
    {
        m_screenList = std::make_unique<ScreenList>(this);
    }

    IMainGame::~IMainGame()
    {

    }

    void IMainGame::run()
    {
        if(!init())
            return;
        const int MAX_LOGIC_STEPS = 6;
        const float TARGETED_FPS = 60.0f;
        const float MS_PER_SECOND = 1000.0f;
        const float DESIRED_FRAMETIME = MS_PER_SECOND / TARGETED_FPS;
        const float MAX_DELTA_TIME = 1.0f;

        float prevTicks = SDL_GetTicks();

        FPSLimiter limiter;
        limiter.setTargetFPS(999.0f);
        m_isRunning = true;
        while (m_isRunning)
        {
            limiter.begin();
            float newTicks = SDL_GetTicks();
            float frameTime = newTicks - prevTicks;
            prevTicks = newTicks;
            float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

            inputManager.update();


            if (m_isRunning)
            {
                int i = 0;
                while (totalDeltaTime > 0.0f && i < MAX_LOGIC_STEPS)
                {
                    float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

                    update(deltaTime);
                    draw(deltaTime);
                    totalDeltaTime -= deltaTime;
                    i++;
                }


                m_FPS = limiter.end();
                m_window.swapBuffer();
            }
        }
    }

    void IMainGame::exitGame()
    {
        m_currentScreen->onExit();
        if (m_screenList)
        {
            m_screenList->destroy();
            m_screenList.reset();
        }
        m_isRunning = false;
    }

    bool IMainGame::init()
    {
        Falcon::init();

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

        if(!initSystems())
            return false;

        onInit();
        addScreens();

        m_currentScreen = m_screenList->getCurrent();
        m_currentScreen->onEntry();
        m_currentScreen->setRunning();
        return true;
    }

    bool IMainGame::initSystems()
    {
        m_window.create("Application", 1280, 720, 0);
        glewExperimental = GL_TRUE;
        glewInit();
        return true;
    }

    void IMainGame::update(float deltaTime)
    {
        if(m_currentScreen)
        {
            switch (m_currentScreen->getState())
            {
                case ScreenState::RUNNING:
                    m_currentScreen->update(deltaTime);
                    break;
                case ScreenState::CHANGE_NEXT:
                    m_currentScreen->onExit();
                    m_currentScreen = m_screenList->moveNext();
                    if (m_currentScreen)
                    {
                        m_currentScreen->setRunning();
                        m_currentScreen->onEntry();
                    }
                    break;
                case ScreenState::CHANGE_PREV:
                    m_currentScreen->onExit();
                    m_currentScreen = m_screenList->movePrev();
                    if (m_currentScreen)
                    {
                        m_currentScreen->setRunning();
                        m_currentScreen->onEntry();
                    }
                    break;
                case ScreenState::EXIT_APP:
                    exitGame();
                    break;
                default:
                    break;
            }
        }
        else
        {
            exitGame();
        }
    }

    void IMainGame::draw(float deltaTime)
    {
        glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());
        if(m_currentScreen && m_currentScreen->getState() == ScreenState::RUNNING)
        {
            m_currentScreen->draw(deltaTime);
        }
    }

    void IMainGame::onSDLEvent(SDL_Event &event)
    {
        switch (event.type)
        {
            case SDL_QUIT:
                exitGame();
                break;
            case SDL_MOUSEMOTION:
                inputManager.setMouseCoords((float)event.motion.x, (float)event.motion.y);
                break;
            case SDL_KEYDOWN:
                inputManager.pressKey(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                inputManager.releaseKey(event.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                inputManager.pressKey(event.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                inputManager.releaseKey(event.button.button);
                break;
        }
    }
}