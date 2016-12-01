#ifndef FALCONLATINUM_INPUTMANAGER_H
#define FALCONLATINUM_INPUTMANAGER_H

#include <unordered_map>
#include <glm/vec2.hpp>

namespace Falcon
{
    class InputManager
    {
    public:
        InputManager() : m_mouseCoords(0) {};
        ~InputManager() {};


        void pressKey(unsigned int keyID);
        void releaseKey(unsigned int keyID);
        void setMouseCoords(float x, float y);

        bool isKeyPressed(unsigned int keyID) const;
        glm::vec2 getMouseCoords() const { return m_mouseCoords; };

    private:
        std::unordered_map<unsigned int, bool> m_keyMap;
        glm::vec2 m_mouseCoords;
    };
}

#endif