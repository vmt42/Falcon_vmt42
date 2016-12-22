#include "InputManager.h"

namespace Falcon
{


    void InputManager::update()
    {
        for (auto& it : m_keyMap)
        {
            _previousKeyMap[it.first] = it.second;
        }
    }

    void InputManager::pressKey(unsigned int keyID)
    {
        m_keyMap[keyID] = true;
    }

    void InputManager::releaseKey(unsigned int keyID)
    {
        m_keyMap[keyID] = false;
    }

    bool InputManager::isKeyPressed(unsigned int keyID)
    {
        if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false) {
            return true;
        }
        return false;
    }

    void InputManager::setMouseCoords(float x, float y)
    {
        m_mouseCoords.x = x;
        m_mouseCoords.y = y;
    }

    bool InputManager::isKeyDown(unsigned int keyID)
    {
        auto it = m_keyMap.find(keyID);
        if (it != m_keyMap.end()) {
            // Found the key
            return it->second;
        } else {
            // Didn't find the key
            return false;
        }
    }
    bool InputManager::wasKeyDown(unsigned int keyID)
    {
        auto it = _previousKeyMap.find(keyID);
        if (it != _previousKeyMap.end())
        {
            return it->second;
        } else
        {
            return false;
        }
    }

}