#ifndef FALCONLATINUM_INPUTMANAGER_H
#define FALCONLATINUM_INPUTMANAGER_H

#include <unordered_map>

namespace Falcon
{
    class InputManager
    {
    public:
        InputManager() {};
        ~InputManager() {};


        void pressKey(unsigned int keyID);
        void releaseKey(unsigned int keyID);

        bool isKeyPressed(unsigned int keyID);

    private:
        std::unordered_map<unsigned int, bool> m_keyMap;

    };
}

#endif