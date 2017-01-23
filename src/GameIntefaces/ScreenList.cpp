#include "ScreenList.h"
#include "IGameScreen.h"

namespace Falcon
{
    ScreenList::ScreenList(Falcon::IMainGame *game)
    : m_game(game)
    {

    }

    IGameScreen *Falcon::ScreenList::moveNext()
    {
        IGameScreen* currentScreen = getCurrent();
        if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN)
        {
            m_currentScreenIndex = currentScreen->getNextScreenIndex();
        }
        return getCurrent();
    }

    IGameScreen *Falcon::ScreenList::movePrev()
    {
        IGameScreen* currentScreen = getCurrent();
        if (currentScreen->getPrevScreenIndex() != SCREEN_INDEX_NO_SCREEN)
        {
            m_currentScreenIndex = currentScreen->getPrevScreenIndex();
        }
        return getCurrent();
    }

    void ScreenList::setScreen(int nextScreen)
    {
        m_currentScreenIndex = nextScreen;
    }

    void ScreenList::addScreen(Falcon::IGameScreen *newScreen)
    {
        newScreen->m_screenIndex = (int) m_screen.size();
        m_screen.push_back(newScreen);
        newScreen->create();
        newScreen->setParentGame(m_game);
    }

    void ScreenList::destroy()
    {
        for (int i = 0; i < m_screen.size(); i++)
        {
            m_screen[i]->destroy();
        }
        m_screen.resize(0);
        m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
    }

    IGameScreen *ScreenList::getCurrent()
    {
        if (m_currentScreenIndex == SCREEN_INDEX_NO_SCREEN)
            return nullptr;

        return m_screen[m_currentScreenIndex];
    }

    ScreenList::~ScreenList()
    {
        destroy();
    }
}

