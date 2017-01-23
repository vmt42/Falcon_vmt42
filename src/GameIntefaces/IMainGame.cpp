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

        FPSLimiter limiter;
        limiter.setTargetFPS(60.0f);
        m_isRunning = true;
        while (m_isRunning)
        {
            limiter.begin();

            inputManager.update();
            update();
            if (m_isRunning)
            {
                draw();

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
        m_window.create("Default", 1280, 720, 0);
        glewExperimental = GL_TRUE;
        glewInit();
        return true;
    }

    void IMainGame::update()
    {
        if(m_currentScreen)
        {
            switch (m_currentScreen->getState())
            {
                case ScreenState::RUNNING:
                    m_currentScreen->update();
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

    void IMainGame::draw()
    {
        glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());
        if(m_currentScreen && m_currentScreen->getState() == ScreenState::RUNNING)
        {
            m_currentScreen->draw();
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